#include "CppUnitTest.h"

#include "iterator.hpp"
#include "range.hpp"

#include <string>
#include <format>
#include <chrono>

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

namespace test_iterator
{
	TEST_CLASS(TestConstruction)
	{
	public:
		TEST_METHOD(DefaultConstructionDereferencesToCurrentTime)
		{
			const auto it = kjc::chrono::iterator{};
			Assert::IsTrue(approx_equal(*it, std::chrono::system_clock::now(), 10us));
		}

		TEST_METHOD(ConstructWithTimePoint)
		{
			const auto time_point = std::chrono::system_clock::now() + 1s;
			const auto it = kjc::chrono::iterator{ time_point };

			Assert::AreEqual(time_point, *it);
		}

		TEST_METHOD(ConstructWithTimePointAndIncrement)
		{
			const auto time_point = std::chrono::system_clock::now() + 1s;
			auto it = kjc::chrono::iterator{time_point, 100ms};

			Assert::AreEqual(time_point, *it);
			Assert::AreEqual(time_point + 100ms, *(++it));
			Assert::AreEqual(time_point + 100ms, *it++);
			Assert::AreEqual(time_point + 200ms, *it);
		}

		TEST_METHOD(ConstructWithIncrement)
		{
			auto it = kjc::chrono::iterator{ 100ms };
			const auto base_time = std::chrono::system_clock::now();
			
			for (auto i = 0; i < 10; ++i, ++it) {
				Assert::IsTrue(approx_equal(*it, base_time + (i * 100ms), 10us));
			}
		}
	};

	TEST_CLASS(TestComparison)
	{
	public:
		TEST_METHOD(Equality)
		{
			const auto time_point = std::chrono::system_clock::now();

			const auto it_1 = kjc::chrono::iterator{ time_point };
			const auto it_2 = kjc::chrono::iterator{ time_point };

			Assert::IsTrue(it_1 == it_2);
			Assert::IsFalse(it_1 != it_2);
		}

		TEST_METHOD(Inequality)
		{
			const auto time_point = std::chrono::system_clock::now();

			const auto it_1 = kjc::chrono::iterator{ time_point };
			const auto it_2 = kjc::chrono::iterator{ time_point + 1us };

			Assert::IsTrue(it_1 != it_2);
			Assert::IsFalse(it_1 == it_2);
		}

		TEST_METHOD(LessAndGreater)
		{
			const auto time_point = std::chrono::system_clock::now();

			const auto it_1 = kjc::chrono::iterator{ time_point };
			const auto it_2 = kjc::chrono::iterator{ time_point + 1us };

			Assert::IsTrue(it_1 < it_2);
			Assert::IsFalse(it_2 < it_1);

			Assert::IsTrue(it_2 > it_1);
			Assert::IsFalse(it_1 > it_2);
		}
	};


	TEST_CLASS(TestAlgorithms)
	{
	public:
		TEST_METHOD(FindIf)
		{
			const auto begin = kjc::chrono::iterator{ 10ms };
			const auto end = kjc::chrono::iterator{ std::chrono::system_clock::now() + 1s };

			const auto it = std::find_if(begin, end, [](const auto&) { static int i = 0; return ++i == 20; });

			Assert::IsTrue(it < end);
		}
	};

}	// namespace: test_iterator

namespace test_range
{
TEST_CLASS(TestConstruction)
{
public:
	TEST_METHOD(ConstructWithBeginAndEndTimePoint)
	{
		const auto t = std::chrono::system_clock::now();
		const auto time_range = kjc::chrono::range{ t, t + 10ms };

		Assert::AreEqual(t, *time_range.begin());
		Assert::AreEqual(t + 10ms, *time_range.end());
	}

	TEST_METHOD(ConstructWithBeginEndAndIncrement)
	{
		const auto t = std::chrono::system_clock::now();
		const auto time_range = kjc::chrono::range{ t, t + 10ms, 1ms };

		auto it = time_range.begin();

		Assert::AreEqual(t, *it);
		Assert::AreEqual(t + 10ms, *time_range.end());

		++it;
		Assert::AreEqual(t + 1ms, *it);
	}

	TEST_METHOD(ConstructWithDurationAndIncrement)
	{
		const auto time_range = kjc::chrono::range{ 100ms, 10ms };

		auto it = time_range.begin();
		for (auto i = 0; i < 10; ++i, ++it) {
			Assert::AreEqual(*time_range.begin() + i * 10ms, *it);
		}

		Assert::IsTrue(time_range.end() == it);
	}
};
}
