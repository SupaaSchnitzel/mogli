#pragma once

#include <filesystem>
#include <string>


#include "../igameinfoprovider.hpp"

namespace mogli::lib {
	using GameID = std::string;
	using gameDatabases = mogli::lib::gameDatabases;
	/**
	 * @brief The local provider is used to read the locally provided game information and serve it through the



	 * * public functions

	 */

	class LocalProvider : public IGameInfoProvider {
	public:
		
		/**
		 * @brief Queries the Local Yaml file and returns the metadata for the given search string.

		 * * 

		 * *
		 * @param search The search string, the name of the local gamefolder wherein the yaml
		 * file is
		 *
		 * located is expected(See Doxygen for more Info).
         * 
		 * @return Returns a string formatted as json
		 * of the game Metadata
		 */

		std::optional<std::string> tryFindGame(std::string search) override;
		/**
		 * @brief Queries local game which responds to the id in the postgresdb to get the  and returns the


		 * * metadata for the given GameID.

		 * *
		 *
		 * @param id ID of the game
		 * @return Returns a string
		 * formatted as json of the game Metadata

		 */
		std::optional<std::string> getGameByID(GameID id) override;

	
	};
} // namespace mogli::lib