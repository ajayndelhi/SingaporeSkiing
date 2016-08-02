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

#define MaxLineLen 5000
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


short ** SkiHelper::CreateEmptyDataGrid(int size)
{
	short **ptr = NULL;
	ptr = new short*[size];
	for (int i = 0; i < size; i++)
	{
		ptr[i] = new short[size];
		for (int j = 0; j < size; j++)
		{
			ptr[i][j] = 0;
		}
	}

	return ptr;
}

short ** SkiHelper::CreateLargeTestData(int size)
{
	short **ptr = NULL;
	char lineBuffer[MaxLineLen + 1];
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, "C:\\TestProjects\\SingaporeSkiing\\LargeTestData.txt", "r");
	if (err != 0)
	{
		cout << "Unable to read input file" << endl;
		return NULL;
	}

	int lineCount = 0;

	// now create two dimensional array
	ptr = SkiHelper::CreateEmptyDataGrid(size);
	
	while (fgets(lineBuffer, MaxLineLen, fp))
	{
		lineCount++;
		/// skip first line as it has count only
		if (lineCount > 1)
		{
			SkiHelper::GetTokensFromLine(lineBuffer, size, ptr[lineCount - 2]);
		}
	}
	fclose(fp);
	fp = NULL;
	return ptr;
}

void SkiHelper::GetTokensFromLine(char *buf, int maxTokens, short * const ptr)
{
	int j = 0;
	char c;
	
	int si = 0;
	int ei = 0;
	for (si = 0, ei = 0; (c = buf[ei]) != '\0' && j < maxTokens; ei++)
	{
		if (c == ' ')
		{
			buf[ei] = '\0';
			ptr[j++] = SkiHelper::atoi(buf + si);
			si = ei + 1;
		}
	}

	if (si < ei && buf[ei]=='\0' && j < maxTokens)
	{
		ptr[j] = SkiHelper::atoi(buf + si);
	}
}

short SkiHelper::atoi(char *s)
{
	short value = 0;
	for (int i = 0; *s != '\n' && *s != '\0'; s++, i++)
	{
		value = value * 10 + (*s - '0');
	}
	return value;
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