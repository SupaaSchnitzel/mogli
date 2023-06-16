#include <mogli/rest/controller/controller.hpp>

#include <mogli/rest/dto/user.hpp>
#include <mogli/rest/dto/game.hpp>

using namespace mogli::rest;

std::shared_ptr<Controller::Response> Controller::getGames() {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}

std::shared_ptr<Controller::Response> Controller::getGame(const oatpp::String& gameid) {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}

std::shared_ptr<Controller::Response> Controller::deleteGame(const oatpp::String& gameid) {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}

std::shared_ptr<Controller::Response> Controller::getUsers() {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}

std::shared_ptr<Controller::Response> Controller::getUser(const oatpp::String& userid) {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}

std::shared_ptr<Controller::Response> Controller::getGamesFromUser(const oatpp::String& userid) {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}

std::shared_ptr<Controller::Response> Controller::getGameFromUser(const oatpp::String& userid, const oatpp::String& gameid) {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}
