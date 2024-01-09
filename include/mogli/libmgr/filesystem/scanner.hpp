#pragma once

#include <filesystem>

namespace mogli::lib {

	class LibraryManager;

	class Scanner final {
		friend LibraryManager;

	private:
		LibraryManager& libmgr;

		Scanner(LibraryManager& libraryManager) noexcept;

	public:
		void scan(std::filesystem::path path) const noexcept;
	};

} // namespace mogli::lib