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
	short** data;
	int gridSize;

	MountainPoint **points;
	List<int> *navPath;

	int prevNavPathCount;
	short prevNavPathSteepValue;

	int totalPathsAnalyzed;
	int standaloneElevationPointCount;  
	// this is the count of elevation points which have to be treated as starting points
	// and can't be reached otherwise;  Also from these points, you can make atleast one move.

	std::stringstream resultBuffer;

	void StorePathIfValid(MountainPoint *, int tr, int tc);
	void Dump(MountainPoint **);
	void ProcessElevationPoint(MountainPoint *);
	void PersistSkiPath(List<int> *navPath, int pathCount, int elevationDropValue);
	void ReadySkiPath(List<int> *navPath);
	void UnWindNavPath(List<int> *navPath, MountainPoint *);

	void CreateEmptyElevationPointsArray();
public:
	__declspec(dllexport) RouteGrid(short **data, int size);

	__declspec(dllexport) bool ValidateData(short lowestValue, short highestValue);
	__declspec(dllexport) void PrintData();

	__declspec(dllexport) void CreateList();
	__declspec(dllexport) void DenormalizePaths();

	__declspec(dllexport) ~RouteGrid();
protected:
};
