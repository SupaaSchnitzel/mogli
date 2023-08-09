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

static std::optional<ryml::Tree> readInfoYAML(dirent entry) noexcept {
	/** \todo this check may be redundant with the is_regular_file one. **/
	if (!entry.is_directory())
		return std::nullopt;
	auto yaml = dirent(entry.path() / "info.yaml");
	if (!yaml.is_regular_file())
		return std::nullopt;
	std::ifstream instream(yaml.path(), std::ios::in | std::ios::binary);
	std::vector<char> data(yaml.file_size());
	data.assign(std::istreambuf_iterator<char>(instream), std::istreambuf_iterator<char>());
	return std::make_optional(ryml::parse_in_place(ryml::to_substr(data)));
};

GameEntry::GameEntry(dirent entry) : logger(mogli::log::getLogger("GameEntry")), entry(entry), yaml(readInfoYAML(entry)) {}

std::optional<ryml::ConstNodeRef> GameEntry::getInfoEntryNode(std::vector<std::string> path) const noexcept {
	if (yaml.has_value()) {
		auto node = yaml.value().rootref();
		for (auto str : path)
			node = node[ryml::to_csubstr(str)];
		if (node.valid())
			return std::make_optional(node);
	}
	return std::nullopt;
}

template<>
std::optional<std::string> GameEntry::getInfoEntry(std::vector<std::string> path) const noexcept {
	/** \todo add error outputs **/
	auto node = getInfoEntryNode(path);
	if (node.has_value() && node.value().is_val())
		return std::make_optional(node.value().val().data());
	return std::nullopt;
}

template<>
std::optional<int> GameEntry::getInfoEntry(std::vector<std::string> path) const noexcept {
	/** \todo add error outputs **/
	auto node = getInfoEntryNode(path);
	int i;
	if (node.has_value() && node.value().is_val() && ryml::atoi(node.value().val(), &i))
		return std::make_optional(i);
	return std::nullopt;
}

template<>
std::optional<float> GameEntry::getInfoEntry(std::vector<std::string> path) const noexcept {
	/** \todo add error outputs **/
	auto node = getInfoEntryNode(path);
	float f;
	if (node.has_value() && node.value().is_val() && ryml::atof(node.value().val(), &f))
		return std::make_optional(f);
	return std::nullopt;
}

std::string GameEntry::getName() const noexcept {
	/** \todo add yaml support **/
	const std::string name = entry.path().stem();
	auto end = name.end();
	// Remove anything trailing that could be a date or hint.
	for (auto& regex : {dateRegex, anyHintRegex})
		if (std::smatch match; std::regex_search(name.begin(), end, match, regex))
			end = name.begin() + match.prefix().length();
	return std::string(name.begin(), end);
}

/** \todo stoi and the like my not be ideal since they throw exceptions. Instead a function of the signature
 *  bool myfunc(std::string str, int& out) would be best. **/

std::optional<int> GameEntry::getReleaseYear() const noexcept {
	std::string name = entry.path().stem();
	std::smatch match;
	if (std::regex_search(name, match, dateRegex)) {
		/** \todo check if conversion fails and log error **/
		return std::make_optional(std::stoi(match[1]));
	}
	return getInfoEntry<int>({"release"});
}

std::optional<std::string> GameEntry::getHint(std::string key) const noexcept {
	/** \todo add yaml support **/
	std::regex regex(std::format("\\[{}-(.*?)\\]", key));
	std::smatch match;
	std::string name = entry.path().stem();
	return std::regex_search(name, match, regex)? std::make_optional(match[1]) : std::nullopt;
}

std::vector<std::string> GameEntry::getTags() const noexcept {
	/** \todo implement **/
	return {};
}

std::vector<std::string> GameEntry::getDeveloper() const noexcept {
	if (yaml.has_value()) {
		/** \todo implement **/
	}
	return {};
}

std::vector<std::string> GameEntry::getPublisher() const noexcept {
	if (yaml.has_value()) {
		/** \todo implement **/
	}
	return {};
}

std::optional<float> GameEntry::getRating() const noexcept {
	return getInfoEntry<float>({"rating"});
}

static std::filesystem::directory_entry toDirEntry(std::string path) {
	return std::filesystem::directory_entry(path);
}

std::optional<std::filesystem::directory_entry> GameEntry::getLogo() const noexcept {
	/** \todo search for the default file **/
	return getInfoEntry<std::string>({"media", "logo"}).transform(toDirEntry);
}

std::optional<std::filesystem::directory_entry> GameEntry::getBanner() const noexcept {
	/** \todo search for the default file **/
	return getInfoEntry<std::string>({"media", "banner"}).transform(toDirEntry);
}

std::optional<std::filesystem::directory_entry> GameEntry::getBoxart() const noexcept {
	/** \todo search for the default file **/
	return getInfoEntry<std::string>({"media", "boxart"}).transform(toDirEntry);
}

std::vector<GameEntry::path_or_url> GameEntry::getScreenshots() const noexcept {
	/** \todo implement **/
	return {};
}

std::vector<GameEntry::path_or_url> GameEntry::getTrailers() const noexcept {
	/** \todo implement **/
	return {};
}

std::set<Platform> GameEntry::getSupportedPlatforms() const noexcept {
	/** \todo implement **/
	return {};
}

std::vector<std::filesystem::directory_entry> GameEntry::getInstallerFiles(Platform platform) const noexcept {
	/** \todo implement **/
	return {};
}
