#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "SkiHelper.h"
#include "exception"
#include <ctime>
using namespace std;

#define MaxFirstLineLen 80

short ** SkiHelper::CreateTestData(char *inputFile, int *rows, int *cols)
{
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, inputFile, "r");
	if (err != 0)
	{
		cout << "Unable to read input file" << endl;
		return NULL;
	}

	char buffer[MaxFirstLineLen+1];
	// read first line
	if (fgets(buffer, MaxFirstLineLen, fp))
	{
		short dimArray[2];
		SkiHelper::GetTokensFromLine(buffer, 2, dimArray);

		*rows = dimArray[0];
		*cols = dimArray[1];

		short **dataPtr = SkiHelper::CreateEmptyDataGrid(*rows, *cols);

		SkiHelper::ReadTestData(fp, *rows, *cols, dataPtr);

		fclose(fp);
		fp = NULL;
		return dataPtr;
	}
	else
	{
		cout << "unable to read first line from input data file" << endl;
		fclose(fp);
		return NULL;
	}
}

void SkiHelper::DeleteTestData(int rows, int cols, short **ptr)
{
	if (ptr == NULL)
	{
		return;
	}

	for(int i = 0; i < rows; i++)
	{
		delete [] ptr[i];
		ptr[i]=NULL;
	}

	delete [] ptr;
}

short ** SkiHelper::CreateEmptyDataGrid(int rows, int cols)
{
	short **ptr = NULL;
	ptr = new short*[rows];
	for (int i = 0; i < rows; i++)
	{
		ptr[i] = new short[cols];
		for (int j = 0; j < cols; j++)
		{
			ptr[i][j] = 0;
		}
	}

	return ptr;
}

void SkiHelper::ReadTestData(FILE *fp, int rows, int cols, short **ptr)
{
	int maxLineLen = cols * 5;
	char *lineBuffer = (char *)calloc(maxLineLen+1, sizeof(char));

	for (int lineCount = 0; fgets(lineBuffer, maxLineLen, fp) != NULL; lineCount++)
	{
		SkiHelper::GetTokensFromLine(lineBuffer, cols, ptr[lineCount]);
	}

	fclose(fp);
	fp = NULL;

	free(lineBuffer);
}

void SkiHelper::GetTokensFromLine(char *buf, int maxTokens, short * const ptr)
{
	int j = 0;
	char c;
	
	char *bufStart = buf;

	for (bufStart = buf; *buf != '\0' && j < maxTokens; buf++)
	{
		if (*buf == ' ')
		{
			*buf = '\0';
			ptr[j++] = SkiHelper::atoi(bufStart);
			bufStart = buf + 1;
		}
	}

	if (bufStart < buf && *buf=='\0' && j < maxTokens)
	{
		ptr[j] = SkiHelper::atoi(bufStart);
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

const char * SkiHelper::CurrentDateTime(char *buf, int size)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(buf, size, "%d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
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