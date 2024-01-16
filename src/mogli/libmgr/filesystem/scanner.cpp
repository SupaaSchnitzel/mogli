#include <mogli/libmgr/filesystem/scanner.hpp>

#include <mogli/libmgr/filesystem/gameentry.hpp>
#include <mogli/libmgr/manager.hpp>

#include <spdlog/fmt/bundled/format.h>

using namespace mogli::lib;

static Game toGame(const GameEntry& fsentry) {
	std::map<std::string, std::filesystem::path> media;
	auto addToMedia = [&media](std::string name) { return [&media, name](auto path) { return media[name] = path; }; };
	fsentry.getLogo().transform(addToMedia(Game::MediaKeyLogo));
	fsentry.getBanner().transform(addToMedia(Game::MediaKeyBanner));
	fsentry.getBoxart().transform(addToMedia(Game::MediaKeyBoxart));
	return Game{
		.id = InvalidGameID,
		.path = fsentry.getPath(),
		.title = fsentry.getName(),
		.description = fsentry.getDescription(),
		.tags = fsentry.getTags(),
		.media = media
	};
}

Scanner::Scanner(LibraryManager& libmgr) noexcept : logger(mogli::log::getLogger("Scanner")), libmgr(libmgr) {}

void Scanner::merge() const noexcept {

}

void Scanner::scan(std::filesystem::path path, bool dryrun) const noexcept {
	logger->info("Scanning {}...", path);
	if (dryrun)
		logger->info("A dryrun is performed, i.e., no data will be modified");
	if (!std::filesystem::is_directory(path)) {
		logger->warn("{} is not a directory or does not exist. Aborting scan", path);
		return;
	}
	unsigned newlyAdded = 0;
	unsigned updated = 0;
	for (auto const& entry : std::filesystem::directory_iterator(path)) {
		logger->trace("{}", entry.path());
		GameDBEntry dbentry;
		auto error = libmgr.database.getGame(entry.path(), dbentry);
		if (error == IGameDatabase::Success) {
			logger->trace("Found: [id: {}, path: {}]", dbentry.id, dbentry.path);
			/** \todo implement **/
		} else {
			logger->info("The database contains no game stored at {}", entry.path());
			GameEntry fsentry(entry);
			auto game = toGame(fsentry);
			logger->info("Adding game:");
			logger->info("\tPath:        {}", game.path);
			logger->info("\tTitle:       {}", game.title);
			logger->info("\tDescription: {}", game.description);
			logger->info("\tTags:        [{}]", fmt::to_string(fmt::join(game.tags, ",")));
			if (!dryrun) {
				auto code = libmgr.database.addGame(game);
				if (code != IGameDatabase::Success) {
					logger->error("Failed to add game: [{}] {}", code, libmgr.database.getErrorMessage(code));
				}
			}
			newlyAdded++;
		}
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