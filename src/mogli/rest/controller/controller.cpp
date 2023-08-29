#include <mogli/rest/controller/controller.hpp>

#include <mogli/rest/dto/game.hpp>
#include <mogli/rest/dto/user.hpp>

#include <oatpp/web/protocol/http/outgoing/StreamingBody.hpp>
#include <oatpp/core/data/stream/FileStream.hpp>

#include <fstream>

using namespace mogli::rest;
using LibraryManager = mogli::lib::LibraryManager;

static dto::GameInfo::Wrapper toDTO(const mogli::lib::Game& game) noexcept {
	auto dto = dto::GameInfo::createShared();
	dto->id = game.id;
	dto->title = game.title;
	if (game.description.has_value())
		dto->description = game.description.value();
	dto->tags = oatpp::Vector<oatpp::String>::createShared();
	for (auto&& tag : game.tags)
		dto->tags->push_back(tag);
	dto->media = dto::GameMedia::createShared();
	dto->media->logo = std::format("/games/{}/media/logo", game.id);
	dto->media->banner = std::format("/games/{}/media/banner", game.id);
	dto->media->boxart = std::format("/games/{}/media/boxart", game.id);

	// dto->media->logo =
	// "https://images.gog-statics.com/16a042d9ad86c137414df42707c88a09339acc9b8dc495b9cd5fe895d6ec0af4.jpg";
	// dto->media->banner =
	// "https://images.gog-statics.com/e3088bde9f3e6fa08141df4c38c25e8c5a9404cbcae31dac29933a6f4c40550e.jpg";
	// dto->media->boxart =
	// "https://images.gog-statics.com/2d9394e5d51c8625c1c674f908c2267fe85caad145f05413ffa3a7664036d62a.jpg";
	return dto;
}

Controller::Controller(LibraryManager& libmgr, std::shared_ptr<ObjectMapper>& objectMapper)
		: ApiController(objectMapper), logger(mogli::log::getLogger("REST")), libmgr(libmgr) {}

std::shared_ptr<Controller::Response> Controller::getGames() {
	logger->trace("/games");
	auto vector = oatpp::Vector<oatpp::Object<dto::GameInfo>>::createShared();
	auto games = libmgr.games.all();
	std::transform(games.begin(), games.end(), std::back_inserter(*vector.get()), toDTO);
	return createDtoResponse(Status::CODE_200, vector);
}

std::shared_ptr<Controller::Response> Controller::getGame(const oatpp::Int32& gameid) {
	logger->trace("/game/{}", *gameid.get());
	auto game = libmgr.games[*gameid.get()];
	auto dto = toDTO(game);
	return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<Controller::Response> Controller::deleteGame(const oatpp::Int32& gameid) {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}

std::shared_ptr<Controller::Response> Controller::getGameMedia(const oatpp::Int32& gameid, const oatpp::String& media) {
	logger->trace("/game/{}/media/{}", gameid.getValue(0), media.getValue(""));
	auto game = libmgr.games[gameid];
	auto iterator = game.media.find(media.getValue(""));
	if (iterator == game.media.end()) {
		return createResponse(Status::CODE_404, "");
	}
	auto path = iterator->second;
	auto file = std::make_shared<oatpp::data::stream::FileInputStream>(path.c_str());
	auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(file);
	return Response::createShared(Status::CODE_200, body);
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

std::shared_ptr<Controller::Response>
Controller::getGameFromUser(const oatpp::String& userid, const oatpp::String& gameid) {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}
