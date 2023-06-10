#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace mogli::log {
	using LoggerPtr = std::shared_ptr<spdlog::logger>;
	LoggerPtr getLogger(std::string name);
}