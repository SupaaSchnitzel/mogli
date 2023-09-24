#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace mogli::lib {
	/**
	 * @brief Contains the library manager configuration options.
	 */
	struct LibMgrConfig {
		std::filesystem::path root; /**< The filepath to the root folder of the game library. **/
	};
} // namespace mogli::lib