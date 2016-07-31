// SingaporeSkiing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RouteGrid.h"
#include "SkiHelper.h"
#include "exception"
#include <iostream>

//
// Problem Definition
// http://geeks.redmart.com/2015/01/07/skiing-in-singapore-a-coding-diversion/

// sample walkthrough
// https://msdn.microsoft.com/en-us/library/ms235636(v=vs.110).aspx

#define SIMPLEDATASIZE 4
#define COMPLEXDATASIZE 1000

int _tmain(int argc, _TCHAR* argv[])
{
	int dataSize = SIMPLEDATASIZE;

	int **dataGrid = NULL;

	try
	{
		cout << SkiHelper::CurrentDateTime() << " Reading input data..." << endl;
		dataGrid = SkiHelper::CreateTestData(dataSize);
		
		cout << SkiHelper::CurrentDateTime() << " Creating class object..." << endl;
		RouteGrid *rg = new RouteGrid(dataGrid, dataSize);

		cout << SkiHelper::CurrentDateTime() << " Validating input data values..." << endl;
		bool dataStatus = rg->ValidateData(0, 1500);

		if (!dataStatus)
		{
			printf("Data is not valid\n");
			return -1;
		}

		// create nodes for the items in the array
		cout << SkiHelper::CurrentDateTime() << " Creating graph list..." << endl;
		rg->CreateList();

		cout << SkiHelper::CurrentDateTime() << " Finding Best Path..." << endl;
		rg->DenormalizePaths();

		cout << SkiHelper::CurrentDateTime() << " Releasing Memory..." << endl;
		delete rg;

		cout << SkiHelper::CurrentDateTime() << " Deleting Test Data from memory..." << endl;
		SkiHelper::DeleteTestData(dataSize, dataGrid);
		dataGrid = NULL;
	}
	catch(std::exception &e)
	{
		std::cout << "an error occured\n";
	}
}

