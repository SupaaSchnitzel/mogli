#include <mogli/libmgr/manager.hpp>

using namespace mogli::lib;

LibraryManager::LibraryManager(const std::filesystem::path& root, IGameDatabase& database)
		: logger(mogli::log::getLogger("Library")), database(database) {}