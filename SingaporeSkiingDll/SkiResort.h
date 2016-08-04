#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include "SkiHop.h"

using namespace std;
#define MAX_PATHS_FROM_ELEVATION 4

// setting some practical limit for performance
#define MAX_SKI_PATH_SIZE 100
class SkiResort
{
private:
	short** data;
	int gridRows;
	int gridCols;

	// using array here to store the paths - 
	// this though sets a limit, but it is much better performance wise
	// when compared to using vector here;
	const SkiHop *skiPathVector[MAX_SKI_PATH_SIZE];
	short skiPathVectorIndex;

	// this vector caches nodes for optimization
	//vector<SkiHop *> CachedNodes;
	// moving away from vector
	SkiHop **CachedNodes;

	// Captures the best ski path
	short bestSkiPathCount;
	short bestSkiPathSteepValue;
	// this array stores the index value in CachedNodes;
	int bestSkiPath[MAX_SKI_PATH_SIZE];

	int totalPathsAnalyzed;

	void ProcessElevationPoint(SkiHop * const hop);
	
	void PersistSkiPath();
	void ReadySkiPath();
	void UnWindNavPath(SkiHop *, int cachedSkiPathVectorIndex);

	int FindAvailableHops(const SkiHop *, SkiHop *availableHops[MAX_PATHS_FROM_ELEVATION]);
	bool IsMovePossible(const SkiHop *currentPoint, int tr, int tc);
	bool IsReachableFromSorroundings(const SkiHop *currentPoint);
	bool IsReachable(const SkiHop *currentPoint, int tr, int tc);
	SkiHop* CreateSkiHop(int r, int c);
	short CalculateElevationDrop(const SkiHop *first, const SkiHop *last);
	void DebugSkiPath();
	void CreateCachedNodesArray();
	void ClearCachedNodes();
	bool IsNavigatingMakeSense(const SkiHop *);
	int GetRunningIndex(int row, int col);
public:
	__declspec(dllexport) SkiResort(short **data, int rows, int cols);
	__declspec(dllexport) ~SkiResort();
	__declspec(dllexport) bool ValidateData(short lowestValue, short highestValue);
	__declspec(dllexport) void FindBestRoute();
	__declspec(dllexport) void PrintBestSkiPath(ostream &);

	// methods added for unit testing
	__declspec(dllexport) short GetBestSkiPathCount();
	__declspec(dllexport) short GetBestSkiPathElevation();
	__declspec(dllexport) void GetBestSkiPath(short *paths);
protected:
};
