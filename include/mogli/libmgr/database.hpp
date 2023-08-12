#pragma once

#include "game.hpp"

#include <optional>

namespace mogli::lib {
	/**
	 * @brief The generic game database interface.
	 * 
	 * Represents a generic game database interface. A game database must provide functionality for registering new
	 * games, listing and filtering known games, and do the same for players.
	 */
	class IGameDatabase {
		/**
		 * @brief The type for all error codes.
		 */
		using ErrorCode = unsigned;

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
		virtual ErrorCode setup() noexcept = delete;

		/**
		 * @brief Tears down the database. This code should handle any deinitialization that is too complex for the
		 * destructor.
		 * 
		 * @return The error code on failure, 0 on success.
		 */
		virtual ErrorCode teardown() noexcept = delete;

		/**
		 * @brief Returns a textual description of the error code.
		 * 
		 * @param error The error code to retrieve the description for.
		 * @return The description of the provided error code.
		 */
		virtual const char* getErrorMessage(ErrorCode error) = delete;
	};

	/**
	 * @brief Provides the implementation of IGameDatabase for a PostgreSQL backend.
	 */
	class PostgreGameDatabase final : public IGameDatabase {};
} // namespace mogli::lib