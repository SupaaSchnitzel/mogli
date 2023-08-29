#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace mogli::lib {
	using GameID = int;

	struct Game final {
		GameID id;
		std::filesystem::path path;
		std::string title;
		std::optional<std::string> description;
		std::vector<std::string> tags;
		std::map<std::string, std::filesystem::path> media;
	};
};