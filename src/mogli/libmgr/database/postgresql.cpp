#include <mogli/libmgr/database.hpp>

#include <mogli/logging.hpp>

#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>

#include <format>

using namespace mogli::lib;

class PostgreGameDatabase final : public IGameDatabase {
private:
    soci::session session;
    mogli::log::LoggerPtr logger;

    struct ErrorCodes {
        static constexpr ErrorCode success = 0;
        static constexpr ErrorCode genericError = 1;
    };
public:
    PostgreGameDatabase() : logger(mogli::log::getLogger("Database")) {}
    ~PostgreGameDatabase() override {}

    ErrorCode setup(DBConfig config) noexcept override {
        auto connectString = std::format("host={} port={} user={} password={} dbname={}", config.host, config.port, config.username, config.password, config.dbname);
        try {
            session.open(soci::postgresql, connectString);
            return ErrorCodes::success;
        } catch (const soci::soci_error& e) {
            logger->error("Failed to connect to database [{}]: {}", (int)e.get_error_category(), e.get_error_message());
            return ErrorCodes::genericError;
        }
    }

    ErrorCode teardown() noexcept override {
        session.close();
        return ErrorCodes::success;
    }

    const char* getErrorMessage(ErrorCode error) noexcept override {
        switch(error) {
        case ErrorCodes::success:
            return "Success";
        case ErrorCodes::genericError:
            return "Generic Error";
        default:
            return "Invalid Error Code";
        };
    }
};

std::unique_ptr<IGameDatabase> mogli::lib::createPostgreSQLConnector() {
    return std::make_unique<PostgreGameDatabase>();
}
