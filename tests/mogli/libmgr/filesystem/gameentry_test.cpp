#include <mogli/libmgr/filesystem/gameentry.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include <ranges>

using GameEntry = mogli::lib::GameEntry;
using std::filesystem::path;
using direntry = std::filesystem::directory_entry;
using std::filesystem::relative;
using std::views::transform;

using Catch::Matchers::UnorderedRangeEquals;

TEST_CASE("No info.yml", "[gameentry]") {
	{
        GameEntry entry(direntry("media/media1/Game 1"));
        CHECK(entry.getName() == "Game 1");
        CHECK_FALSE(entry.getReleaseYear().has_value());
        CHECK_FALSE(entry.getBanner().has_value());
        CHECK_FALSE(entry.getBoxart().has_value());
        CHECK_FALSE(entry.getLogo().has_value());
        CHECK(entry.getPublisher().empty());
        CHECK(entry.getDeveloper().empty());
        CHECK(entry.getInstallerFiles(mogli::lib::Platform::Linux).empty());
        CHECK(entry.getInstallerFiles(mogli::lib::Platform::Mac).empty());
        CHECK(entry.getInstallerFiles(mogli::lib::Platform::Windows).empty());
        CHECK_FALSE(entry.getRating().has_value());
        CHECK(entry.getScreenshots().empty());
        CHECK(entry.getSupportedPlatforms().empty());
        CHECK(entry.getTags().empty());
        CHECK(entry.getTrailers().empty());
    }
    {
        path entrypath = "media/media1/Game 2 [hint1-value] [hint2-value-with-dash] [hint with space-value with space] (2003)";
        auto to_relative = [entrypath](const path& p) { return relative(p, entrypath); };
        auto to_relative_pou = [to_relative](const GameEntry::path_or_url& p) -> GameEntry::path_or_url {
            return std::holds_alternative<path>(p)? to_relative(std::get<path>(p)) : p;
        };
        GameEntry entry(direntry{entrypath});
        CHECK(entry.getName() == "Game 2");
        REQUIRE(entry.getReleaseYear().has_value());
        CHECK(entry.getReleaseYear().value() == 2003);
        CHECK(entry.getHint("hint1") == "value");
        CHECK(entry.getHint("hint2") == "value-with-dash");
        CHECK_FALSE(entry.getHint("hint3").has_value());
        CHECK(entry.getHint("hint with space") == "value with space");
        REQUIRE(entry.getBanner().has_value());
        CHECK(to_relative(entry.getBanner().value().path()) == "banner.png");
        REQUIRE(entry.getBoxart().has_value());
        CHECK(to_relative(entry.getBoxart().value().path()) == "boxart.png");
        REQUIRE(entry.getLogo().has_value());
        CHECK(to_relative(entry.getLogo().value().path()) == "logo.png");
        CHECK(entry.getPublisher().empty());
        CHECK(entry.getDeveloper().empty());
        CHECK(entry.getInstallerFiles(mogli::lib::Platform::Linux).empty());
        CHECK(entry.getInstallerFiles(mogli::lib::Platform::Mac).empty());
        CHECK(entry.getInstallerFiles(mogli::lib::Platform::Windows).empty());
        CHECK_FALSE(entry.getRating().has_value());
        std::vector<GameEntry::path_or_url> expected = {path("screenshot1.png"), path("screenshot2.jpg")};
        CHECK_THAT(entry.getScreenshots() | std::views::transform(to_relative_pou), UnorderedRangeEquals(expected));
        CHECK(entry.getSupportedPlatforms().empty());
        CHECK(entry.getTags().empty());
        CHECK(entry.getTrailers().empty());
    }
}
