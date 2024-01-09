#pragma once

#include "soci/type-conversion-traits.h"

#include <functional>

namespace soci {

	template <typename T>
	struct type_conversion<std::reference_wrapper<T>> {
		typedef typename type_conversion<T>::base_type base_type;

		static void from_base(base_type const& in, indicator ind, const std::reference_wrapper<T>& out) {
			type_conversion<T>::from_base(in, ind, *out);
		}

		static void to_base(std::reference_wrapper<T> const& in, base_type& out, indicator& ind) {
			type_conversion<T>::to_base(in.get(), out, ind);
		}
	};

} // namespace soci