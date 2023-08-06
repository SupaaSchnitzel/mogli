#pragma once

#include <optional>
#include <string>

#include "../logging.hpp"
#include "./config.hpp"

namespace mogli::lib {
	using GameID = std::string;
	/**
	 * @brief The GameInfoProvider interfaces provides the general functions implemented by each
	 *
	 * GameInfoProvider.
	 */
	class IGameInfoProvider {
	public:
		virtual void init() = delete;
		virtual void deinit() = delete;
		/**
		 * @brief Queries the External Game Database and returns the metadata for the given search string.

		 * *
		 *
		 * @param search The search string to look for the game in the external database
		 * @return
		 * Returns a string formatted as json of the game Metadata
		 */
		virtual std::optional<std::string> tryFindGame(std::string search);
		/**
		 * @brief Queries the External Game Database to get the  and returns the metadata for the given GameID.

		 * *
		 *
		 * @param id ID of the game
		 * @return Returns a string formatted as json of the game Metadata

		 */
		virtual std::optional<std::string> getGameByID(GameID id);
	};
} // namespace mogli::lib