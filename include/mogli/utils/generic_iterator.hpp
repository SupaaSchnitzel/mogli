#pragma once

#include <iterator>
#include <memory>

namespace mogli::utils {

    template<typename T>
    class GenericIterator {
    private:
        class AIter {
        public:
            virtual ~AIter() noexcept = default;
            virtual T deref() const noexcept = 0;
            virtual void advance() noexcept = 0;
            virtual bool equals(const AIter& other) const noexcept = 0;
            virtual const std::type_info& type() const = 0;
            virtual std::unique_ptr<AIter> clone() const = 0;
        };

        template<std::input_or_output_iterator TIt>
        class Iter final : public AIter {
        private:
            TIt iterator;
            std::shared_ptr<void> userdata;
        
        public:
            explicit Iter(TIt& iterator) noexcept : iterator(iterator), userdata(nullptr) {}

            template<typename K>
            Iter(TIt& iterator, std::shared_ptr<K> userdata) : iterator(iterator), userdata(userdata) {}

            T deref() const noexcept override { return *iterator; }
            void advance() noexcept override { std::ranges::advance(iterator, 1); }
            bool equals(const AIter& other) const noexcept override {
                return other.type() == type() && static_cast<const Iter<TIt>&>(other).iterator == iterator;
            }
            const std::type_info& type() const override { return typeid(this); }
            std::unique_ptr<AIter> clone() const override { return std::make_unique<Iter<TIt>>(*this); }
        };

        std::unique_ptr<AIter> impl;
    public:
        GenericIterator(const GenericIterator& other) : impl(other.impl->clone()) {}

        template<std::input_or_output_iterator TIt>
        explicit GenericIterator(TIt iterator) noexcept : impl(std::make_unique<Iter<TIt>>(iterator)) {}
        
        template<std::input_or_output_iterator TIt, typename K>
        GenericIterator(TIt iterator, std::shared_ptr<K> userdata) noexcept : impl(std::make_unique<Iter<TIt>>(iterator, userdata)) {}

        T operator*() const { return impl->deref(); }
        GenericIterator& operator++() noexcept {
            impl->advance();
            return *this;
        }
        bool operator==(const GenericIterator& other) const { return impl->equals(*other.impl); }
    };
} // namespace mogli::utils