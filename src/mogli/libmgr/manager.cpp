#include <mogli/libmgr/manager.hpp>

#include <mogli/libmgr/filesystem/gameentry.hpp>

#include <exception>
#include <map>

using namespace mogli::lib;
using mogli::utils::Iterable;
using std::views::transform;

#include <iostream>

static Game toGame(GameDBEntry entry) {
	std::map<std::string, std::filesystem::path> media;
	/** \todo remove hardcoded, use config **/
	auto globalpath = std::filesystem::path("/workspaces/mogli/tests/testfiles/media") / entry.path;
	GameEntry file{std::filesystem::directory_entry(globalpath)};
	auto addToMedia = [&media](std::string name) { return [&media, name](auto path) { return media[name] = path; }; };
	file.getLogo().transform(addToMedia("logo"));
	file.getBanner().transform(addToMedia("banner"));
	file.getBoxart().transform(addToMedia("boxart"));
	return Game{
			.id = entry.id,
			.path = globalpath,
			.title = entry.title,
			.description = entry.description,
			// The GameDBEntry does not contain tags since they have to be fetched manually via IGameDatabase::fetchTags
			.tags = {},
			.media = media
	};
}

Games::Games(LibraryManager& libmgr) noexcept : libmgr(libmgr) {}

Game Games::operator[](GameID id) const noexcept {
	GameDBEntry entry;
	auto error = libmgr.database.getGame(id, entry);
	if (error == IGameDatabase::Success) {
		auto game = toGame(entry);
		libmgr.database.fetchTags(game.id, game.tags);
		return game;
	} else {
		libmgr.logger->debug("Failed to fetch game by id ({}): {}", id, libmgr.database.getErrorMessage(error));
		return {.id = InvalidGameID};
	}
}

Iterable<Game> Games::all() const noexcept {
	Iterable<GameDBEntry> games;
	auto error = libmgr.database.fetchGames(games);
	if (error == IGameDatabase::Success)
		return Iterable<Game>(games | transform(toGame));
	libmgr.logger->error("Failed to fetch games: {}", libmgr.database.getErrorMessage(error));
	return Iterable<Game>();
}

LibraryManager::LibraryManager(LibMgrConfig config, IGameDatabase& database)
		: logger(mogli::log::getLogger("Library")), config(config), database(database), games(*this) {
	logger->info("Initializing Library Manager");
}

std::string LibraryManager::getGameMetadata(GameID gameTitle) { throw std::runtime_error("Not implemented"); }

int LibraryManager::addGame(std::string gameInfo) { throw std::runtime_error("Not implemented"); }

int LibraryManager::setGameMetadata(std::string url, GameID game) { throw std::runtime_error("Not implemented"); }