#include "Special.h"
#include <vector>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<typename E> std::wstring ToString(const std::vector<int> & t)
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