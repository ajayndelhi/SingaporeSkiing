#include "stdafx.h"
using namespace std;

#undef __ACTIVATE_PAUSE__
class SkiHelper
{
private:
	static void ReadTestData(FILE *fp, int rows, int cols, short **ptr);
	static short ** CreateEmptyDataGrid(int rows, int cols);
	static short atoi(char *);
public:
	static __declspec(dllexport) short ** CreateTestData(char *inputFile, int *rows, int *cols);
	static __declspec(dllexport) void DeleteTestData(int rows, int cols, short **);
	static __declspec(dllexport) const std::string CurrentDateTime();
	static __declspec(dllexport) void Pause();

	static __declspec(dllexport) void GetTokensFromLine(char *buf, int maxTokens, short * const);
protected:
};

