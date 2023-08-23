#pragma once

#include "generic_iterator.hpp"

namespace mogli::utils {
    template<typename T>
    class Iterable final {
    private:
        generic_iterator<T> _begin;
        generic_iterator<T> _end;

    public:
        Iterable(generic_iterator<T> begin, generic_iterator<T> end) noexcept : _begin(begin), _end(end) {}

        template<typename Container>
        explicit Iterable(Container c) noexcept : Iterable(std::ranges::begin(c), std::ranges::end(c)) {}

        generic_iterator<T> begin() const noexcept { return _begin; }
        generic_iterator<T> end() const noexcept { return _end; }
    };
}