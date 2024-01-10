#pragma once

#include "../../logging.hpp"

#include <filesystem>

namespace mogli::lib {

	class GameEntry;
	class LibraryManager;

	struct ScannerConfig final {
		bool dryrun;
	};

	class Scanner final {
		friend LibraryManager;

	private:
		mogli::log::LoggerPtr logger; /**< The logger the scanner should write to. **/
		LibraryManager& libmgr;

		Scanner(LibraryManager& libraryManager) noexcept;

		void merge() const noexcept;
	public:
		void scan(std::filesystem::path path) const noexcept;
		void scanSingle(const GameEntry& game) const noexcept;
	};

} // namespace mogli::lib