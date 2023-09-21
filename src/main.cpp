#include <mogli/libmgr/filesystem/gameentry.hpp>
#include <mogli/logging.hpp>
#include <mogli/mogli.hpp>
#include <mogli/rest/config.hpp>
#include <mogli/rest/endpoint.hpp>

#include <CLI/CLI.hpp>

#include <assert.h>
#include <csignal>
#include <format>

#include <iostream>

mogli::rest::RESTEndpoint* endpoint = nullptr;
void sigintHandler(int signal) {
	assert(endpoint != nullptr);
	endpoint->stop();
}

struct RunAppArgs {
	bool quiet = false;
	/**
	 * @brief Sets the verbosity of the logger. Per default (verbosity=0) only critical, error and warnings are printed.
	 * If verbosity is...
	 *  - 1: info and above is printed
	 *  - 2: debug and above is printed
	 *  - 3+: trace and above is printed
	 */
	int verbosity = 0;
	mogli::lib::LibMgrConfig libConf;
	mogli::rest::RESTConfig restConf;
	mogli::lib::DBConfig dbConf;

	mogli::log::Verbosity getVerbosity() const noexcept {
		if (quiet)
			return mogli::log::Verbosity::Off;
		auto verb = static_cast<int>(mogli::log::Verbosity::Warning) + verbosity;
		verb = std::min(verb, static_cast<int>(mogli::log::Verbosity::Trace));
		return static_cast<mogli::log::Verbosity>(verb);
	}
};

struct ScanAppArgs {
	std::filesystem::path mediaRoot;
};

static void runRunCommand(const RunAppArgs args) {
	mogli::log::setVerbosity(args.getVerbosity());
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
	/** \todo implement **/
	throw std::runtime_error("Not yet implemented");
}

const std::string versionString = std::format("mogli v.{}", mogli::version);

int main(int argc, char* argv[]) {
	/** \todo Maybe add a better description of the app. **/
	CLI::App app("Manage your own game library.");
	app.set_version_flag("-v,--version", versionString);

	// Run Command
	RunAppArgs runArgs;
	CLI::App& runApp = *app.add_subcommand("run", "Runs the mogli REST-server and library manager");
	runApp.set_config("-c,--config");
	runApp.add_option("--media", runArgs.libConf.root, "The root directory of all media")
		->configurable(true)
		->envname("MOGLI_MEDIA_ROOT_DIR");
	runApp.add_option("--host", runArgs.restConf.host, "The host to bind the REST-server to")
		->default_val("localhost")
		->configurable(true)
		->envname("MOGLI_REST_HOST");
	runApp.add_option("--port", runArgs.restConf.port, "The port used by the REST-server")
		->default_val(8000)
		->configurable(true)
		->envname("MOGLI_REST_PORT");
	runApp.add_flag("--ipv4", runArgs.restConf.useIPv4, "Use IPv4 for the REST-server")
		->default_val(true)
		->configurable(true)
		->envname("MOGLI_REST_IPV4");
	runApp.add_option("--dbhost", runArgs.dbConf.host, "The database host to connect to")
		->configurable(true)
		->envname("MOGLI_DB_HOST");
	runApp.add_option("--dbport", runArgs.dbConf.port, "The database port to connect to")
		->default_val(5432)
		->configurable(true)
		->envname("MOGLI_DB_PORT");
	runApp.add_option("--dbname", runArgs.dbConf.dbname, "The name of the database")
		->configurable(true)
		->envname("MOGLI_DB_NAME");
	runApp.add_option("--dbuser", runArgs.dbConf.username, "The database user account to log into")
		->configurable(true)
		->envname("MOGLI_DB_USER");
	runApp.add_option("--dbpassword", runArgs.dbConf.password, "The database user password to log in with")
		->configurable(true)
		->envname("MOGLI_DB_PASSWORD");
	runApp.add_flag("-v,--verbose", runArgs.verbosity, "Sets the logger's verbosity. Passing it multiple times increases verbosity.");
	runApp.add_flag("-q,--quiet", runArgs.quiet, "Supresses all outputs");
	runApp.callback({[&runArgs]() { runRunCommand(runArgs); }});

	// Scan Command
	ScanAppArgs scanArgs;
	CLI::App& scanApp = *app.add_subcommand("scan", "Scans mogli's libraries and updates the database accordingly");
	scanApp.set_config("-c,--config");
	scanApp.add_option("--media", scanArgs.mediaRoot, "The root directory of all media")
		->configurable(true)
		->envname("MEDIA_ROOT_DIR");
	scanApp.callback({[&scanArgs]() { runScanCommand(scanArgs); }});
	
	CLI11_PARSE(app, argc, argv);
	return 0;
}