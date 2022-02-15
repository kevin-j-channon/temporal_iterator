#include "CppUnitTest.h"

#include "temporal_iterator.hpp"

#include <string>
#include <format>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::chrono_literals;

template<>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::chrono::system_clock::time_point>(const std::chrono::system_clock::time_point& tp)
{
	return std::format(L"{}", tp);
}

namespace
{

template<typename Value_T, typename Delta_T>
bool approx_equal(const Value_T& expected, const Value_T& actual, const Delta_T& delta)
{
	return (actual - delta < expected) && (actual + delta > expected);
}

}	// namespace: 

namespace test_temporal_iterator
{

	TEST_CLASS(TestConstruction)
	{
	public:
		TEST_METHOD(DefaultConstructionDereferencesToCurrentTime)
		{
			auto it = kjc::chrono::iterator<>{};
			Assert::IsTrue(approx_equal(*it, std::chrono::system_clock::now(), 10us));
		}

		TEST_METHOD(ConstructWithTimePoint)
		{
			const auto time_point = std::chrono::system_clock::now() + 1s;
			auto it = kjc::chrono::iterator<>{ time_point };

			Assert::AreEqual(time_point, *it);
		}
	};

}	// namespace: test_temporal_iterator
