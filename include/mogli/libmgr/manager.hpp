#pragma once

#include <filesystem>
#include <iterator>
#include <string>

#include <map>

#include "../logging.hpp"
#include "../utils/iterable.hpp"
#include "config.hpp"
#include "database.hpp"
#include "filesystem/scanner.hpp"
#include "game.hpp"

namespace mogli::lib {
	class LibraryManager;

	/**
	 * @brief Abstraction layer around the game database for a more readable API.
	 * @details The Games class wraps around the IGameDatabase implementation in the LibraryManager to provide an
	 * intuitive interface for retrieving game information and iterating over games. This class should not be manually
	 * instantiated. Instead use LibraryManager::games.
	 */
	struct Games final {
		friend LibraryManager;

	private:
		const LibraryManager& libmgr;

		Games(LibraryManager& libmgr) noexcept;

	public:
		Game operator[](GameID id) const noexcept;

		mogli::utils::Iterable<Game> all() const noexcept;
	};

	/**
	 * @brief The library manager is responsible for synching the database and filesystem and provides and interface for
	 * enumerating, adding and modifying the game library.
	 *
	 * The library manager is responsible for synching the database and filesystem and provides and interface for
	 * enumerating, adding and modifying the game library. To do this, it uses an abstract notion of a game database,
	 * IGameDatabase.
	 */
	class LibraryManager final {
		friend Games;
		friend Scanner;

	private:
		mogli::log::LoggerPtr logger; /**< The logger the library manager should write to. **/
		IGameDatabase& database;	  /**< The concrete mogli game database to use. **/
		const LibMgrConfig config;	  /**< The config of the library manager. **/
		const Scanner scanner;

		// It would not really make sense to assign and copy the library manager
		LibraryManager(const LibraryManager& other) = delete;
		LibraryManager& operator=(LibraryManager& other) = delete;

	public:
		/**
		 * @brief Abstracts the gamedatabase API to be more readable. Use this to retrieve information from the
		 * database. Note that changes in the filesystem will only be applied to the database after scanning.
		 */
		Games games;

		/**
		 * @brief Creates a new library manager that uses the given root directory as its home. That is scanning
		 * the filesystem for changes. This is performed using the root directory as starting point.
		 *
		 * @param config the Configuration that should be used for anything regarding the library manager.
		 * @param database the database implementation to store games into.
		 */
		LibraryManager(LibMgrConfig config, IGameDatabase& database);

		void scanAll() const noexcept;
	};
} // namespace mogli::lib