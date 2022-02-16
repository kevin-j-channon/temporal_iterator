#pragma once

#include "iterator.hpp"

#include <ranges>
#include <cassert>
#include <chrono>
#include <vector>

namespace kjc
{
namespace chrono
{

template<typename TimePoint_T>
class basic_range : public std::ranges::view_interface<basic_range<TimePoint_T>>
{
public:
	using value_type = TimePoint_T;
	using interval_type = std::chrono::duration<typename value_type::rep, typename value_type::period>;
	using iterator = iterator;
	using const_iterator = iterator;

	basic_range(value_type begin, value_type end, interval_type step)
		: _begin{ begin, step }
		, _end{ end, step }
	{
//		assert(begin < end || begin == end);
	}

	basic_range(value_type begin, value_type end)
		: basic_range{ begin, end, interval_type{} }
	{}

	basic_range(interval_type duration, interval_type step)
		: _begin{ value_type::clock::now(), step }
		, _end{*_begin + duration, step}
	{
//		assert(begin < end || begin == end); 
	}

	basic_range(interval_type duration)
		: basic_range{ duration, interval_type{} }
	{}

	basic_range(const basic_range&) = default;
	basic_range& operator=(const basic_range&) = default;
	basic_range(basic_range&&) = default;
	basic_range& operator=(basic_range&&) = default;

	auto begin() const { return _begin; }
	auto end() const { return _end; }

private:
	const iterator _begin;
	const iterator _end;
};

using range = basic_range<std::chrono::system_clock::time_point>;

}
}
