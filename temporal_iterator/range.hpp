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
	using difference_type = std::chrono::duration<typename value_type::rep, typename value_type::period>;

	basic_range(value_type begin, value_type end, difference_type step)
		: _begin{ begin }
		, _end{ end }
		, _step{ step }
	{
		assert(begin <= end);
	}

	basic_range(value_type begin, value_type end)
		: basic_range{ begin, end, difference_type{} }
	{}

	auto begin() const { return iterator{ _begin, _step }; }
	auto end() const { return iterator{ _end, _step }; }

private:
	const value_type _begin, _end;
	const difference_type _step;
};

using range = basic_range<std::chrono::system_clock::time_point>;

}
}
