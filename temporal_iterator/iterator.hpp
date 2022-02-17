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
	using difference_type = std::ptrdiff_t;
	using interval_type = std::chrono::duration<typename value_type::rep, typename value_type::period>;
	using clock = typename TimePoint_T::clock;

	basic_iterator(value_type start, value_type end, interval_type inc)
		: _current{ start }
		, _end{ end }
		, _increment{ inc }
	{}

	basic_iterator(value_type start, value_type end)
		: basic_iterator{ start, end, interval_type{} }
	{}

	basic_iterator(value_type start, interval_type duration, interval_type inc)
		: _current{ start }
		, _end{ _current + duration }
		, _increment{ inc }
	{}

	basic_iterator(value_type end, interval_type inc)
		: _current{ value_type::clock::now()}
		, _end{ end }
		, _increment{ inc }
	{}

	basic_iterator(interval_type duration, interval_type inc)
		: _current{ value_type::clock::now() }
		, _end{ _current + duration }
		, _increment{ inc }
	{}

	explicit basic_iterator(interval_type duration)
		: basic_iterator{ duration, interval_type{} }
	{}

	explicit basic_iterator(value_type end)
		: basic_iterator{ end, interval_type{} }
	{}

	basic_iterator()
		: _current{}
		, _end{}
		, _increment{}
	{}

	basic_iterator(const basic_iterator&) = default;
	basic_iterator& operator=(const basic_iterator&) = default;
	basic_iterator(basic_iterator&&) = default;
	basic_iterator& operator=(basic_iterator&&) = default;

	auto operator<=>(const basic_iterator&) const = default;

	reference operator*() const
	{
		return _current;
	}

	basic_iterator& operator++()
	{
		_current += _increment;

		if (_current >= _end) {
			*this = basic_iterator{};
			return *this;
		}

		std::this_thread::sleep_until(_current);
		

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
	value_type _end;
	interval_type _increment;
};

using iterator = basic_iterator<std::chrono::high_resolution_clock::time_point>;

}
}
