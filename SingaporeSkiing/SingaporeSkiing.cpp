// SingaporeSkiing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RouteGrid.h"
#include "TestData.h"
#include "exception"
#include <iostream>
#include <ctime>

//
// Problem Definition
// http://geeks.redmart.com/2015/01/07/skiing-in-singapore-a-coding-diversion/

// sample walkthrough
// https://msdn.microsoft.com/en-us/library/ms235636(v=vs.110).aspx

#define SIMPLEDATASIZE 4
#define COMPLEXDATASIZE 1000

const std::string currentDateTime() 
{
    time_t     now = time(0);
    struct tm  *tstruct = new struct tm;
    char       buf[80];

    localtime_s(tstruct, &now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", tstruct);

    return buf;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int dataSize = SIMPLEDATASIZE;

	int **dataGrid = NULL;

	try
	{
		cout << currentDateTime() << " Reading input data..." << endl;
		dataGrid = TestData::CreateTestData(dataSize);
		
		cout << currentDateTime() << " Creating class object..." << endl;
		RouteGrid *rg = new RouteGrid(dataGrid, dataSize);

		cout << currentDateTime() << " Validating input data values..." << endl;
		bool dataStatus = rg->ValidateData(0, 1500);

		if (!dataStatus)
		{
			printf("Data is not valid\n");
			return -1;
		}

		// create nodes for the items in the array
		cout << currentDateTime() << " Creating graph list..." << endl;
		rg->CreateList();

		cout << currentDateTime() << " Finding Best Path..." << endl;
		rg->DenormalizePaths();

		cout << currentDateTime() << " Releasing Memory..." << endl;
		rg->DeleteList();
		delete rg;

		cout << currentDateTime() << " Deleting Test Data from memory..." << endl;
		TestData::DeleteTestData(dataSize, dataGrid);
		dataGrid = NULL;
	}
	catch(std::exception &e)
	{
		std::cout << "an error occured\n";
	}
}

