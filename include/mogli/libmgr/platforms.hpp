#pragma once

namespace mogli::lib {
	enum class Platform { Windows, Linux, Mac };

	inline const char* getName(Platform platform) {
		switch (platform) {
		case Platform::Windows:
			return "Windows";
		case Platform::Linux:
			return "Linux";
		case Platform::Mac:
			return "Mac";
		default:
			return "invalid";
		};
	}

	inline unsigned getPlatformID(Platform platform) {
		switch (platform) {
		case Platform::Windows:
			return 0;
		case Platform::Linux:
			return 1;
		case Platform::Mac:
			return 2;
		default:
			return -1;
		};
	}
} // namespace mogli::lib