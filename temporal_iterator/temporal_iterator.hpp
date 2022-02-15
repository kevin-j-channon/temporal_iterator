#pragma once

#include <chrono>
#include <thread>
#include <utility>

namespace kjc
{
namespace chrono
{

template<typename TimePoint_T = std::chrono::system_clock::time_point>
class iterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = TimePoint_T;
	using reference = const value_type&;
	using pointer = const value_type*;
	using difference_type = std::chrono::duration<typename TimePoint_T::rep, typename TimePoint_T::period>;

	iterator()
		: _current{ value_type::clock::now() }
	{}

	iterator(value_type tp)
		: _current{ tp }
	{}

	reference operator*() const
	{
		return _current;
	}

private:
	value_type _current;
};

}
}
