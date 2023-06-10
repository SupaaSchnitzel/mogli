#include <assert.h>
#include <csignal>

#include <mogli/libmgr/manager.hpp>
#include <mogli/logging.hpp>
#include <mogli/mogli.hpp>
#include <mogli/rest/endpoint.hpp>
#include <mogli/rest/config.hpp>


mogli::rest::RESTEndpoint* endpoint = nullptr;
void sigintHandler(int signal) {
	assert(endpoint != nullptr);
	endpoint->stop();
}

int main(int argc, char* argv[]) {
	auto logger = mogli::log::getLogger("Mogli");
	logger->info("Launching mogli v.{}", mogli::version);
	mogli::lib::PostgreGameDatabase database;
	mogli::lib::LibraryManager libmgr("/library", database);
	mogli::rest::RESTConfig conf = {
		.host = "localhost",
		.port = 8000,
		.useIPv4 = true,
	};
	mogli::rest::RESTEndpoint endpoint(conf);
	::endpoint = &endpoint;
	logger->info("Initializing endpoint");
	endpoint.init();
	logger->info("Instating sigint handler");

	std::signal(SIGINT, sigintHandler);
	logger->info("Setup done");

	endpoint.run();

	logger->info("Exiting");
	endpoint.deinit();
	::endpoint = nullptr;

	return 0;
}