#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "../../libmgr/manager.hpp"

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
		mogli::log::LoggerPtr logger;
		mogli::lib::LibraryManager& libmgr;

	public:
		/**
		 * @brief TODO: add documentation
		 * 
		 * @param objectMapper
		 * @param libmgr

		 */
		explicit Controller(
				mogli::lib::LibraryManager& libmgr, OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)
		);

		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("GET", "/games", getGames);
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("GET", "/games/{gameid}", getGame, PATH(oatpp::Int32, gameid));
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("DELETE", "/games/{gameid}", deleteGame, PATH(oatpp::Int32, gameid));
		/**
		 * @brief TODO: add documentation
		 * 
		 */
		ENDPOINT("GET", "/games/{gameid}/media/{media}", getGameMedia, PATH(oatpp::Int32, gameid), PATH(oatpp::String, media));
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
	};
} // namespace mogli::rest

#include OATPP_CODEGEN_END(ApiController)