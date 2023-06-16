#pragma once

#include <filesystem>
#include <iterator>
#include <string>

#include "database.hpp"
#include "../logging.hpp"

namespace mogli::lib {
	using GameID = std::string;

	/**
	 * @brief The library manager is responsible for synching the database and filesystem and provides and interface for
	 * enumerating, adding and modifying the game library.
	 * 
	 * The library manager is responsible for synching the database and filesystem and provides and interface for
	 * enumerating, adding and modifying the game library. To do this, it uses an abstract notion of a game database,
	 * IGameDatabase.
	 */
	class LibraryManager final {
	private:
		/**
		 * @brief The logger the library manager should write to.
		 */
		mogli::log::LoggerPtr logger;
		/**
		 * @brief The concrete game database to use.
		 */
		IGameDatabase& database;

		// It would not really make sense to assign and copy the library manager
		LibraryManager(const LibraryManager& other) = delete;
		LibraryManager& operator=(LibraryManager& other) = delete;

	public:
		/**
		 * @brief Creates a new library manager that uses the given root directory as its home. That is scanning
		 * the filesystem for changes is performed using the root directory as starting point.
		 * 
		 * @param root the root folder which stores the games' installers.
		 * @param database the database implementation to store games into.
		 */
		LibraryManager(const std::filesystem::path& root, IGameDatabase& database);
	};
} // namespace mogli::lib