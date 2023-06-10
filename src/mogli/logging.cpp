#include <mogli/logging.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

using LoggerPtr = mogli::log::LoggerPtr;

LoggerPtr mogli::log::getLogger(std::string name) {
	auto logger = spdlog::get(name);
	if (logger == nullptr) {
		logger = spdlog::stdout_color_mt(name);
	}
	return logger;
}