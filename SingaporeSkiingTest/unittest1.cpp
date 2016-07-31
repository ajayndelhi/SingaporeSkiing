#include "stdafx.h"
#include "CppUnitTest.h"
#include "RouteGrid.h"
#include "TestData.h"
#include "List.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SingaporeSkiingTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			// Arrange
			int **dataGrid = NULL;
			dataGrid = TestData::CreateTestData(4);

			RouteGrid *rg = new RouteGrid(dataGrid, 4);

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
	};
}