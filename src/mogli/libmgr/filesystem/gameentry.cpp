#include <mogli/libmgr/filesystem/gameentry.hpp>

#include <ryml_std.hpp>

#include <format>
#include <fstream>
#include <regex>

using namespace mogli::lib;
using dirent = std::filesystem::directory_entry;
using path = std::filesystem::path;

static std::regex dateRegex("\\((\\d{4})\\)");
static std::regex anyHintRegex("\\[(.*?)-(.*?)\\]");

static std::optional<ryml::Tree> readConfigYAML(dirent entry) noexcept {
	/** \todo this check may be redundant with the is_regular_file one. **/
	if (!entry.is_directory())
		return std::nullopt;
	auto yaml = dirent(entry.path() / "config.yaml");
	if (!yaml.is_regular_file())
		return std::nullopt;
	std::ifstream instream(yaml.path(), std::ios::in | std::ios::binary);
	std::vector<char> data(yaml.file_size());
	data.assign(std::istreambuf_iterator<char>(instream), std::istreambuf_iterator<char>());
	return std::make_optional(ryml::parse_in_place(ryml::to_substr(data)));
};

GameEntry::GameEntry(dirent entry) : entry(entry), yaml(readConfigYAML(entry)) {}

std::string GameEntry::getName() const noexcept {
	const std::string name = entry.path().stem();
	auto end = name.end();
	for (auto& regex : {dateRegex, anyHintRegex})
		if (std::smatch match; std::regex_search(name.begin(), end, match, regex))
			end = name.begin() + match.prefix().length();
	return std::string(name.begin(), end);
}

std::optional<int> GameEntry::getReleaseYear() const noexcept {
	std::string name = entry.path().stem();
	std::smatch match;
	return std::regex_search(name, match, dateRegex)? std::make_optional(std::stoi(match[1])) : std::nullopt;
}

std::optional<std::string> GameEntry::getHint(std::string key) const noexcept {
	std::regex regex(std::format("\\[{}-(.*?)\\]", key));
	std::smatch match;
	std::string name = entry.path().stem();
	return std::regex_search(name, match, regex)? std::make_optional(match[1]) : std::nullopt;
}
