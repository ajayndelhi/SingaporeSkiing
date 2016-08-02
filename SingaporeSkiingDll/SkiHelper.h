#include "stdafx.h"
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#undef __ACTIVATE_PAUSE__
class SkiHelper
{
private:
	static short ** CreateSimpleTestData(int size);
	static short ** CreateLargeTestData(int size);
	static short ** CreateEmptyDataGrid(int size);
	static short atoi(char *);
public:
	static __declspec(dllexport) short ** CreateTestData(int size);
	static __declspec(dllexport) void DeleteTestData(int size, short **);
	static __declspec(dllexport) const std::string CurrentDateTime();
	static __declspec(dllexport) void Pause();

	static __declspec(dllexport) void GetTokensFromLine(char *buf, int maxTokens, short * const);
protected:
};

