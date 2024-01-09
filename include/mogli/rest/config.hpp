#pragma once

#include <inttypes.h>
#include <string>

namespace mogli::rest {
	/**
	 * @brief Contains configuration options for the REST-endpoint.
	 */
	struct RESTConfig {
		std::string host; /**< The hostname or ip-address to bind to. **/
		uint16_t port;	  /**< The port the REST-endpoint should be listening to. **/
		/**
		 * @brief True if the REST-endpoint should listen on the IP v4 adress, false if IP v6 should be used.
		 */
		bool useIPv4;
	};
} // namespace mogli::rest