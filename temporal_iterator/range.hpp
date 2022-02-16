#pragma once

#include "iterator.hpp"

#include <ranges>
#include <cassert>
#include <chrono>

namespace kjc
{
namespace chrono
{

template<typename TimePoint_T>
class basic_range : public std::ranges::view_interface<basic_range<TimePoint_T>>
{
public:
	using value_type = TimePoint_T;

	basic_range(value_type begin, value_type end)
		: _begin{ begin }
		, _end{ end }
	{
		assert(begin <= end);
	}

	auto begin() const { return iterator{ _begin }; }
	auto end() const { return iterator{ _end }; }

private:
	const value_type _begin, _end;
};

using range = basic_range<std::chrono::system_clock::time_point>;

}
}
