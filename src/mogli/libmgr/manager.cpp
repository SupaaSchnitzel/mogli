#include <mogli/libmgr/manager.hpp>

using namespace mogli::lib;
using libConfig = mogli::lib::libConfig;

LibraryManager::LibraryManager(libConfig config, IGameDatabase& database)
		: logger(mogli::log::getLogger("Library")), config(config), database(database) {}

std::string LibraryManager::getGameMetadata(GameID gameTitle) {
	switch (config.gamedb) {
	case steam:
		return getGameMetadataSteam(gameTitle);
	case gog:
		return getGameMetadataGog(gameTitle);
	case gogdb:
		return getGameMetadataGogdb(gameTitle);
	}
}

std::string LibraryManager::getGameMetadataGog(GameID gameTitle) { return "TODO"; }

std::string LibraryManager::getGameMetadataGogdb(GameID gameTitle) { return "TODO"; }

std::string LibraryManager::getGameMetadataSteam(GameID gameTitle) { return "TODO"; }

int LibraryManager::addGame(std::string gameInfo) { return -1; }

int LibraryManager::scanThread() {
	if (!std::filesystem::is_directory(config.root))
		return -1;
	for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator{config.root}) {
		if (!dir_entry.is_directory())
			continue;
		std::string gameTitle = dir_entry.path().filename();
		std::string gameInfo = getGameMetadata(gameTitle);
		if (!addGame(gameInfo))
			return -1;
	}
	return -1;
}

int LibraryManager::rescanThread() {
	if (!std::filesystem::is_directory(config.root))
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
	return -1;
}

int LibraryManager::scan() { return -1; }

int LibraryManager::rescan() { return -1; }

int LibraryManager::setGameMetadata(std::string url, GameID game) { return -1; }