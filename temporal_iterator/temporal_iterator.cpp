#include "CppUnitTest.h"

#include "iterator.hpp"

#include <string>
#include <format>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::chrono_literals;

template<>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<kjc::chrono::iterator::clock::time_point>(const kjc::chrono::iterator::clock::time_point& tp)
{
	return std::format(L"{}", tp.time_since_epoch());
}

template<>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<kjc::chrono::iterator>(const kjc::chrono::iterator& it)
{
	return ToString(*it);
}

template<>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::chrono::microseconds>(const std::chrono::microseconds& t)
{
	return std::format(L"{}", t);
}

template<>
std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<std::chrono::milliseconds>(const std::chrono::milliseconds& t)
{
	return std::format(L"{}", t);
}

namespace
{

template<typename Value_T, typename Delta_T>
bool approx_equal(const Value_T& expected, const Value_T& actual, const Delta_T& delta)
{
	if (!(actual - delta < expected) && (actual + delta > expected)) {
		Logger::WriteMessage(std::format(L"Expected: {} +/- {}, actual: {}\n", ToString(expected), ToString(delta), ToString(actual)).c_str());
		return false;
	}

	return true;
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
			const auto time_point = kjc::chrono::iterator::clock::now() + 1s;
			const auto it = kjc::chrono::iterator{ time_point };

			Assert::IsTrue(approx_equal(*it, kjc::chrono::iterator::clock::now(), 10us));
		}

		TEST_METHOD(ConstructWithEndTimeAndIncrement)
		{
			const auto start_time = kjc::chrono::iterator::clock::now();
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
			const auto now = kjc::chrono::iterator::clock::now();
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
			const auto time_point = kjc::chrono::iterator::clock::now();

			const auto it_1 = kjc::chrono::iterator{ time_point, time_point + 1s };
			const auto it_2 = kjc::chrono::iterator{ time_point, time_point + 1s };

			Assert::IsTrue(it_1 == it_2);
			Assert::IsFalse(it_1 != it_2);
		}

		TEST_METHOD(Inequality)
		{
			const auto time_point = kjc::chrono::iterator::clock::now();

			const auto it_1 = kjc::chrono::iterator{ time_point };
			const auto it_2 = kjc::chrono::iterator{ time_point + 1us };

			Assert::IsTrue(it_1 != it_2);
			Assert::IsFalse(it_1 == it_2);
		}

		TEST_METHOD(LessAndGreater)
		{
			const auto time_point = kjc::chrono::iterator::clock::now();

			const auto it_1 = kjc::chrono::iterator{ time_point };
			const auto it_2 = kjc::chrono::iterator{ time_point + 1us };

			Assert::IsTrue(it_1 < it_2);
			Assert::IsFalse(it_2 < it_1);

			Assert::IsTrue(it_2 > it_1);
			Assert::IsFalse(it_1 > it_2);
		}
	};

	TEST_CLASS(TestIncrement)
	{
	public:
		TEST_METHOD(ExpectedNumberOfIncrementsIn100ms)
		{
			for (auto i = 0; i < 20; ++i) {
				const auto start = kjc::chrono::iterator::clock::now() + 50ms;
				auto count = 0;
				std::for_each(kjc::chrono::iterator{ start, 100ms, 10ms }, kjc::chrono::iterator{}, [&count](const auto&) {++count; });

				const auto approx_end = kjc::chrono::iterator::clock::now();
				Assert::IsTrue(approx_equal(100ms, std::chrono::duration_cast<std::chrono::milliseconds>(approx_end - start), 10ms));
				Assert::AreEqual(10, count);
			}

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

		TEST_METHOD(TransformForLogging)
		{
			using Iter_t = kjc::chrono::basic_iterator<std::chrono::system_clock::time_point>;

			std::stringstream log;
			std::transform(Iter_t{ 1s, 50ms }, Iter_t{}, std::ostream_iterator<std::string>{log}, [](auto&& time_point) {
				return std::format("{}: Log entry!\n", time_point);
				});

			Logger::WriteMessage(log.str().c_str());
		}
	};

}	// namespace: test_iterator
