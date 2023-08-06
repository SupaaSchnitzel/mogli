#include <mogli/libmgr/manager.hpp>

#include <exception>
#include <map>

using namespace mogli::lib;

static std::map<GameID, Game> gamesdata{
		{"0001",
		 Game{
				 .id = "0001",
				 .title = "The Witcher 3: Wild Hunt - Complete Edition",
				 .description = "Hello World",
				 .tags = {"Adventure", "Fantasy", "Role-playing"},
		 }},
		{"0002",
		 Game{
				 .id = "0002",
				 .title = "The Elder Scrolls V: Skyrim Anniversary Edition",
				 .description = "Hello World",
				 .tags = {"Adventure", "Fantasy", "Role-playing"},
		 }},
};

Games::Iterator::Iterator(T iterator) : value(iterator) {}
Games::Iterator& Games::Iterator::operator++() {
	value++;
	return *this;
}
Games::Iterator Games::Iterator::operator++(int) {
	auto retval = *this;
	++(*this);
	return retval;
}
bool Games::Iterator::operator==(Games::Iterator other) const noexcept { return value == other.value; }
bool Games::Iterator::operator!=(Games::Iterator other) const noexcept { return value != other.value; }
const Game& Games::Iterator::operator*() const noexcept { return value->second; }

Games::Iterator Games::begin() { return Games::Iterator(gamesdata.begin()); }

Games::Iterator Games::end() { return Games::Iterator(gamesdata.end()); }

Game Games::operator[](GameID id) const noexcept { return gamesdata[id]; }

LibraryManager::LibraryManager(LibMgrConfig config, IGameDatabase& database)
		: logger(mogli::log::getLogger("Library")), config(config), database(database) {
	logger->info("Initializing Library Manager");
}

std::string LibraryManager::getGameMetadata(GameID gameTitle) { throw std::runtime_error("Not implemented"); }

int LibraryManager::addGame(std::string gameInfo) { throw std::runtime_error("Not implemented"); }

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

int LibraryManager::scan() { throw std::runtime_error("Not implemented"); }

int LibraryManager::rescan() { throw std::runtime_error("Not implemented"); }

int LibraryManager::setGameMetadata(std::string url, GameID game) { throw std::runtime_error("Not implemented"); }