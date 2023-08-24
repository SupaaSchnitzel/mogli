#pragma once

#include "generic_iterator.hpp"

#include <iterator>
#include <memory>
#include <ranges>

namespace mogli::utils {
    template<typename T>
    class Iterable final {
    private:
        GenericIterator<T> _begin;
        GenericIterator<T> _end;
    
    public:
        template<std::input_or_output_iterator IterB, std::input_or_output_iterator IterE>
        Iterable(IterB begin, IterE end) noexcept : _begin(begin), _end(end) {}

        template<std::input_or_output_iterator IterB, std::input_or_output_iterator IterE, typename U>
        Iterable(IterB begin, IterE end, std::shared_ptr<U> userdata) noexcept : _begin(begin, userdata), _end(end, userdata) {}

        Iterable(std::ranges::range auto& container) noexcept : Iterable(std::ranges::begin(container), std::ranges::end(container)) {}

        template <typename U>
        Iterable(std::ranges::range auto& container, std::shared_ptr<U> userdata) noexcept : Iterable(std::ranges::begin(container), std::ranges::end(container), userdata) {}

        const GenericIterator<T>& begin() const noexcept { return _begin; }
        const GenericIterator<T>& end() const noexcept { return _end; }
    };
}