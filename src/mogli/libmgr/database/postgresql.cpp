#include <mogli/libmgr/database.hpp>

#include <mogli/logging.hpp>

#include <soci/postgresql/soci-postgresql.h>
#include <soci/soci.h>
#include <soci/std-optional.h>

#include <format>
#include <iostream>
#include <ranges>

using namespace mogli::lib;
using mogli::utils::Iterable;
using std::views::transform;

static GameDBEntry toGameDBEntry(const soci::row& row) {
	return GameDBEntry{
			.id = row.get<int>("id"),
			.path = row.get<std::string>("path"),
			.title = row.get<std::string>("title"),
			.description = row.get<std::optional<std::string>>("description")};
}

class PostgreGameDatabase final : public IGameDatabase {
public:
	static constexpr int CurrentSchemaVersion = 1;

private:
	soci::session session;
	mogli::log::LoggerPtr logger;

	struct ErrorCodes {
		static constexpr ErrorCode success = 0;
		static constexpr ErrorCode genericError = 1;
		static constexpr ErrorCode setupFailed = 2;
		static constexpr ErrorCode dbTooNew = 3;
	};

	bool tableExists(std::string table) noexcept {
		int exists;
		session << R"(SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_name = :tbl);)",
				soci::into(exists), soci::use(table);
		return exists != 0;
	}

	bool setupDatabase() noexcept {
		logger->info("Seting up the database schema");
		try {
			soci::transaction transaction(session);
			session.create_table("meta").column("version", soci::data_type::dt_integer);
			session << "INSERT INTO meta(version) VALUES (:version)", soci::use(CurrentSchemaVersion);
			session << R"(
                CREATE TABLE games (
                    id          SERIAL PRIMARY KEY,
                    title       TEXT NOT NULL,
                    description TEXT,
                    path        VARCHAR(255) NOT NULL UNIQUE
                );
            )";
			session << R"(
                CREATE TABLE tags (
                    gameid	INTEGER NOT NULL,
					tag		TEXT NOT NULL,
					PRIMARY KEY (gameid, tag),
					FOREIGN KEY (gameid) REFERENCES games(id)
                );
            )";
			transaction.commit();
			logger->info("Database was set up");
			return true;
		} catch (soci::soci_error& e) {
			logger->critical(
					"Failed to set up the database schema: [{}] {}", (int)e.get_error_category(), e.get_error_message()
			);
			return false;
		}
	}

	ErrorCode fetchTags(GameID gameid, std::vector<std::string>& tags) {
		try {
			session << "SELECT tag FROM tags WHERE gameid=:id", soci::into(tags), soci::use(gameid);
			return ErrorCodes::success;
		} catch (soci::soci_error& e) {
			logger->error(
					"Failed to fetch tags for gameid {}: [{}] {}", gameid, (int)e.get_error_category(),
					e.get_error_message()
			);
			return ErrorCodes::genericError;
		}
	}

	int fetchDBSchemaVersion() noexcept {
		try {
			int version = -1;
			session << "SELECT version FROM meta;", soci::into(version);
			return version;
		} catch (soci::soci_error& e) {
			/** \todo in the future this should be critical since it prohibits possibly migrating the db **/
			logger->error(
					"Failed to fetch the database schema version: [{}] {}", (int)e.get_error_category(),
					e.get_error_message()
			);
			return -1;
		}
	}

public:
	PostgreGameDatabase() : logger(mogli::log::getLogger("Database")) {
		/** \todo replace with actual calls to the logger in TRACE mode **/
		session.set_log_stream(&std::cout);
	}
	~PostgreGameDatabase() override {}

	ErrorCode setup(DBConfig config) noexcept override {
		try {
			logger->info("Connecting to database");
			auto connectString = std::format(
					"host={} port={} user={} password={} dbname={}", config.host, config.port, config.username,
					config.password, config.dbname
			);
			session.open(soci::postgresql, connectString);

			// Check if database was set up
			if (!tableExists("meta")) {
				logger->info("The database does not seem to be set up");
				if (!setupDatabase()) {
					teardown();
					return ErrorCodes::setupFailed;
				}
			}
			auto dbVersion = fetchDBSchemaVersion();
			logger->info("Database schema version: {}", dbVersion);
			if (dbVersion > CurrentSchemaVersion) {
				logger->critical(
						"The datbase version ({}) is newer than the supported schema ({})", dbVersion,
						CurrentSchemaVersion
				);
				return ErrorCodes::dbTooNew;
			} else if (dbVersion < CurrentSchemaVersion) {
				/** \todo implement migration **/
				logger->info("Migrating database from version {} to {}", dbVersion, CurrentSchemaVersion);
				return ErrorCodes::genericError;
			}

			return ErrorCodes::success;
		} catch (const soci::soci_error& e) {
			logger->critical(
					"Failed to connect to database [{}]: {}", (int)e.get_error_category(), e.get_error_message()
			);
			return ErrorCodes::genericError;
		}
	}

	ErrorCode teardown() noexcept override {
		session.close();
		return ErrorCodes::success;
	}

	ErrorCode addGame(mogli::lib::Game entry) noexcept override {
		try {
			soci::transaction transaction(session);
			/** \todo implement **/
			session << "INSERT INTO games(title, description, path) VALUES (:title, :description, :path)",
					soci::use(entry.title), soci::use(entry.description), soci::use((std::string)entry.path);
			transaction.commit();
			return ErrorCodes::genericError;
		} catch (soci::soci_error& e) {
			logger->critical(
					"Failed to add game to database [{}]: {}", (int)e.get_error_category(), e.get_error_message()
			);
			return ErrorCodes::genericError;
		}
	}

	std::variant<Iterable<GameDBEntry>, ErrorCode> games() noexcept override {
		try {
			soci::rowset<soci::row> rows = session.prepare << "SELECT id, title, description, path FROM games";
			return Iterable<GameDBEntry>(std::views::all(std::move(rows)) | transform(toGameDBEntry));
		} catch (soci::soci_error& e) {
			logger->critical("Failed to iterate games [{}]: {}", (int)e.get_error_category(), e.get_error_message());
			return ErrorCodes::genericError;
		}
	}

	ErrorCode getGame(GameID id, GameDBEntry& entry) noexcept override {
		try {
			std::string path;
			session << "SELECT id, title, description, path FROM games WHERE id=:id", soci::into(entry.id),
					soci::into(entry.title), soci::into(entry.description), soci::into(path), soci::use(id);
			entry.path = path;
			return ErrorCodes::success;
		} catch (soci::soci_error& e) {
			logger->error(
					"Failed to fetch game with id {} [{}]: {}", id, (int)e.get_error_category(), e.get_error_message()
			);
			return ErrorCodes::genericError;
		}
	}

	const char* getErrorMessage(ErrorCode error) noexcept override {
		switch (error) {
		case ErrorCodes::success:
			return "Success";
		case ErrorCodes::genericError:
			return "Generic Error";
		case ErrorCodes::setupFailed:
			return "Failed to Setup the DB Schema";
		case ErrorCodes::dbTooNew:
			return "Database is a higher version than this client";
		default:
			return "Invalid Error Code";
		};
	}
};

std::unique_ptr<IGameDatabase> mogli::lib::createPostgreSQLConnector() {
	return std::make_unique<PostgreGameDatabase>();
}
