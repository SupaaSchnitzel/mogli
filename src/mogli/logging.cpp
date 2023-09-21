#include <mogli/logging.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

using LoggerPtr = mogli::log::LoggerPtr;

void mogli::log::setVerbosity(mogli::log::Verbosity verbosity) noexcept {
	auto level = spdlog::level::off - static_cast<int>(verbosity);
	spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
}

LoggerPtr mogli::log::getLogger(std::string name) {
	auto logger = spdlog::get(name);
	if (logger == nullptr) {
		logger = spdlog::stdout_color_mt(name);
	}
	return logger;
}