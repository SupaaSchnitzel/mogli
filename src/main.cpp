#include <mogli/libmgr/filesystem/gameentry.hpp>
#include <mogli/logging.hpp>
#include <mogli/mogli.hpp>
#include <mogli/rest/config.hpp>
#include <mogli/rest/endpoint.hpp>

#include <CLI/CLI.hpp>

#include <assert.h>
#include <csignal>
#include <format>
#include <ranges>

#include <iostream>

mogli::rest::RESTEndpoint* endpoint = nullptr;
void sigintHandler(int signal) {
	assert(endpoint != nullptr);
	endpoint->stop();
}

struct LoggerConf {
	bool quiet = false;
	/**
	 * @brief Sets the verbosity of the logger. Per default (verbosity=0) only critical, error and warnings are printed.
	 * If verbosity is...
	 *  - 1: info and above is printed
	 *  - 2: debug and above is printed
	 *  - 3+: trace and above is printed
	 */
	int verbosity = 0;

	mogli::log::Verbosity getVerbosity() const noexcept {
		if (quiet)
			return mogli::log::Verbosity::Off;
		auto verb = static_cast<int>(mogli::log::Verbosity::Warning) + verbosity;
		verb = std::min(verb, static_cast<int>(mogli::log::Verbosity::Trace));
		return static_cast<mogli::log::Verbosity>(verb);
	}
};

struct RunAppArgs final {
	LoggerConf logConf;
	mogli::lib::LibMgrConfig libConf;
	mogli::rest::RESTConfig restConf;
	mogli::lib::DBConfig dbConf;
};

struct ScanAppArgs final {
	LoggerConf logConf;
	mogli::lib::LibMgrConfig libConf;
	mogli::lib::DBConfig dbConf;
	bool dryrun = false; /**< true iff the scanner should only log what it would do instead of actually modifying the database **/
};

static void runRunCommand(const RunAppArgs args) {
	mogli::log::setVerbosity(args.logConf.getVerbosity());
	auto logger = mogli::log::getLogger("Mogli");
	logger->info("Launching mogli v.{}", mogli::version);
	auto database = mogli::lib::createPostgreSQLConnector();
	auto dbError = database->setup(args.dbConf);
	if (dbError == mogli::lib::IGameDatabase::Success) {
		mogli::lib::LibraryManager libmgr(args.libConf, *database);
		mogli::rest::RESTEndpoint endpoint(libmgr, args.restConf);
		::endpoint = &endpoint;

		if (endpoint.init()) {
			logger->info("Instantiating sigint handler");
			std::signal(SIGINT, sigintHandler);

			logger->info("Setup done");

			endpoint.run();

			logger->info("Exiting");
			endpoint.deinit();
		}
		database->teardown();
	} else {
		logger->critical("Failed to setup database: {}", database->getErrorMessage(dbError));
	}
	::endpoint = nullptr;
	logger->info("Exited");
}

static void runScanCommand(const ScanAppArgs args) {
	mogli::log::setVerbosity(args.logConf.getVerbosity());
	auto logger = mogli::log::getLogger("Mogli");
	logger->info("Launching mogli v.{}", mogli::version);
	auto database = mogli::lib::createPostgreSQLConnector();
	auto dbError = database->setup(args.dbConf);
	if (dbError == mogli::lib::IGameDatabase::Success) {
		mogli::lib::LibraryManager libmgr(args.libConf, *database);
		libmgr.scanAll(args.dryrun);
		database->teardown();
	} else {
		logger->critical("Failed to setup database: {}", database->getErrorMessage(dbError));
	}
	logger->info("Exited");
}

void setupLoggerArgs(CLI::App& app, LoggerConf& conf) {
	app.add_flag(
			"-v,--verbose", conf.verbosity,
			"Sets the logger's verbosity. Passing it multiple times increases verbosity."
	);
	app.add_flag("-q,--quiet", conf.quiet, "Supresses all outputs");
}

void setupLibMgrArgs(CLI::App& app, mogli::lib::LibMgrConfig& conf) {
	app.add_option("--media", conf.root, "The root directory of all media")
			->configurable(true)
			->envname("MOGLI_MEDIA_ROOT_DIR");
}

void setupRESTArgs(CLI::App& app, mogli::rest::RESTConfig& conf) {
	app.add_option("--host", conf.host, "The host to bind the REST-server to")
			->default_val("localhost")
			->configurable(true)
			->envname("MOGLI_REST_HOST");
	app.add_option("--port", conf.port, "The port used by the REST-server")
			->default_val(8000)
			->configurable(true)
			->envname("MOGLI_REST_PORT");
	app.add_flag("--ipv4", conf.useIPv4, "Use IPv4 for the REST-server")
			->default_val(true)
			->configurable(true)
			->envname("MOGLI_REST_IPV4");
}

void setupDBArgs(CLI::App& app, mogli::lib::DBConfig& conf) {
	app.add_option("--dbhost", conf.host, "The database host to connect to")
			->configurable(true)
			->envname("MOGLI_DB_HOST");
	app.add_option("--dbport", conf.port, "The database port to connect to")
			->default_val(5432)
			->configurable(true)
			->envname("MOGLI_DB_PORT");
	app.add_option("--dbname", conf.dbname, "The name of the database")
			->configurable(true)
			->envname("MOGLI_DB_NAME");
	app.add_option("--dbuser", conf.username, "The database user account to log into")
			->configurable(true)
			->envname("MOGLI_DB_USER");
	app.add_option("--dbpassword", conf.password, "The database user password to log in with")
			->configurable(true)
			->envname("MOGLI_DB_PASSWORD");
}

const std::string mogliVersion = std::format("mogli v.{}", mogli::version);
const std::string cliVersion = "CLI11 v." CLI11_VERSION;
const std::string timestamp = "Commit " GIT_COMMIT_HASH;

int main(int argc, char* argv[]) {
	/** \todo Maybe add a better description of the app. **/
	CLI::App app("Manage your own game library.");
	std::vector<std::string> libs{
			mogliVersion, cliVersion, mogli::log::getVersionStr(), mogli::rest::getVersionStr(), timestamp
	};
	/** \todo Use std::format instead of fmt. **/
	app.set_version_flag("-v,--version", fmt::to_string(fmt::join(libs, "\n")));

	// Run Command
	RunAppArgs runArgs;
	CLI::App& runApp = *app.add_subcommand("run", "Runs the mogli REST-server and library manager");
	runApp.set_config("-c,--config");
	setupLoggerArgs(runApp, runArgs.logConf);
	setupLibMgrArgs(runApp, runArgs.libConf);
	setupRESTArgs(runApp, runArgs.restConf);
	setupDBArgs(runApp, runArgs.dbConf);
	runApp.callback({[&runArgs]() { runRunCommand(runArgs); }});

	// Scan Command
	ScanAppArgs scanArgs;
	CLI::App& scanApp = *app.add_subcommand("scan", "Scans mogli's libraries and updates the database accordingly");
	scanApp.set_config("-c,--config");
	scanApp.add_flag("--dryrun", scanArgs.dryrun, "If set, the database will not be modified");
	setupLoggerArgs(scanApp, scanArgs.logConf);
	setupLibMgrArgs(scanApp, scanArgs.libConf);
	setupDBArgs(scanApp, scanArgs.dbConf);
	scanApp.callback({[&scanArgs]() { runScanCommand(scanArgs); }});

	CLI11_PARSE(app, argc, argv);
	return 0;
}