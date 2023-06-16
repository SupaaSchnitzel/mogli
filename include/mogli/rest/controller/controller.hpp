#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace mogli::rest {
	/**
	 * @brief TODO: add documentation
	 * 
	 */
	class Controller final : public oatpp::web::server::api::ApiController {
	public:
		using ApiController = oatpp::web::server::api::ApiController;
		using Response = oatpp::web::protocol::http::outgoing::Response;

	private:
	public:
		/**
		 * @brief TODO: add documentation
		 * 
		 * @param objectMapper 
		 */
		explicit Controller(const std::shared_ptr<ObjectMapper>& objectMapper) : ApiController(objectMapper) {}

		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("GET", "/games", getGames);
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("GET", "/games/{gameid}", getGame, PATH(oatpp::String, gameid));
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("DELETE", "/games/{gameid}", deleteGame, PATH(oatpp::String, gameid));
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("GET", "/users", getUsers);
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("GET", "/users/{userid}", getUser, PATH(oatpp::String, userid));
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("GET", "/users/{userid}/games", getGamesFromUser, PATH(oatpp::String, userid));
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT(
				"GET", "/users/{userid}/games/{gameid}", getGameFromUser, PATH(oatpp::String, userid),
				PATH(oatpp::String, gameid)
		);

		/**
		 * @brief TODO: add documentation
		 * 
		 * @return std::shared_ptr<Controller> 

		 */
		static std::shared_ptr<Controller> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
			return std::make_shared<Controller>(objectMapper);
		}
	};
} // namespace mogli::rest

#include OATPP_CODEGEN_END(ApiController)