#pragma once

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include "config.hpp"
#include "../logging.hpp"

namespace mogli::rest {
	class RESTEndpoint final {
	private:
		using HttpRouter = oatpp::web::server::HttpRouter;
		using HttpConnectionHandler = oatpp::web::server::HttpConnectionHandler;
		using ConnectionProvider = oatpp::network::tcp::server::ConnectionProvider;
		using Server = oatpp::network::Server;
		std::shared_ptr<HttpRouter> router;
		std::shared_ptr<ConnectionProvider> connectionProvider;
		std::shared_ptr<Server> server;
		
		const mogli::log::LoggerPtr logger;
		const RESTConfig config;

		static unsigned InstanceCounter;
	public:
		RESTEndpoint(RESTConfig config);
		bool init();
		void deinit();

		void run();
		void stop();
	};
}