#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "SkiHelper.h"
#include "exception"
#include <ctime>
using namespace std;

short ** SkiHelper::CreateSimpleTestData(int size)
{
	short **ptr = NULL;

	ptr = new short*[size];
	for(int i = 0; i < size; i++)
	{
		ptr[i] = new short[size];
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


short ** SkiHelper::CreateTestData(int size)
{
	if (size == 4)
	{
		return SkiHelper::CreateSimpleTestData(size);
	}
	else if (size == 1000)
	{
		return SkiHelper::CreateLargeTestData(size);
	}
	else
	{
		throw new std::exception("Invalid data size.");
	}
}

void SkiHelper::DeleteTestData(int size, short **ptr)
{
	if (ptr == NULL)
	{
		return;
	}

	for(int i = 0; i < size; i++)
	{
		delete ptr[i];
		ptr[i]=NULL;
	}

	delete [] ptr;
}


short ** SkiHelper::CreateLargeTestData(int size)
{
	short **ptr = NULL;

	// read data from file
	string line;
	ifstream inputFile("C:\\TestProjects\\SingaporeSkiing\\LargeTestData.txt");
	if (inputFile.is_open())
	{
		int lineCount = 0;
		vector<short> elems;
		while( getline(inputFile, line))
		{
			lineCount++;
			/// skip first line
			if (lineCount > 1)
			{
				//cout << lineCount << endl;
				SkiHelper::GetTokensFromLine(line, elems);
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
		ptr = new short*[size];
		for(int i = 0; i < size; i++)
		{
			ptr[i] = new short[size];
			for(int j=0; j < size; j++)
			{
				ptr[i][j] = elems[i*size+j];
			}
		}
	}
	return ptr;
}

void SkiHelper::GetTokensFromLine(string line, vector<short> &elems)
{
	stringstream ss(line);
	string item;
	while(getline(ss, item, ' '))
	{
		elems.push_back(stoi(item));
	}
}

const std::string SkiHelper::CurrentDateTime()
{
	time_t     now = time(0);
	struct tm  *tstruct = new struct tm;
	char       buf[80];

	localtime_s(tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", tstruct);

	return buf;
}

void SkiHelper::Pause()
{
#ifdef __ACTIVATE_PAUSE__
	char c;
	cout << "Press key to continue...";
	cin>>c;
#endif
}