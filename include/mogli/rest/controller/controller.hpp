#pragma once

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace mogli::rest {
	class Controller final : public oatpp::web::server::api::ApiController {
	public:
		using ApiController = oatpp::web::server::api::ApiController;
		using Response = oatpp::web::protocol::http::outgoing::Response;
	private:

	public:
		Controller(const std::shared_ptr<ObjectMapper>& objectMapper) : ApiController(objectMapper) {}

		ENDPOINT("GET", "/games", getGames);
		ENDPOINT("GET", "/games/{gameid}", getGame, PATH(oatpp::String, gameid));
		ENDPOINT("DELETE", "/games/{gameid}", deleteGame, PATH(oatpp::String, gameid));
		ENDPOINT("GET", "/users", getUsers);
		ENDPOINT("GET", "/users/{userid}", getUser, PATH(oatpp::String, userid));
		ENDPOINT("GET", "/users/{userid}/games", getGamesFromUser, PATH(oatpp::String, userid));
		ENDPOINT("GET", "/users/{userid}/games/{gameid}", getGameFromUser, PATH(oatpp::String, userid), PATH(oatpp::String, gameid));

		static std::shared_ptr<Controller> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
			return std::make_shared<Controller>(objectMapper);
		}
	};
}

#include OATPP_CODEGEN_END(ApiController)