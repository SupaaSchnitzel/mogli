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
	return Game {
		.id = entry.id,
		.path = globalpath,
		.title = entry.title,
		.description = entry.description,
		.tags = {"TODO"}, /** \todo implement */
		.media = media
	};
}

Games::Games(LibraryManager& libmgr) noexcept : libmgr(libmgr) {}

Game Games::operator[](GameID id) const noexcept {
	GameDBEntry entry;
	auto error = libmgr.database.getGame(id, entry);
	if (error == IGameDatabase::Success) {
		return toGame(entry);
	} else {
		/** \todo log error **/
		return {};
	}
}

Iterable<Game> Games::all() const noexcept {
	auto games = libmgr.database.games();
	if (std::holds_alternative<Iterable<GameDBEntry>>(games))
		return Iterable<Game>(std::get<Iterable<GameDBEntry>>(games) | transform(toGame));
	/** \todo use logger **/
	std::cout << "Failed to fetch games: " << libmgr.database.getErrorMessage(std::get<IGameDatabase::ErrorCode>(games)) << std::endl;
	//logger->error("Failed to fetch games: {}", libmgr.database.getErrorMessage(std::get<IGameDatabase::ErrorCode>(games)));
	return Iterable<Game>();
}

LibraryManager::LibraryManager(LibMgrConfig config, IGameDatabase& database)
		: logger(mogli::log::getLogger("Library")), config(config), database(database), games(*this) {
	logger->info("Initializing Library Manager");
}

std::string LibraryManager::getGameMetadata(GameID gameTitle) { throw std::runtime_error("Not implemented"); }

int LibraryManager::addGame(std::string gameInfo) { throw std::runtime_error("Not implemented"); }

int LibraryManager::scanThread() {
	/*if (!std::filesystem::is_directory(config.root))
		return -1;
	for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator{config.root}) {
		if (!dir_entry.is_directory())
			continue;
		std::string gameTitle = dir_entry.path().filename();
		std::string gameInfo = getGameMetadata(gameTitle);
		if (!addGame(gameInfo))
			return -1;
	}
	return -1;*/
	throw std::runtime_error("Not implemented");
}

int LibraryManager::rescanThread() {
	/*if (!std::filesystem::is_directory(config.root))
		return -1;
	for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator{config.root}) {
		if (!dir_entry.is_directory())
			continue;
		std::string gameTitle = dir_entry.path().filename();
		// TODO CHECK if Game is in database
		std::string gameInfo = getGameMetadata(gameTitle);
		if (!addGame(gameInfo))
			return -1;
	}
	return -1;*/
	throw std::runtime_error("Not implemented");
}

int LibraryManager::scan() { throw std::runtime_error("Not implemented"); }

int LibraryManager::rescan() { throw std::runtime_error("Not implemented"); }

int LibraryManager::setGameMetadata(std::string url, GameID game) { throw std::runtime_error("Not implemented"); }