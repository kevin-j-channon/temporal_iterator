#include "CppUnitTest.h"

#include "iterator.hpp"

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

template<>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<kjc::chrono::iterator>(const kjc::chrono::iterator& it)
{
	return std::format(L"(*{})", *it);
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
		TEST_METHOD(DefaultConstructionDereferencesToCur)
		{
			const auto it = kjc::chrono::iterator{};
		}

		TEST_METHOD(ConstructWithTimePoint)
		{
			const auto time_point = std::chrono::system_clock::now() + 1s;
			const auto it = kjc::chrono::iterator{ time_point };

			Assert::IsTrue(approx_equal(*it, std::chrono::system_clock::now(), 10us));
		}

		TEST_METHOD(ConstructWithEndTimeAndIncrement)
		{
			const auto start_time = std::chrono::system_clock::now();
			const auto end_time = start_time + 1s;
			auto it = kjc::chrono::iterator{end_time, 100ms};

			Assert::IsTrue(approx_equal(start_time, *it, 20us));
			Assert::IsTrue(approx_equal(start_time + 100ms, *(++it), 20us));
			Assert::IsTrue(approx_equal(start_time + 100ms, *it++, 20us));
			Assert::IsTrue(approx_equal(start_time + 200ms, *it, 20us));
		}

		TEST_METHOD(ConstructWithDurationAndIncrement)
		{
			auto it = kjc::chrono::iterator{ 100ms, 10ms };
			const auto now = std::chrono::system_clock::now();
			Assert::IsTrue(approx_equal(now, *it, 10us));

			++it;
			Assert::IsTrue(approx_equal(now + 10ms, *it, 10us));
		}
	};

	TEST_CLASS(TestComparison)
	{
	public:
		TEST_METHOD(Equality)
		{
			const auto time_point = std::chrono::system_clock::now();

			const auto it_1 = kjc::chrono::iterator{ time_point, time_point + 1s };
			const auto it_2 = kjc::chrono::iterator{ time_point, time_point + 1s };

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
		TEST_METHOD(FindIfFinds)
		{
			const auto begin = kjc::chrono::iterator{ 1s , 10ms };
			const auto end = kjc::chrono::iterator{};

			const auto it = std::find_if(begin, end, [](const auto&) { static int i = 0; return ++i == 20; });

			Assert::AreNotEqual(end, it);
		}

		TEST_METHOD(FindIfDoesntFind)
		{
			const auto begin = kjc::chrono::iterator{ 100ms , 10ms };
			const auto end = kjc::chrono::iterator{};

			const auto it = std::find_if(begin, end, [](const auto&) { return false; });

			Assert::AreEqual(end, it);
		}
	};

}	// namespace: test_iterator
