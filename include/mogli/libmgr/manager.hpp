#pragma once

#include <filesystem>
#include <iterator>
#include <string>

#include <mogli/logging.hpp>
#include "database.hpp"

namespace mogli::lib {
	using GameID = std::string;

	class LibraryManager final {
	private:
		mogli::log::LoggerPtr logger;
		IGameDatabase& database;

		// It would not really make sense to assign and copy the library manager
		LibraryManager(const LibraryManager& other) = delete;
		LibraryManager& operator=(LibraryManager& other) = delete;
	public:
		/**
		 * @brief Creates a new library manager that uses the given root directory as its home. That is scanning the
		 * filesystem for changes is performed using the root directory as starting point.
		 * 
		 * @param root 
		 */
		LibraryManager(const std::filesystem::path& root, IGameDatabase& database);
	};
}