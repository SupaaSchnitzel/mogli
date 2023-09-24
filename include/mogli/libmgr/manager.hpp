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
		LibraryManager& libmgr;

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
		LibMgrConfig config;		  /**< The config of the library manager. **/

		/**
		 * @brief Queries the gamedb specified in the config and returns the metadata for the given gameTitle.
		 *
		 * @param gameTitle Title of the game
		 * @return Returns a string formatted as json of the game Metadata
		 */
		std::string getGameMetadata(GameID gameTitle);

		/**
		 * @brief Adds a game into the mogli gamedb.
		 */
		int addGame(std::string gameInfo);

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

		/**
		 * Overrides the Metadata for a single game.
		 *
		 * @param url The url of the new game data (can only be steam or gog for now).
		 * @param game The game for which the data will be changed.
		 *
		 * @return Returns a positive integer if Metadata was succesfully fetched and set.
		 */
		int setGameMetadata(std::string url, GameID game);
	};
} // namespace mogli::lib