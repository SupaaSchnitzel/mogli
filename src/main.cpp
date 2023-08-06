#include <assert.h>
#include <csignal>

#include <iostream>

#include <mogli/libmgr/filesystem/gameentry.hpp>
#include <mogli/logging.hpp>
#include <mogli/mogli.hpp>
#include <mogli/rest/config.hpp>
#include <mogli/rest/endpoint.hpp>

mogli::rest::RESTEndpoint* endpoint = nullptr;
void sigintHandler(int signal) {
	assert(endpoint != nullptr);
	endpoint->stop();
}

int main(int argc, char* argv[]) {
	//mogli::lib::Scanner scanner;
	//scanner.scan("/media");

	mogli::lib::GameEntry entry(std::filesystem::directory_entry(std::filesystem::path("/media/test [hint-xyz] [hint2-abc] (2009)")));
	std::cout << entry.getHint("hint").value_or("___") << std::endl;
	std::cout << entry.getHint("hint2").value_or("___") << std::endl;
	std::cout << entry.getHint("hint3").value_or("___") << std::endl;
	std::cout << entry.getReleaseYear().value_or(0) << std::endl;
	std::cout << entry.getName() << std::endl;

	auto logger = mogli::log::getLogger("Mogli");
	logger->info("Launching mogli v.{}", mogli::version);
	mogli::lib::PostgreGameDatabase database;
	mogli::lib::LibMgrConfig config{.root = "/media"};
	mogli::lib::LibraryManager libmgr(config, database);
	mogli::rest::RESTConfig conf = {
			.host = "localhost",
			.port = 8000,
			.useIPv4 = true,
	};
	mogli::rest::RESTEndpoint endpoint(libmgr, conf);
	::endpoint = &endpoint;
	endpoint.init();

	logger->info("Instanting sigint handler");
	std::signal(SIGINT, sigintHandler);
	
	logger->info("Setup done");

	endpoint.run();

	logger->info("Exiting");
	endpoint.deinit();
	::endpoint = nullptr;

	return 0;
}