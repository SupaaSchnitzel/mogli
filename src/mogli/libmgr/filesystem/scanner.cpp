#include <mogli/libmgr/filesystem/scanner.hpp>

#include <mogli/libmgr/filesystem/gameentry.hpp>

using namespace mogli::lib;

void Scanner::scan(std::filesystem::path path) {
	for (auto const& entry : std::filesystem::directory_iterator(path)) {
		GameEntry game(entry);
	}
}