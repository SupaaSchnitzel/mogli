#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

namespace mogli::utils {

	// https://stackoverflow.com/a/35866966
	template<typename value_type>
	struct generic_iterator {
		struct Concept {
			virtual void next(int n) = 0;
			virtual value_type deref() const = 0;
			virtual bool equal(const void* other) const = 0;
			virtual std::unique_ptr<Concept> clone() const = 0;
			virtual const std::type_info& type() const = 0;
			virtual const void* address() const = 0;
			virtual ~Concept() = default;
		};

		template <std::input_or_output_iterator Iter>
		struct ConcreteIter : Concept {
			ConcreteIter(Iter iter) : _iter(iter) {}

			void next(int n) override { _iter = std::ranges::next(_iter, n); }
			value_type deref() const override { return *_iter; }
			bool equal(const void* rp) const override { return _iter == static_cast<const ConcreteIter*>(rp)->_iter; }
			std::unique_ptr<Concept> clone() const override { return std::make_unique<ConcreteIter>(*this); }
			const std::type_info& type() const override { return typeid(_iter); }
			const void* address() const override { return this; }

			Iter _iter;
		};

		std::unique_ptr<Concept> _impl;

	public:
		template <std::input_or_output_iterator Iter>
		generic_iterator(Iter iter) : _impl(std::make_unique<ConcreteIter<Iter>>(iter)){};

		generic_iterator(const generic_iterator& r) : _impl(r._impl->clone()){};

		value_type operator*() const { return _impl->deref(); }

		generic_iterator& operator++() {
			_impl->next(1);
			return *this;
		}

		bool operator==(const generic_iterator& r) const {
			return _impl->type() == r._impl->type() and _impl->equal(r._impl->address());
		}

		bool operator!=(const generic_iterator& r) const { return not(*this == r); }
	};
} // namespace mogli::utils