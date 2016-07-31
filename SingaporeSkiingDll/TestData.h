#include "stdafx.h"
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class TestData
{
private:
	static int ** CreateSimpleTestData(int size);
	static int ** CreateLargeTestData(int size);
	static void GetTokensFromLine(string line, vector<int>&);
public:
	static __declspec(dllexport) int ** CreateTestData(int size);
	static __declspec(dllexport) void DeleteTestData(int size, int **);
protected:
};

