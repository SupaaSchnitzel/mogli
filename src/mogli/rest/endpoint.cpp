#include <mogli/rest/endpoint.hpp>

#include <mogli/rest/controller/controller.hpp>

using RESTEndpoint = mogli::rest::RESTEndpoint;
using RESTConfig = mogli::rest::RESTConfig;
using Controller = mogli::rest::Controller;

unsigned RESTEndpoint::InstanceCounter = 0;

RESTEndpoint::RESTEndpoint(RESTConfig config) : logger(mogli::log::getLogger("REST")), config(config) {}

bool RESTEndpoint::init() {
	// Create the global oatpp environment if this is the first endpoint to use it
	if (RESTEndpoint::InstanceCounter == 0)
		oatpp::base::Environment::init();
	++RESTEndpoint::InstanceCounter;

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([]{
		auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
		objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
		return objectMapper;
	}());

	// 
	logger->info("Initializing HTTP router");
	router = HttpRouter::createShared();

	auto controller = router->addController(Controller::createShared());

	logger->info("Initializing connection handler");
	auto connectionHandler = HttpConnectionHandler::createShared(router);
	logger->info("Initializing TCP connection provider");
	auto version = config.useIPv4 ? oatpp::network::Address::IP_4 : oatpp::network::Address::IP_6;
	connectionProvider = ConnectionProvider::createShared({config.host, config.port, version});
	logger->info("Initializing server");
	server = Server::createShared(connectionProvider, connectionHandler);
	return true;
}

void RESTEndpoint::deinit() {
	logger->info("Deinitializing server");
	connectionProvider = nullptr;
	server = nullptr;
	router = nullptr;

	// Destroy the global oatpp environment if this was the last endpoint to use it
	--RESTEndpoint::InstanceCounter;
	if (RESTEndpoint::InstanceCounter == 0)
		oatpp::base::Environment::destroy();
	logger->info("Server deinitialized");
}

void RESTEndpoint::run() {
	logger->info("REST server running on port {}", *connectionProvider->getProperty("port").toString());
	server->run();
}

void RESTEndpoint::stop() {
	logger->info("Stopping REST endpoint");
	server->stop();
}
