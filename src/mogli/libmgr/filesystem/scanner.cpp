#include <mogli/libmgr/filesystem/scanner.hpp>

#include <mogli/libmgr/filesystem/gameentry.hpp>
#include <mogli/libmgr/manager.hpp>

using namespace mogli::lib;

Scanner::Scanner(LibraryManager& libraryManager) noexcept : libmgr(libmgr) {}

void Scanner::scan(std::filesystem::path path) const noexcept {
	for (auto const& entry : std::filesystem::directory_iterator(path)) {
		/** \todo: check for changes and only updated when changed */
		GameEntry game(entry);
		// libmgr.database.getGame(path);
	}
}