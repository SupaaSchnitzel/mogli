#include <mogli/libmgr/filesystem/scanner.hpp>

using namespace mogli::lib;

void Scanner::scan(std::filesystem::path path) {
	for (auto const& entry : std::filesystem::directory_iterator(path)) {
		//if (hasChanges(entry)) {
			/** \todo implement **/
		//}
	}
}