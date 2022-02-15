#pragma once

#include <chrono>
#include <thread>
#include <utility>

namespace kjc
{
namespace chrono
{

template<typename TimePoint_T>
class basic_iterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = TimePoint_T;
	using reference = const value_type&;
	using pointer = const value_type*;
	using difference_type = std::chrono::duration<typename value_type::rep, typename value_type::period>;

	basic_iterator(value_type tp, difference_type inc)
		: _current{ tp }
		, _increment{ inc }
	{}

	basic_iterator()
		: basic_iterator{ value_type::clock::now(), difference_type{} }
	{}

	basic_iterator(value_type tp)
		: basic_iterator{ tp, difference_type{} }
	{}

	basic_iterator(difference_type inc)
		: basic_iterator{ value_type::clock::now(), inc }
	{}

	basic_iterator(const basic_iterator&) = default;
	basic_iterator& operator=(const basic_iterator&) = default;
	basic_iterator(basic_iterator&&) = default;
	basic_iterator& operator=(basic_iterator&&) = default;

	reference operator*() const
	{
		return _current;
	}

	basic_iterator& operator++()
	{
		const auto next_value = _current + _increment;
		std::this_thread::sleep_until(next_value);
		_current = next_value;

		return *this;
	}

	const basic_iterator operator++(int)
	{
		const auto temp = *this;
		this->operator++();

		return temp;
	}

private:
	value_type _current;
	difference_type _increment;
};

using iterator = basic_iterator<std::chrono::system_clock::time_point>;

}
}
