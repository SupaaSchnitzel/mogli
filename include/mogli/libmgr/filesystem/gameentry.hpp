#pragma once

#include "../platforms.hpp"

#include <ryml.hpp>

#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "../../logging.hpp"

namespace mogli::lib {

	/**
	 * @brief The GameEntry class abstracts a folder containing user provided information on a game.
	 * @details The GameEntry class abstracts a folder containing user provided information on a game. The specification
	 * of the folder structure can be found \ref md_media_filesystem "here".
	 */
	class GameEntry final {
		using path_or_url = std::variant<std::filesystem::path, std::string>;
	private:
		mogli::log::LoggerPtr logger; /**< The logger the library manager should write to. **/
		std::filesystem::directory_entry entry; /**< The directory that holds this game's informations. **/
		std::optional<ryml::Tree> yaml; /**< The parsed yaml file contents. **/

		/**
		 * @brief Retrieves the yaml tree node associated with the provided path. If the path does not exist, empty is
		 * returned.
		 * 
		 * @param path 
		 * @return std::optional<ryml::NodeRef> 
		 */
		std::optional<ryml::ConstNodeRef> getInfoEntryNode(std::vector<std::string> path) const noexcept;
		/**
		 * @brief Retrieves a key from the info.yml file describing metadata of this game entry.
		 * 
		 * @tparam T 
		 * @param path 
		 * @return T 
		 */
		template<typename T>
		std::optional<T> getInfoEntry(std::vector<std::string> path) const noexcept;
	public:
		/**
		 * @brief Construct a new GameEntry object that is associated with the provided directory.
		 * 
		 * @param entry The directory entry containing information regarding the game.
		 */
		explicit GameEntry(std::filesystem::directory_entry entry);

		/**
		 * @brief Returns the name of the game as it is inferred from the file name.
		 * @details Returns the name of the game as it is inferred from the file name. The game's name is the initial
		 * substring that does not consist of the release year or any hints.
		 * For example, executing `getName()` on a file named `myfilename [myhint-the value] (2009)` will return
		 * `myfilename`.
		 * 
		 * @return The name of the game.
		 */
		std::string getName() const noexcept;

		/**
		 * @brief Returns the year of the game's release as it is inferred from the file name.
		 * @details Returns the year of the game's release as it is inferred from the file name. The year of the game's
		 * release is inferred by a substring satisfying the regex `\(\d{4}\)`.
		 * For example, executing `getReleaseYear()` on a file named `myfilename [myhint-the value] (2009)` will return
		 * `2009`.
		 * 
		 * @return The year of the game's release or an empty optional if it could not be inferred.
		 */
		std::optional<int> getReleaseYear() const noexcept;

		/**
		 * @brief Extracts the specified hint from the filename and returns it. If the hint was not present, an empty
		 * optional is returned instead.
		 * @details Extracts the specified hint from the filename and returns it. If the hint was not present, an empty
		 * optional is returned instead. A hint is a character-sequence of the form `[key-value]` within the filename.
		 * For example, executing `getHint("myhint")` on a file named `myfilename [myhint-the value]` will return `the
		 * value`.
		 * 
		 * @param key The key of the hint to extract.
		 * @return Empty, if the key was not found. The value of the hint otherwise.
		 */
		std::optional<std::string> getHint(std::string key) const noexcept;

		/**
		 * @brief Returns the tags (keywords) associated with the game. These are read from the yaml file.
		 * 
		 * @return The tags (keywords) associated with the game.
		 */
		std::vector<std::string> getTags() const noexcept;

		/**
		 * @brief Returns the developer of the game as it is specified in the yaml file.
		 * 
		 * @return The developer of the game if it is specified and an empty optional otherwise.
		 */
		std::vector<std::string> getDeveloper() const noexcept;

		/**
		 * @brief Returns the publisher of the game as it is specified in the yaml file.
		 * 
		 * @return The publisher of the game if it is specified and an empty optional otherwise.
		 */
		std::vector<std::string> getPublisher() const noexcept;

		/**
		 * @brief The highest possible rating.
		 */
		constexpr static float maxRating = 5.f;

		/**
		 * @brief Returns the rating of the game as it is specified in the yaml file.
		 * @details Returns the rating for the game as it is specified in the yaml file. The rating can be any number
		 * in the range from 0 (worst) to @ref maxRating (best).
		 * 
		 * @return The rating of the game if it is specified and an empty optional otherwise.
		 * @see maxRating
		 */
		std::optional<float> getRating() const noexcept;

		/**
		 * @brief Returns the path to the logo if it exists.
		 * 
		 * @return The path to the logo or empty if it was not found.
		 */
		std::optional<std::filesystem::directory_entry> getLogo() const noexcept;

		/**
		 * @brief Returns the path to the banner if it exists.
		 * 
		 * @return The path to the banner or empty if it was not found.
		 */
		std::optional<std::filesystem::directory_entry> getBanner() const noexcept;

		/**
		 * @brief Returns the path to the boxart if it exists.
		 * 
		 * @return The path to the boxart or empty if it was not found.
		 */
		std::optional<std::filesystem::directory_entry> getBoxart() const noexcept;

		/**
		 * @brief Returns the paths or URLs of all the screenshots that were found for the game.
		 * 
		 * @return the paths or URLs of all the screenshots that were found.
		 */
		std::vector<path_or_url> getScreenshots() const noexcept;

		/**
		 * @brief Returns the paths or URLs of all the trailers that were found for the game.
		 * 
		 * @return the paths or URLs of all the trailers that were found.
		 */
		std::vector<path_or_url> getTrailers() const noexcept;

		/**
		 * @brief Returns the platforms for which installers were found.
		 * 
		 * @return the platforms for which installers were found.
		 */
		std::set<Platform> getSupportedPlatforms() const noexcept;

		/**
		 * @brief Returns the installer files for the specified platform. If none were found, an empty vector is
		 * returned.
		 * 
		 * @param platform The platform for which to return the installer files.
		 * @return the installer files for the specified platform.
		 */
		std::vector<std::filesystem::directory_entry> getInstallerFiles(Platform platform) const noexcept;
	};
} // namespace mogli::lib