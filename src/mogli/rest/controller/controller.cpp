#include <mogli/rest/controller/controller.hpp>

#include <mogli/rest/dto/game.hpp>
#include <mogli/rest/dto/user.hpp>

using namespace mogli::rest;
using LibraryManager = mogli::lib::LibraryManager;

static dto::GameInfo::Wrapper toDTO(const mogli::lib::Game& game) noexcept {
	auto dto = dto::GameInfo::createShared();
	dto->id = game.id;
	if (game.title.has_value())
		dto->title = game.title.value();
	if (game.description.has_value())
		dto->description = game.description.value();
	dto->tags = oatpp::Vector<oatpp::String>::createShared();
	for (auto&& tag : game.tags)
		dto->tags->push_back(tag);
	dto->media = dto::GameMedia::createShared();
	dto->media->logo =
			"https://images.gog-statics.com/71d71c40052877a42c096b8230c71856a560d42132208cfa8e719243b5c6626b.jpg";
	dto->media->banner =
			"https://images.gog-statics.com/9f3a742af9acf163ed9ce23dece108fd416433dc71cf6392a9ee6f8c7a6e7afe.jpg";
	dto->media->boxart =
			"https://images.gog-statics.com/6278561a6c70684ab3cd86065ba337c4ae9dd5e3f63b5bd9997d2eba602d17cd.jpg";

	// dto->media->logo =
	// "https://images.gog-statics.com/16a042d9ad86c137414df42707c88a09339acc9b8dc495b9cd5fe895d6ec0af4.jpg";
	// dto->media->banner =
	// "https://images.gog-statics.com/e3088bde9f3e6fa08141df4c38c25e8c5a9404cbcae31dac29933a6f4c40550e.jpg";
	// dto->media->boxart =
	// "https://images.gog-statics.com/2d9394e5d51c8625c1c674f908c2267fe85caad145f05413ffa3a7664036d62a.jpg";
	return dto;
}

Controller::Controller(LibraryManager& libmgr, std::shared_ptr<ObjectMapper>& objectMapper)
		: ApiController(objectMapper), libmgr(libmgr) {}

std::shared_ptr<Controller::Response> Controller::getGames() {
	auto vector = oatpp::Vector<oatpp::Object<dto::GameInfo>>::createShared();
	std::transform(libmgr.games.begin(), libmgr.games.end(), std::back_inserter(*vector.get()), toDTO);
	return createDtoResponse(Status::CODE_200, vector);
}

std::shared_ptr<Controller::Response> Controller::getGame(const oatpp::String& gameid) {
	auto game = libmgr.games[gameid];
	auto dto = toDTO(game);
	return createDtoResponse(Status::CODE_200, dto);
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

std::shared_ptr<Controller::Response>
Controller::getGameFromUser(const oatpp::String& userid, const oatpp::String& gameid) {
	/** \todo implement */
	return createResponse(Status::CODE_200, "TODO");
}
