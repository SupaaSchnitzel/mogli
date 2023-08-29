#include <mogli/rest/endpoint.hpp>

#include <mogli/rest/controller/controller.hpp>

#include <oatpp/web/server/interceptor/AllowCorsGlobal.hpp>

using mogli::lib::LibraryManager;
using mogli::rest::RESTEndpoint;
using mogli::rest::RESTConfig;
using mogli::rest::Controller;

using oatpp::web::server::interceptor::AllowCorsGlobal;
using oatpp::web::server::interceptor::AllowOptionsGlobal;

unsigned RESTEndpoint::InstanceCounter = 0;

RESTEndpoint::RESTEndpoint(LibraryManager& libmgr, RESTConfig config)
		: logger(mogli::log::getLogger("REST")), libmgr(libmgr), config(config) {}

bool RESTEndpoint::init() {
	try {
		logger->info("Initializing REST Endpoint...");
		// Create the global oatpp environment if this is the first endpoint to use it
		if (RESTEndpoint::InstanceCounter == 0) {
			oatpp::base::Environment::init();
			logger->info("Created oatpp environment");
		}
		++RESTEndpoint::InstanceCounter;

		OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)
		([] {
			auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
			objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
			return objectMapper;
		}());

		//
		logger->info("Initializing HTTP router");
		router = HttpRouter::createShared();

		auto controller = router->addController(std::make_shared<Controller>(libmgr));

		logger->info("Initializing connection handler");
		auto connectionHandler = HttpConnectionHandler::createShared(router);
		connectionHandler->addRequestInterceptor(std::make_shared<AllowOptionsGlobal>());
		connectionHandler->addResponseInterceptor(std::make_shared<AllowCorsGlobal>());
		logger->info("Initializing TCP connection provider to listen to {}:{}", config.host, config.port);
		auto version = config.useIPv4 ? oatpp::network::Address::IP_4 : oatpp::network::Address::IP_6;
		connectionProvider = ConnectionProvider::createShared({config.host, config.port, version});
		logger->info("Initializing server");
		server = Server::createShared(connectionProvider, connectionHandler);
		return true;
	} catch (std::runtime_error& e) {
		logger->critical("Failed to initialize REST Endpoint: {}", e.what());
		return false;
	}
}

void RESTEndpoint::deinit() {
	logger->info("Deinitializing server");
	connectionProvider = nullptr;
	server = nullptr;
	router = nullptr;

	// Destroy the global oatpp environment if this was the last endpoint to use it
	--RESTEndpoint::InstanceCounter;
	if (RESTEndpoint::InstanceCounter == 0) {
		oatpp::base::Environment::destroy();
		logger->info("Destroyed oatpp environment");
	}
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
