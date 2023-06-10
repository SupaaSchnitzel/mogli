#pragma once

#include <inttypes.h>
#include <string>

namespace mogli::rest {
	struct RESTConfig {
		std::string host;
		uint16_t port;
		bool useIPv4;
	};
}