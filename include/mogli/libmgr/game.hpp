#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace mogli::lib {
	using GameID = int;

	constexpr GameID InvalidGameID = -1;

	/**
	 * @brief The Game datastructure represents all information regarding a game.
	 * @details It should not be confused with mogli::lib::GameDBEntry which only represents a row in mogli's games
	 * table in the database (e.g., media is not part of this structure) and mogli::lib::GameEntry which abstracts the
	 * information stored in the game's folder within the file system.
	 * 
	 * @see mogli::lib::GameDBEntry
	 * @see mogli::lib::GameEntry
	 */
	struct Game final {
		GameID id = InvalidGameID;
		std::filesystem::path path;
		std::string title;
		std::optional<std::string> description;
		std::vector<std::string> tags;
		std::map<std::string, std::filesystem::path> media;

		static constexpr std::string MediaKeyLogo = "logo";
		static constexpr std::string MediaKeyBanner = "banner";
		static constexpr std::string MediaKeyBoxart = "boxart";
	};
}; // namespace mogli::lib