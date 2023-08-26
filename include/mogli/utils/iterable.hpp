#pragma once

#include "generic_iterator.hpp"

#include <iterator>
#include <memory>
#include <ranges>

namespace mogli::utils {

    /**
     * @brief Abstracts general containers into an "iterable" class. That is any object that can be iterated over via a
     * begin- and end-iterator can be used to initialize an instance of Iterable. This allows for datastructure agnostic
     * return values and even returning views. To additionally keep the original data in memory until the last iterator
     * is destroyed, you may pass additional user data.
     * @details See the following exmaple:
     * ```cpp
     * std::vector<int> myvec = { 1, 2, 3, 4 };
     * Iterable<int> iterable = myvec;
     * for (auto i : iterable)
     *     std::cout << i << std::endl;
     * ```
     * or, to avoid the underlying data to get out of scope before all iterators are destroyed:
     * ```cpp
     * Iterable<int> getData() {
    *     auto data = std::make_shared<std::vector>({ 1, 2, 3, 4 });
    *     return Iterable<int>(*data, data);
    * }
     * ```
     * 
     * @tparam T the element-type
     */
	template <typename T>
	class Iterable final {
	private:
		GenericIterator<T> _begin; /**< The first iterator. **/
		GenericIterator<T> _end; /**< The last iterator. **/

	public:
        /**
         * @brief Construct a new Iterable object from the provided begin- and end-iterators.
         * 
         * @tparam IterB The datatype of the begin-iterator.
         * @tparam IterE The datatype of the end-iterator.
         * @param begin The begin-iterator.
         * @param end The end-iterator.
         * @see Iterable(IterB, IterE)
         */
		template <std::input_or_output_iterator IterB, std::input_or_output_iterator IterE>
		Iterable(IterB begin, IterE end) noexcept : _begin(begin), _end(end) {}

        /**
         * @brief Construct a new Iterable object from the provided begin- and end-iterators. The additional `userdata`
         * parameter allows to keep the original data in memory until the last iterator is destroyed.
         * 
         * @tparam IterB The datatype of the begin-iterator.
         * @tparam IterE The datatype of the end-iterator.
         * @param begin The begin-iterator.
         * @param end The end-iterator.
         * @param userdata (Optional) additional userdata holding the data of the iterators.
         * @see Iterable(IterB, IterE, std::shared_ptr<auto>)
         */
		template <std::input_or_output_iterator IterB, std::input_or_output_iterator IterE>
		Iterable(IterB begin, IterE end, std::shared_ptr<auto> userdata) noexcept
				: _begin(begin, userdata), _end(end, userdata) {}

        /**
         * @brief Construct a new Iterable object from the provided container. This is equivalent to
         * `Iterable(begin(container), end(container))`.
         * 
         * @param container The container to be converted into an Iterable.
         * @see Iterable(std::ranges::range auto&, std::shared_ptr<auto>)
         */
		explicit Iterable(std::ranges::range auto& container) noexcept
				: Iterable(std::ranges::begin(container), std::ranges::end(container)) {}

        /**
         * @brief Construct a new Iterable object from the provided container. This is equivalent to
         * `Iterable(begin(container), end(container))`. The additional `userdata` parameter allows to keep the original
         * data in memory until the last iterator is destroyed.
         * 
         * @param container The container to be converted into an Iterable.
         * @param userdata (Optional) additional userdata holding the data of the iterators.
         * @see Iterable(std::ranges:range auto&)
         */
		Iterable(std::ranges::range auto& container, std::shared_ptr<auto> userdata) noexcept
				: Iterable(std::ranges::begin(container), std::ranges::end(container), userdata) {}

        /**
         * @brief Retrieves the iterator pointing to the first element in the iterable.
         * 
         * @return the iterator pointing to the first element in the iterable.
         */
		const GenericIterator<T>& begin() const noexcept { return _begin; }
        /**
         * @brief Retrieves the iterator pointing to the last element in the iterable.
         * 
         * @return the iterator pointing to the last element in the iterable.
         */
		const GenericIterator<T>& end() const noexcept { return _end; }
	};
} // namespace mogli::utils