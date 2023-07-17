#pragma once

#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

#include "../libmgr/manager.hpp"
#include "../logging.hpp"
#include "config.hpp"

namespace mogli::rest {
	/**
	 * @brief RESTEndpoint implements a server that provides the REST-API interface to external tools.
	 */
	class RESTEndpoint final {
	private:
		using HttpRouter = oatpp::web::server::HttpRouter;
		using HttpConnectionHandler = oatpp::web::server::HttpConnectionHandler;
		using ConnectionProvider = oatpp::network::tcp::server::ConnectionProvider;
		using Server = oatpp::network::Server;
		std::shared_ptr<HttpRouter> router;
		std::shared_ptr<ConnectionProvider> connectionProvider;
		std::shared_ptr<Server> server;

		/**
		 * @brief The logger which should be used by RESTEndpoint instances.
		 */
		const mogli::log::LoggerPtr logger;
		mogli::lib::LibraryManager& libmgr;
		const RESTConfig config;

		static unsigned InstanceCounter;

	public:
		/**
		 * @brief Creates a new instance from the provided configurations.
		 * 
		 * @param config the
		 * config that should be used for anything related to the RESTEndpoint.
		 */
		explicit RESTEndpoint(mogli::lib::LibraryManager& libmgr, RESTConfig config);
		/**
		 * @brief Initializes this endpoint. This method should only be called once per instance and, if
		 * successful,
		 * must be followed by a call to #deinit
		 * 
		 * @return true on success, else false. 

		 */
		bool init();
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		void deinit();

		/**
		 * @brief TODO: add documentation
		 * 
		 */
		void run();
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		void stop();
	};
} // namespace mogli::rest