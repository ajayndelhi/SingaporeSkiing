#include "stdafx.h"
#include "CppUnitTest.h"
#include "SkiHelper.h"
#include "SkiResort.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define SIMPLEDATAFILE "C:\\TestProjects\\SingaporeSkiing\\SmallTestData.txt"
#define LARGEDATAFILE "C:\\TestProjects\\SingaporeSkiing\\LargeTestData.txt"

namespace SingaporeSkiingTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestSingaporeSkiingSimplePath)
		{
			// Arrange
			short **dataGrid = NULL;
			int rowCount = 0;
			int colCount = 0;
			dataGrid = SkiHelper::CreateTestData(SIMPLEDATAFILE, &rowCount, &colCount);
			SkiResort *sr = new SkiResort(dataGrid, rowCount, colCount);

			// Act
			bool dataStatus = sr->ValidateData(1, 9);
			sr->FindBestRoute();

			// Assert
			Assert::IsTrue(dataStatus, L"Data Grid has numbers out of range");
			Assert::AreEqual<int>(5, sr->GetBestSkiPathCount(), L"Best ski path count is not correct.");
			Assert::AreEqual<int>(8, sr->GetBestSkiPathElevation(), L"Best ski path elevation is not correct.");

			short *array = new short[sr->GetBestSkiPathCount()];
			sr->GetBestSkiPath(array);
			Assert::AreEqual<int>(9, *(array + 0), L"Invalid path");
			Assert::AreEqual<int>(5, *(array + 1), L"Invalid path");
			Assert::AreEqual<int>(3, *(array + 2), L"Invalid path");
			Assert::AreEqual<int>(2, *(array + 3), L"Invalid path");
			Assert::AreEqual<int>(1, *(array + 4), L"Invalid path");
			delete[] array;
			array = NULL;

			// Cleanup
			delete sr;
			sr = NULL;
			SkiHelper::DeleteTestData(rowCount, colCount, dataGrid);
			dataGrid = NULL;
		}

		TEST_METHOD(TestSingaporeSkiingComplexPath)
		{
			// Arrange
			short **dataGrid = NULL;
			int rowCount = 0;
			int colCount = 0;
			dataGrid = SkiHelper::CreateTestData(LARGEDATAFILE, &rowCount, &colCount);
			SkiResort *sr = new SkiResort(dataGrid, rowCount, colCount);

			// Act
			bool dataStatus = sr->ValidateData(0, 1500);
			sr->FindBestRoute();

			// Assert
			Assert::IsTrue(dataStatus, L"Data Grid has numbers out of range");
			Assert::AreEqual<int>(15, sr->GetBestSkiPathCount(), L"Best ski path count is not correct.");
			Assert::AreEqual<int>(1422, sr->GetBestSkiPathElevation(), L"Best ski path elevation is not correct.");

			short *array = new short[sr->GetBestSkiPathCount()];
			sr->GetBestSkiPath(array);
			Assert::AreEqual<int>(1422, *(array + 0), L"Invalid path");
			Assert::AreEqual<int>(0, *(array + sr->GetBestSkiPathCount() - 1), L"Invalid path");
			delete[] array;
			array = NULL;

			// Cleanup
			delete sr;
			sr = NULL;
			SkiHelper::DeleteTestData(rowCount, colCount, dataGrid);
			dataGrid = NULL;
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