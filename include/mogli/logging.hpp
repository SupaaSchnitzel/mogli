#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace mogli::log {
	using LoggerPtr = std::shared_ptr<spdlog::logger>;

	/**
	 * @brief Creates a new named logger instance with the given name or fetches an existing one associated with
	 * the name.
	 * 
	 * @param name The name of the the logger to be returned.
	 * @return A logger with the specified name.
	 */
	LoggerPtr getLogger(std::string name);
} // namespace mogli::log