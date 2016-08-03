#include "stdafx.h"
#include "List.h"
#include <iostream>
#include <sstream>
#include <string>
#include "SkiHop.h"
#include <vector>

using namespace std;

class SkiResort
{
private:
	short** data;
	int gridRows;
	int gridCols;

	vector<const SkiHop *> skiPathVector;

	// this vector caches nodes for optimization
	vector<SkiHop *> CachedNodes;

	int prevNavPathCount;
	short prevNavPathSteepValue;

	int totalPathsAnalyzed;

	std::stringstream resultBuffer;

	void ProcessElevationPoint(const SkiHop * const hop);
	void PersistSkiPath();
	void ReadySkiPath();
	void UnWindNavPath(const SkiHop *);

	int FindAvailableHops(const SkiHop *, vector<SkiHop*> &);
	bool IsMovePossible(const SkiHop *currentPoint, int tr, int tc);
	bool IsReachableFromSorroundings(const SkiHop *currentPoint);
	bool IsReachable(const SkiHop *currentPoint, int tr, int tc);
	SkiHop* CreateSkiHop(int r, int c);
	short CalculateElevationDrop(const SkiHop *first, const SkiHop *last);
	void DebugSkiPath();
	void ClearCachedNodes();
public:
	__declspec(dllexport) SkiResort(short **data, int rows, int cols);
	__declspec(dllexport) ~SkiResort();
	__declspec(dllexport) bool ValidateData(short lowestValue, short highestValue);
	__declspec(dllexport) void FindBestRoute();
protected:
};
