#include <mogli/libmgr/filesystem/gameentry.hpp>

#include <ryml_std.hpp>
/** \todo maybe remove at some point to be more logger-agnostic **/
#include <spdlog/fmt/bundled/ranges.h>

#include <format>
#include <fstream>
#include <ranges>
#include <regex>

using namespace mogli::lib;
using dirent = std::filesystem::directory_entry;
using std::filesystem::directory_iterator;
using std::filesystem::path;
using std::ranges::find_if;
using std::views::transform;

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
	return std::make_optional(ryml::parse_in_arena(ryml::to_csubstr(std::move(data))));
};

static std::optional<dirent> getMemberFile(dirent dir, std::string name) noexcept {
	if (!dir.is_directory())
		return std::nullopt;
	for (auto&& entry : directory_iterator(dir))
		if (entry.is_regular_file() && entry.path().stem() == name)
			return std::make_optional(entry);
	return std::nullopt;
}

inline static bool isPicture(std::filesystem::path path) {
	static const char* picExtensions[] = {".png", ".jpg", ".jpeg", ".webp", ".tiff"};
	return std::find(std::begin(picExtensions), std::end(picExtensions), path.extension()) != std::end(picExtensions);
}

GameEntry::GameEntry(dirent entry)
		: logger(mogli::log::getLogger("GameEntry")), entry(entry), yaml(readInfoYAML(entry)) {}

std::optional<ryml::ConstNodeRef> GameEntry::getInfoEntryNode(std::vector<std::string> path) const noexcept {
	if (yaml.has_value()) {
		auto node = yaml.value().crootref();
		for (auto str : path) {
			auto rstr = ryml::to_csubstr(str);
			if (node.has_child(rstr))
				node = node[rstr];
			else {
				logger->info("No entry found in info.yaml for path [{}]", fmt::join(path, " > "));
				return std::nullopt;
			}
		}
		return std::make_optional(node);
	}
	return std::nullopt;
}

template <> std::optional<std::string> GameEntry::getNodeValue(const ryml::ConstNodeRef& node) const noexcept {
	if (!node.has_val()) {
		std::vector<std::string> path;
		for (auto n = node; n.has_parent(); n = n.parent())
			path.push_back(n.key().data());
		logger->error("[{}] was found in info.yaml but has no value", fmt::join(path.rbegin(), path.rend(), " > "));
		return std::nullopt;
	}
	return std::make_optional(std::string(node.val().begin(), node.val().end()));
}

template <> std::optional<int> GameEntry::getNodeValue(const ryml::ConstNodeRef& node) const noexcept {
	if (!node.has_val()) {
		std::vector<std::string> path;
		for (auto n = node; n.has_parent(); n = n.parent())
			path.push_back(n.key().data());
		logger->error("[{}] was found in info.yaml but has no value", fmt::join(path.rbegin(), path.rend(), " > "));
		return std::nullopt;
	}
	int i;
	if (!ryml::atoi(node.val(), &i)) {
		std::vector<std::string> path;
		for (auto n = node; n.has_parent(); n = n.parent())
			path.push_back(n.key().data());
		logger->error(
				"[{}] was not correctly formatted. An int is expected, instead found '{}'", fmt::join(path, " > "),
				node.val().data()
		);
		return std::nullopt;
	}
	return std::make_optional(i);
}

template <> std::optional<float> GameEntry::getNodeValue(const ryml::ConstNodeRef& node) const noexcept {
	if (!node.has_val()) {
		std::vector<std::string> path;
		for (auto n = node; n.has_parent(); n = n.parent())
			path.push_back(n.key().data());
		logger->error("[{}] was found in info.yaml but has no value", fmt::join(path.rbegin(), path.rend(), " > "));
		return std::nullopt;
	}
	float f;
	if (!ryml::atof(node.val(), &f)) {
		std::vector<std::string> path;
		for (auto n = node; n.has_parent(); n = n.parent())
			path.push_back(n.key().data());
		logger->error(
				"[{}] was not correctly formatted. A float is expected, instead found '{}'", fmt::join(path, " > "),
				node.val().data()
		);
		return std::nullopt;
	}
	return std::make_optional(f);
}

template <typename T> std::optional<T> GameEntry::getInfoEntry(std::vector<std::string> path) const noexcept {
	auto node = getInfoEntryNode(path);
	return node.and_then([this](auto n) { return getNodeValue<T>(n); });
}

template <typename T>
std::optional<std::vector<T>> GameEntry::getInfoSeqEntry(std::vector<std::string> path) const noexcept {
	auto node = getInfoEntryNode(path);
	if (!node.has_value()) {
		return std::nullopt;
	}
	if (!node.value().is_seq()) {
		logger->error("[{}] was found but a sequence was expected", fmt::join(path, " > "));
		return std::nullopt;
	}
	std::vector<T> vec;
	for (auto&& c : node.value().cchildren()) {
		auto v = getNodeValue<T>(c);
		if (!v.has_value())
			return std::nullopt;
		vec.push_back(v.value());
	}
	return std::make_optional(vec);
}

std::string GameEntry::getName() const noexcept {
	if (auto title = getInfoEntry<std::string>({"title"}); title.has_value())
		return title.value();
	const std::string name = entry.path().stem();
	auto end = name.end();
	// Remove anything trailing that could be a date or hint.
	for (auto& regex : {dateRegex, anyHintRegex})
		if (std::smatch match; std::regex_search(name.begin(), end, match, regex))
			end = name.begin() + match.prefix().length();
	// Remove trailing whitespaces
	// I don't like that this is necessary (https://stackoverflow.com/a/21578623)
	const auto isspace = [](char c) { return std::isspace(static_cast<unsigned char>(c)); };
	end = std::find_if_not(std::make_reverse_iterator(end), name.rend(), isspace).base();
	return std::string(name.begin(), end);
}

/** \todo stoi and the like my not be ideal since they throw exceptions. Instead a function of the signature
 * bool myfunc(std::string str, int& out) would be best. **/
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
	std::regex regex(std::format("\\[{}-(.*?)\\]", key));
	std::smatch match;
	std::string name = entry.path().stem();
	return std::regex_search(name, match, regex) ? std::make_optional(match[1])
												 : getInfoEntry<std::string>({"hints", key});
}

std::vector<std::string> GameEntry::getTags() const noexcept {
	return getInfoSeqEntry<std::string>({"tags"}).value_or(std::vector<std::string>());
}

std::vector<std::string> GameEntry::getDeveloper() const noexcept {
	return getInfoSeqEntry<std::string>({"developer"}).value_or(std::vector<std::string>());
}

std::vector<std::string> GameEntry::getPublisher() const noexcept {
	return getInfoSeqEntry<std::string>({"publisher"}).value_or(std::vector<std::string>());
}

std::optional<float> GameEntry::getRating() const noexcept { return getInfoEntry<float>({"rating"}); }

static dirent toDirEntry(std::string path) { return dirent(path); }

std::optional<dirent> GameEntry::getLogo() const noexcept {
	return getInfoEntry<std::string>({"media", "logo"}).transform(toDirEntry).or_else([this] {
		return getMemberFile(entry, "logo");
	});
}

std::optional<dirent> GameEntry::getBanner() const noexcept {
	return getInfoEntry<std::string>({"media", "banner"}).transform(toDirEntry).or_else([this] {
		return getMemberFile(entry, "banner");
	});
}

std::optional<dirent> GameEntry::getBoxart() const noexcept {
	return getInfoEntry<std::string>({"media", "boxart"}).transform(toDirEntry).or_else([this] {
		return getMemberFile(entry, "boxart");
	});
}

std::vector<std::filesystem::path> GameEntry::getScreenshots() const noexcept {
	auto screenshotDir = entry.path() / "screenshots";
	if (std::filesystem::is_directory(screenshotDir)) {
		auto range = std::filesystem::recursive_directory_iterator(screenshotDir) |
					 std::views::transform(std::mem_fn(&dirent::path)) | std::views::filter(isPicture);
		std::vector<std::filesystem::path> screenshots;
		for (auto&& path : range)
			screenshots.emplace_back(path);
		return screenshots;
	}
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
