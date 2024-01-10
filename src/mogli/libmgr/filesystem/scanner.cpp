#include <mogli/libmgr/filesystem/scanner.hpp>

#include <mogli/libmgr/filesystem/gameentry.hpp>
#include <mogli/libmgr/manager.hpp>

using namespace mogli::lib;

Scanner::Scanner(LibraryManager& libmgr) noexcept : logger(mogli::log::getLogger("Scanner")), libmgr(libmgr) {}

void Scanner::merge() const noexcept {

}

void Scanner::scan(std::filesystem::path path) const noexcept {
	logger->info("Scanning {}...", path.c_str());
	unsigned newlyAdded = 0;
	unsigned updated = 0;
	for (auto const& entry : std::filesystem::directory_iterator(path)) {
		logger->trace("{}", entry.path().c_str());
		GameDBEntry dbentry;
		auto error = libmgr.database.getGame(entry.path(), dbentry);
		if (error != IGameDatabase::Success) {
			logger->info("The database contains no game stored at {}", entry.path().c_str());
			//libmgr.database.addGame(game);
		} else {
			// TODO: implement

		}
		/** \todo implement **/
	}
	logger->info("Scan finished. Added {} new games and updated {}", newlyAdded, updated);
}

void Scanner::scanSingle(const GameEntry& game) const noexcept {
	/*GameDBEntry dbentry;
	auto error = libmgr.database.getGame(game.entry.path(), dbentry);
	if (error != IGameDatabase::Success) {
		logger->info("The database contains no game stored at {}", game.entry.path());
		libmgr.database.addGame(game);
	} else {
		// TODO: implement

	}*/
}