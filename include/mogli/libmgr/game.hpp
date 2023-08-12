#pragma once

#include <optional>
#include <string>
#include <vector>

namespace mogli::lib {
	using GameID = std::string;

	struct Game final {
		GameID id;
		std::optional<std::string> title;
		std::optional<std::string> description;
		std::vector<std::string> tags;
	};
};