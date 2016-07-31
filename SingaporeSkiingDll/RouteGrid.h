#include "stdafx.h"
#include "List.h"
#include "MountainPoint.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class RouteGrid
{
private:
	int** data;
	int gridSize;

	MountainPoint **points;
	List<int> *navPath;

	int prevNavPathCount;
	int prevNavPathSteepValue;

	int totalPathsAnalyzed;
	int standaloneElevationPointCount;  
	// this is the count of elevation points which have to be treated as starting points
	// and can't be reached otherwise;  Also from these points, you can make atleast one move.

	std::stringstream resultBuffer;
	//streambuf resultBuffer;

	void StorePathIfValid(int currentValue, int tr, int tc, MountainPoint *);
	void Dump(MountainPoint **);
	void ProcessChild(MountainPoint *);
	void ProcessGrandChildren(MountainPoint *);
	void PersistNavPath(List<int> *navPath, int pathCount, int elevationDropValue);
	void ReadyNavPath(List<int> *navPath);
	void UnWindNavPath(List<int> *navPath, MountainPoint *);

	void CreateEmptyElevationPointsArray();



public:
	__declspec(dllexport) RouteGrid(int **data, int size);
	__declspec(dllexport) bool ValidateData(int lowestValue, int highestValue);
	__declspec(dllexport) void PrintData();

	__declspec(dllexport) void CreateList();
	__declspec(dllexport) void DenormalizePaths();

	__declspec(dllexport) void DeleteList();

protected:
};
