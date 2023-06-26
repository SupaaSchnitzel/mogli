#pragma once

#include <filesystem>
#include <iterator>
#include <string>

#include "../logging.hpp"
#include "config.hpp"
#include "database.hpp"
namespace mogli::lib {
	using GameID = std::string;
	using gameDatabases = mogli::lib::gameDatabases;
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
		 * @brief The concrete mogli game database to use.
		 */
		IGameDatabase& database;

		// TODO: Vlt. ne init damit man eventuell die gogdb datenbank herunterladen kann.
		/**
		 * @brief The config of the library manager.
		 */
		libConfig config;

		/**
		 * @brief Queries the gamedb specified in the config and returns the metadata for the given gameTitle.
		 *
		 * @param gameTitle Title of the game
		 * @return Returns a string formatted as json of the game Metadata
		 */
		std::string getGameMetadata(GameID gameTitle);

		/**
		 * @brief Queries the gogdb gamedb and returns the metadata for the given gameTitle.
		 *
		 * @param gameTitle Title of the game
		 * @return Returns a string formatted as json of the game Metadata
		 */
		std::string getGameMetadataGogdb(GameID gameTitle);

		/**
		 * @brief Queries the Steam gamedb and returns the metadata for the given gameTitle.
		 *
		 * @param gameTitle Title of the game
		 * @return Returns a string formatted as json of the game Metadata
		 */
		std::string getGameMetadataSteam(GameID gameTitle);

		/**
		 * @brief Queries the gog gamedb and returns the metadata for the given gameTitle.
		 *
		 * @param gameTitle Title of the game
		 * @return Returns a string formatted as json of the game Metadata
		 */
		std::string getGameMetadataGog(GameID gameTitle);

		/**
		 * @brief Adds a game into the mogli gamedb.
		 */
		int addGame(std::string gameInfo);

		/**
		 * @brief This scans the whole filesystem starting from the root folder. Checks all found games for metadata and
		 * adds them to the Databases.
		 *
		 * @return Returns a positive integer if scan was sucessfully started.
		 */
		int scanThread();

		/**
		 * @brief This scans the whole filesystem starting from the root folder. Checks newly found games for metadata
		 * and adds them to the Databases.
		 *
		 * @return Returns a positive integer if scan was sucessfully finished.
		 */
		int rescanThread();

		// It would not really make sense to assign and copy the library manager
		LibraryManager(const LibraryManager& other) = delete;
		LibraryManager& operator=(LibraryManager& other) = delete;

	public:
		/**
		 * @brief Creates a new library manager that uses the given root directory as its home. That is scanning
		 * the filesystem for changes. This is performed using the root directory as starting point.
		 *
		 * @param config the Configuration that should be used for anything regarding the library manager.
		 * @param database the database implementation to store games into.
		 */
		LibraryManager(libConfig config, IGameDatabase& database);

		/**
		 * @brief This scans the whole filesystem starting from the root folder. Checks all found games for metadata and
		 * adds them to the Databases. This function is threaded.
		 *
		 * @return Returns a positive integer if scan was sucessfully started.
		 */
		int scan();

		/**
		 * @brief This scans the whole filesystem starting from the root folder. Checks newly found games for metadata
		 * and adds them to the Databases. This function is threaded.
		 *
		 * @return Returns a positive integer if scan was sucessfully finished.
		 */
		int rescan();

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