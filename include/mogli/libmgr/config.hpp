#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace mogli::lib {
	/**
	 * @brief Contains the library manager configuration options.
	 */
	struct LibMgrConfig {
		/**
		 * @brief The filepath to the root folder of the game library.
		 */
		const std::filesystem::path root;
	};
} // namespace mogli::lib