#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "TestData.h"
#include "exception"
using namespace std;

int ** TestData::CreateSimpleTestData(int size)
{
	int **ptr = NULL;

	ptr = new int*[size];
	for(int i = 0; i < size; i++)
	{
		ptr[i] = new int[size];
	}

	ptr[0][0] = 4;
	ptr[0][1] = 8;
	ptr[0][2] = 7;
	ptr[0][3] = 3;

	ptr[1][0] = 2;
	ptr[1][1] = 5;
	ptr[1][2] = 9;
	ptr[1][3] = 3;

	ptr[2][0] = 6;
	ptr[2][1] = 3;
	ptr[2][2] = 2;
	ptr[2][3] = 5;

	ptr[3][0] = 4;
	ptr[3][1] = 4;
	ptr[3][2] = 1;
	ptr[3][3] = 6;

	return ptr;
}


int ** TestData::CreateTestData(int size)
{
	if (size == 4)
	{
		return TestData::CreateSimpleTestData(size);
	}
	else if (size == 1000)
	{
		return TestData::CreateLargeTestData(size);
	}
	else
	{
		throw new std::exception("Invalid data size.");
	}
}

void TestData::DeleteTestData(int size, int **ptr)
{
	if (ptr == NULL)
	{
		return;
	}

	ptr = new int*[size];
	for(int i = 0; i < size; i++)
	{
		delete ptr[i];
		ptr[i]=NULL;
	}

	delete ptr;
}


int ** TestData::CreateLargeTestData(int size)
{
	int **ptr = NULL;

	// read data from file
	string line;
	ifstream inputFile("C:\\TestProjects\\SingaporeSkiing\\LargeTestData.txt");
	if (inputFile.is_open())
	{
		int lineCount = 0;
		vector<int> elems;
		while( getline(inputFile, line))
		{
			lineCount++;
			/// skip first line
			if (lineCount > 1)
			{
				//cout << lineCount << endl;
				TestData::GetTokensFromLine(line, elems);
			}
		}
		inputFile.close();

		// validate data
		if (lineCount != size+1)
		{
			throw new std::exception("Invalid Input data");
		}

		if (elems.size() != size*size)
		{
			throw new std::exception("Invalid Input data");
		}

		// now create two dimensional array
		ptr = new int*[size];
		for(int i = 0; i < size; i++)
		{
			ptr[i] = new int[size];
			for(int j=0; j < size; j++)
			{
				ptr[i][j] = elems[i*size+j];
			}
		}
	}
	return ptr;
}

void TestData::GetTokensFromLine(string line, vector<int> &elems)
{
	stringstream ss(line);
	string item;
	while(getline(ss, item, ' '))
	{
		elems.push_back(stoi(item));
	}
}