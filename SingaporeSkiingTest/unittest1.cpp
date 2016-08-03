#include "stdafx.h"
#include "CppUnitTest.h"
#include "RouteGrid.h"
#include "SkiHelper.h"
#include "List.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define SIMPLEDATAFILE "C:\\TestProjects\\SingaporeSkiing\\SmallTestData.txt"
#define LARGEDATAFILE "C:\\TestProjects\\SingaporeSkiing\\LargeTestData.txt"

namespace SingaporeSkiingTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			// Arrange
			short **dataGrid = NULL;
			int rowCount = 0;
			int colCount = 0;
			dataGrid = SkiHelper::CreateTestData(SIMPLEDATAFILE, &rowCount, &colCount);

			RouteGrid *rg = new RouteGrid(dataGrid, rowCount, colCount);

			// Act
			bool dataStatus = rg->ValidateData(1, 9);

			// Assert
			Assert::IsTrue(dataStatus, L"Data Grid has numbers out of range");
		}

		TEST_METHOD(TestList)
		{
			// Arrange
			List<int> *myIntList = new List<int>(10);
			myIntList->Add(15);
			myIntList->Add(25);
			myIntList->Add(5);
			myIntList->Add(8);
			myIntList->Add(19);

			// Act
			int itemCount = myIntList->Count();

			//// Assert
			Assert::AreEqual<int>(6, itemCount, L"Item Count does not match");
		}

		TEST_METHOD(TestList2)
		{
			// Arrange
			List<int> *myIntList = new List<int>();
			myIntList->Add(15);
			myIntList->Add(25);
			myIntList->Add(5);
			myIntList->Add(8);
			myIntList->Add(19);

			// Act
			int itemCount = myIntList->Count();

			//// Assert
			Assert::AreEqual<int>(5, itemCount, L"Item Count does not match");
		}

		TEST_METHOD(TestList3)
		{
			// Arrange
			List<int> *myIntList = new List<int>();

			// Act
			int itemCount = myIntList->Count();

			//// Assert
			Assert::AreEqual<int>(0, itemCount, L"Item Count does not match");
		}

		TEST_METHOD(TestReadingNumbersLine1)
		{
			// Arrange
			char buf[80];
			strcpy(buf,"345 0 49 89 1500");
			short intArray[5];

			// Act
			SkiHelper::GetTokensFromLine(buf, 5, intArray);

			//// Assert
			Assert::AreEqual<int>(345, intArray[0]);
			Assert::AreEqual<int>(0, intArray[1]);
			Assert::AreEqual<int>(49, intArray[2]);
			Assert::AreEqual<int>(89, intArray[3]);
			Assert::AreEqual<int>(1500, intArray[4]);
		}

		TEST_METHOD(TestReadingNumbersLine2)
		{
			// Arrange
			char buf[80];
			strcpy(buf,"345 0 49 89 1500");
			short intArray[5];

			// Act
			SkiHelper::GetTokensFromLine(buf, 5, intArray);

			//// Assert
			Assert::AreEqual<int>(345, intArray[0]);
			Assert::AreEqual<int>(0, intArray[1]);
			Assert::AreEqual<int>(49, intArray[2]);
			Assert::AreEqual<int>(89, intArray[3]);
			Assert::AreEqual<int>(1500, intArray[4]);
		}

		TEST_METHOD(TestReadingNumbersLine3)
		{
			// Arrange
			char buf[80];
			strcpy(buf,"345 0 49 89 1500");
			short intArray[4];

			// Act
			SkiHelper::GetTokensFromLine(buf, 4, intArray);

			//// Assert
			Assert::AreEqual<int>(345, intArray[0]);
			Assert::AreEqual<int>(0, intArray[1]);
			Assert::AreEqual<int>(49, intArray[2]);
			Assert::AreEqual<int>(89, intArray[3]);
		}
	};
}