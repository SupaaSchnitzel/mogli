#include <mogli/libmgr/filesystem/gameentry.hpp>

#include <catch2/catch_test_macros.hpp>

using GameEntry = mogli::lib::GameEntry;
using direntry = std::filesystem::directory_entry;
using path = std::filesystem::path;

TEST_CASE("No info.yml", "[gameentry]") {
	{
        GameEntry entry(direntry("media/media1/Game 1"));
        REQUIRE(entry.getName() == "Game 1");
        REQUIRE_FALSE(entry.getReleaseYear().has_value());
        REQUIRE_FALSE(entry.getBanner().has_value());
        REQUIRE_FALSE(entry.getBoxart().has_value());
        REQUIRE_FALSE(entry.getLogo().has_value());
        REQUIRE(entry.getPublisher().empty());
        REQUIRE(entry.getDeveloper().empty());
        REQUIRE(entry.getInstallerFiles(mogli::lib::Platform::Linux).empty());
        REQUIRE(entry.getInstallerFiles(mogli::lib::Platform::Mac).empty());
        REQUIRE(entry.getInstallerFiles(mogli::lib::Platform::Windows).empty());
        REQUIRE_FALSE(entry.getRating().has_value());
        REQUIRE(entry.getScreenshots().empty());
        REQUIRE(entry.getSupportedPlatforms().empty());
        REQUIRE(entry.getTags().empty());
        REQUIRE(entry.getTrailers().empty());
    }
}
