#pragma once

#include "soci/type-conversion-traits.h"

#include <filesystem>

namespace soci {

	template<>
	struct type_conversion<std::filesystem::path> {
		typedef typename type_conversion<std::string>::base_type base_type;

		static void from_base(base_type const& in, indicator ind, std::filesystem::path& out) {
            std::string str;
            type_conversion<std::string>::from_base(in, ind, str);
            out = std::filesystem::path(str);
		}

		static void to_base(std::filesystem::path const& in, base_type& out, indicator& ind) {
            type_conversion<std::string>::to_base(in, out, ind);
		}
	};

} // namespace soci