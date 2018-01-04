#include "CppUnitTest.h"

#include "../ReqAnalysis/Simulator.h"

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<typename E>
			std::wstring ToString(const std::vector<E> & t)
			{
				std::wstringstream result;
				result << L"Size: " << t.size();

				if (t.size())
				{
					result << L", Elements: ";

					for (const auto & element : t)
					{
						result << L"{ " << element << L" } ";
					}
				}
				return result.str();
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(checkBudgetStudentMin)
		{
			Student *st = new Student();
			Assert::IsTrue((*st).checkBudget(0));
			delete st;
		}

		TEST_METHOD(checkBudgetStudentMax)
		{
			Student *st = new Student();
			Assert::IsFalse((*st).checkBudget(21));
			delete st;
		}

		TEST_METHOD(checkBudgetResearcherMin)
		{
			Researcher *re = new Researcher();
			Assert::IsTrue((*re).checkBudget(0));
			delete re;
		}

		TEST_METHOD(checkBudgetResearcherMax)
		{
			Researcher *re = new Researcher();
			Assert::IsFalse((*re).checkBudget(1600));
			delete re;
		}

		TEST_METHOD(ConversionToSecond)
		{
			std::vector<int> date = {2, 3, 4, 5};
			long seconds = convert(date);
			long verifSec = ((((2*24) + 3) * 60) + 4) * 60 + 5;
			Assert::AreEqual(verifSec, seconds);
		}

		TEST_METHOD(ConversionToDate)
		{
			long seconds = ((((5 * 24) + 8) * 60) + 1) * 60 + 7;
			std::vector<int> date = convert(seconds);
			std::vector<int> verifDate = { 5, 8, 1, 7};
			Assert::AreEqual(verifDate, date);
		}

		TEST_METHOD(ConversionToSecondFalse)
		{
			std::vector<int> date = { 5, 4, 4, 2 };
			long seconds = convert(date);
			long verifSec = ((((2 * 24) + 3) * 60) + 4) * 60 + 5;
			Assert::AreNotEqual(verifSec, seconds);
		}

		TEST_METHOD(ConversionToDateFalse)
		{
			long seconds = ((((5 * 24) + 8) * 60) + 1) * 60 + 7;
			std::vector<int> date = convert(seconds);
			std::vector<int> verifDate = { 0, 2, 8, 9 };
			Assert::AreNotEqual(verifDate, date);
		}

		TEST_METHOD(WaitTime)
		{
			Request *re = new shortR(1, 1);
			(*re).setWaitTime(10);
			long waitT = (*re).getWaitTime();
			Assert::AreEqual(long(10), waitT);
			delete re;
		}

		TEST_METHOD(Time)
		{
			Request *re = new shortR(1, 1);
			(*re).setTime(10);
			long T = (*re).getTime();
			Assert::AreEqual(long(10), T);
			delete re;
		}
	};
}