// copyright: Ajay Aggarwal
// SingaporeSkiing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SkiHelper.h"
#include "exception"
#include <iostream>
#include "SkiResort.h"

//
// Problem Definition
// http://geeks.redmart.com/2015/01/07/skiing-in-singapore-a-coding-diversion/

// sample walkthrough
// https://msdn.microsoft.com/en-us/library/ms235636(v=vs.110).aspx


#define SIMPLEDATAFILE "C:\\TestProjects\\SingaporeSkiing\\SmallTestData.txt"
#define LARGEDATAFILE "C:\\TestProjects\\SingaporeSkiing\\LargeTestData.txt"

#define TimeBufSize 80
int _tmain(int argc, _TCHAR* argv[])
{
	short **dataGrid = NULL;
	char c;

	char timeBuffer[TimeBufSize+1];

	try
	{
		cout << SkiHelper::CurrentDateTime(timeBuffer, TimeBufSize) << " Reading input data..." << endl;
		int rowCount = 0;
		int colCount = 0;
		dataGrid = SkiHelper::CreateTestData(LARGEDATAFILE, &rowCount, &colCount);

		// if there is error in creating test data - return;
		if (dataGrid == NULL)
		{
			return -1;
		}
		
		cout << SkiHelper::CurrentDateTime(timeBuffer, TimeBufSize) << " Creating class object..." << endl;
		SkiResort *sr = new SkiResort(dataGrid, rowCount, colCount);

		cout << SkiHelper::CurrentDateTime(timeBuffer, TimeBufSize) << " Validating input data values..." << endl;
		bool dataStatus = sr->ValidateData(0, 1500);

		if (!dataStatus)
		{
			cout << "Data is not valid" << endl;
			return -1;
		}

		cout << SkiHelper::CurrentDateTime(timeBuffer, TimeBufSize) << " Finding Best Path..." << endl;
		sr->FindBestRoute();
		sr->PrintBestSkiPath(cout);

		cout << SkiHelper::CurrentDateTime(timeBuffer, TimeBufSize) << " Releasing Memory..." << endl;
		delete sr;
		sr = NULL;

		cout << SkiHelper::CurrentDateTime(timeBuffer, TimeBufSize) << " Deleting Test Data from memory..." << endl;
		SkiHelper::DeleteTestData(rowCount, colCount, dataGrid);
		dataGrid = NULL;

		cout << SkiHelper::CurrentDateTime(timeBuffer, TimeBufSize) << " All Done" << endl;
	}
	catch(std::exception &e)
	{
		std::cout << "an error occured" << endl;
	}
}

