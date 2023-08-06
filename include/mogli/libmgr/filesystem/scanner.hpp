#pragma once

#include <filesystem>

namespace mogli::lib {

	class Scanner final {
	private:
		virtual bool hasChanges(std::filesystem::directory_entry entry);

	public:
		void scan(std::filesystem::path path);
	};

}