#pragma once

#include "game.hpp"
#include "filesystem/gameentry.hpp"
#include "../utils/iterable.hpp"

#include <inttypes.h>
#include <optional>
#include <memory>
#include <variant>

namespace mogli::lib {

	struct DBConfig {
		std::string host;
		uint16_t port;
		std::string username;
		std::string password;
		std::string dbname;
	};

	/**
	 * @brief The generic game database interface.
	 * 
	 * Represents a generic game database interface. A game database must provide functionality for registering new
	 * games, listing and filtering known games, and do the same for players.
	 */
	class IGameDatabase {
	public:
		/**
		 * @brief The type for all error codes.
		 */
		using ErrorCode = unsigned;

		virtual ~IGameDatabase() = default;

		/**
		 * @brief The error code indicating no error. This must also be 0 for all concrete implementations of this
		 * class.
		 */
		static constexpr ErrorCode Success = 0;
		
		/**
		 * @brief Performs any required setup that is too complex for the constructor.
		 * 
		 * @return The error code on failure, 0 on success.
		 */
		virtual ErrorCode setup(DBConfig config) noexcept = 0;

		/**
		 * @brief Tears down the database. This code should handle any deinitialization that is too complex for the
		 * destructor.
		 * 
		 * @return The error code on failure, 0 on success.
		 */
		virtual ErrorCode teardown() noexcept = 0;

		/**
		 * @brief Returns a textual description of the error code.
		 * 
		 * @param error The error code to retrieve the description for.
		 * @return The description of the provided error code.
		 */
		virtual const char* getErrorMessage(ErrorCode error) noexcept = 0;

		virtual ErrorCode addGame(mogli::lib::GameEntry entry) noexcept = 0;

		virtual std::variant</*std::shared_ptr<*/mogli::utils::Iterable<Game>/*>*/, ErrorCode> games() noexcept = 0;
	};

	std::unique_ptr<IGameDatabase> createPostgreSQLConnector();
} // namespace mogli::lib