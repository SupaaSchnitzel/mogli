#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace mogli::lib {

	/**
	* @brief Contains the possible game database.
	*/
	enum gameDatabases { steam, gog, gogdb };

	/**
     * @brief Contains the library manager configuration options.
     */
	struct libConfig {
		/**
		 * @brief The filepath to the root folder of the game library.
		 */
		const std::filesystem::path& root;
		/**
        * @brief Contains the preferred gamedatabase.
        */
		gameDatabases gamedb;
		/**
         * @brief Contains the steam api token if steam is the preferred db.
        */
		std::optional<std::string> steamToken;
		/**
         * @brief Contains the gog api token if steam is the preferred db.
         * See:
		 *
		 * https://support.gog.com/hc/en-us/articles/4405004689297-How-to-join-the-GOG-Affiliate-Program?product=gog

		 * * last bullet point
        */
		std::optional<std::string> gogToken;
		/**
         * @brief Contains a boolean to determine if mogli downloads the gogdb database fully.
        */
		std::optional<bool> gogdbDownload;
	};
} // namespace mogli::lib