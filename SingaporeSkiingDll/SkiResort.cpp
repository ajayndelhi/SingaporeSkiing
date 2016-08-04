#include "stdafx.h"
#include "SkiResort.h"
#include <iostream>
#include "exception"
#include "SkiHop.h"
#include "SkiHelper.h"
using namespace std;

SkiResort::SkiResort(short **data, int rows, int cols)
{
	if (data == NULL)
	{
		throw new std::exception("Invalid data array");
	}

	this->data = data;
	this->gridRows = rows;
	this->gridCols = cols;
	this->totalPathsAnalyzed = 0;

	this->CreateCachedNodesArray();

	this->bestSkiPathCount = 0;
	this->bestSkiPathSteepValue = 0;
	for (int i = 0; i < MAX_SKI_PATH_SIZE; i++)
	{
		this->bestSkiPath[i] = 0;
	}
}

void SkiResort::CreateCachedNodesArray()
{
	int arraySize = this->gridRows * this->gridCols;
	this->CachedNodes = new SkiHop*[arraySize];
	for (int i = 0; i < arraySize; i++)
	{
		this->CachedNodes[i] = NULL;
	}
}

SkiResort::~SkiResort()
{
	this->data = NULL;
	this->ClearCachedNodes();
}

bool SkiResort::ValidateData(short lowestValue, short highestValue)
{
	// check that all values are in range
	for(int i = 0; i <this->gridRows; i++)
	{
		for(int j = 0; j < this->gridCols; j++)
		{
			short v = this->data[i][j];

			if (v < lowestValue || v > highestValue)
			{
				return false;
			}
		}
	}

	return true;
}

void SkiResort::FindBestRoute()
{
	this->bestSkiPathCount = 0;
	this->bestSkiPathSteepValue = 0;
	this->skiPathVectorIndex = 0;

	SkiHop hop;
	for(int r = 0; r < gridRows; r++)
	{
		for(int c=0; c < gridCols; c++)
		{
			hop.rowIndex = r;
			hop.colIndex = c;
			hop.elevation = this->data[r][c];
			hop.maxPathCount = -1;

			if (this->IsReachableFromSorroundings(&hop) == false)
			{
				SkiHop *ptr = SkiHop::Create(r, c, this->data[r][c]);
				this->CachedNodes[this->GetRunningIndex(r,c)] = ptr;

				this->ProcessElevationPoint(ptr);
				this->ReadySkiPath();
				this->skiPathVectorIndex = 0;
			}
		}
	}

	cout << "===========================================================" << endl;
	cout << "  Total " << this->totalPathsAnalyzed << " paths Analyzed" << endl;
	cout << "===========================================================" << endl;
}

void SkiResort::ProcessElevationPoint(SkiHop * const hop)
{
	if (this->IsNavigatingMakeSense(hop) == false)
	{
		return;
	}

	// we store ski route in this
	if (this->skiPathVectorIndex < MAX_SKI_PATH_SIZE - 1)
	{
		this->skiPathVector[this->skiPathVectorIndex++] = hop;
	}
	else
	{
		cout << "Number of Ski Paths exceeded its max size." << endl;
		throw new std::exception("Number of Ski Paths exceeded its max size.");
	}

	// there can be max 4 hops
	SkiHop *availableHops[MAX_PATHS_FROM_ELEVATION];
	int pathCount = this->FindAvailableHops(hop, availableHops);

	// cache skiPathVectorIndex here so as to rewind into the skiPathVector
	int cachedSkiPathVectorIndex = this->skiPathVectorIndex;

	// if no paths from current point - return;
	for (int x = 0; x < pathCount; x++)
	{
		this->ProcessElevationPoint(availableHops[x]);
		if (x + 1 < pathCount) // more path exist;
		{
			// dump navPaths and then rewind upto current point
			this->ReadySkiPath();
			this->UnWindNavPath(hop, cachedSkiPathVectorIndex);
		}
	}
}

void SkiResort::ReadySkiPath()
{
	int pathCount = this->skiPathVectorIndex;
	// path is found, now apply business rules before the path is accepted/overrides 
	// previously found paths.

	// ignore if there is no path from a elevation point
	if (pathCount < 2)
	{
		return;
	}

	this->totalPathsAnalyzed++;
	//this->DebugSkiPath();

	// path is found, now apply business rules before the path is accepted/overrides 
	// previously found paths.
	if (pathCount < this->bestSkiPathCount)
	{
		return;
	}

	short elevationDropValue = this->CalculateElevationDrop(this->skiPathVector[0], this->skiPathVector[pathCount-1]);

	// compare with best values so far - if current values beat best so far, take current and move on.
	if (pathCount > this->bestSkiPathCount || 
		(pathCount == this->bestSkiPathCount && elevationDropValue > this->bestSkiPathSteepValue))
	{
		this->PersistSkiPath();
		// save best values so far
		this->bestSkiPathCount = pathCount;
		this->bestSkiPathSteepValue = elevationDropValue;
	}
}

void SkiResort::PersistSkiPath()
{
	if (this->skiPathVectorIndex == 0)
	{
		return;
	}

	this->bestSkiPathCount = this->skiPathVectorIndex;
	this->bestSkiPathSteepValue = this->CalculateElevationDrop(this->skiPathVector[0], this->skiPathVector[this->bestSkiPathCount - 1]);

	for (int i = 0; i<this->bestSkiPathCount; i++)
	{
		const SkiHop *mp = this->skiPathVector[i];
		this->bestSkiPath[i] = this->GetRunningIndex(mp->rowIndex, mp->colIndex);
	}
}

// write path to resultBuffer by clearing any previous contents of the buffer
// this way, resultBuffer always has latest and greatest ski path;
void SkiResort::PrintBestSkiPath(ostream &os)
{
	if (this->bestSkiPathCount == 0)
	{
		os << "No Path found";
		return;
	}

	int pathCount = this->bestSkiPathCount;
	short elevationDropValue = this->bestSkiPathSteepValue;

	os << "   *** Path Count = " << pathCount << "; Elevation Drop = " << elevationDropValue << "; ***" << endl;

	for(int i = 0; i<pathCount;i++)
	{
		const SkiHop *mp = this->CachedNodes[this->bestSkiPath[i]];
		//this->resultBuffer << "[" << mp->rowIndex << "," << mp->colIndex << "]" << mp->elevation;
		if (mp)
		{
			os << mp->elevation;
			if (i + 1 < pathCount)
			{
				os << "-";
			}
		}
	}

	os << endl << endl ;
}

void SkiResort::DebugSkiPath()
{
	int pathCount = this->skiPathVectorIndex;

	for(int i = 0; i<pathCount;i++)
	{
		const SkiHop *mp =  this->skiPathVector[i];
		cout << mp->elevation;
		if (i+1 < pathCount)
		{
			cout << "-";
		}
	}

	cout << endl ;
}

short SkiResort::CalculateElevationDrop(const SkiHop *firstPoint, const SkiHop *lastPoint)
{
	return this->data[firstPoint->rowIndex][firstPoint->colIndex] - 
							this->data[lastPoint->rowIndex][lastPoint->colIndex];
}

// unwind upto hop; (keep hop)
// while unwinding, also store the hop points available (max)
// from hop
void SkiResort::UnWindNavPath(SkiHop *hop, int cachedSkiPathVectorIndex)
{
	short currentPathCountFromHop = this->skiPathVectorIndex - cachedSkiPathVectorIndex + 1;
	if (currentPathCountFromHop > hop->maxPathCount)
	{
		hop->maxPathCount = currentPathCountFromHop;
	}
	this->skiPathVectorIndex = cachedSkiPathVectorIndex;
}

int SkiResort::FindAvailableHops(const SkiHop *currentPoint, SkiHop *possibleHops[MAX_PATHS_FROM_ELEVATION])
{
	int targetRow, targetCol;
	int edgeCount = 0;

	int rowIndexes[] = { 
		currentPoint->rowIndex - 1,  // north
		currentPoint->rowIndex + 1,  // south
		currentPoint->rowIndex,      // east
		currentPoint->rowIndex       // west
	};

	int colIndexes[] = {
		currentPoint->colIndex,          // north
		currentPoint->colIndex,          // south
		currentPoint->colIndex - 1,      // east
		currentPoint->colIndex + 1       // west
	};

	for (int i = 0; i < MAX_PATHS_FROM_ELEVATION; i++)
	{
		targetRow = rowIndexes[i];
		targetCol = colIndexes[i];
		if (this->IsMovePossible(currentPoint, targetRow, targetCol))
		{
			possibleHops[edgeCount] = this->CreateSkiHop(targetRow, targetCol);
			edgeCount++;
		}
	}

	return edgeCount;
}

// method to find if point is reachable from higher points - if so, we don't want to
// process it separately, as it is part of higher points and will be covered from higher points;
bool SkiResort::IsReachableFromSorroundings(const SkiHop *currentPoint)
{
	int rowIndexes[] = {
		currentPoint->rowIndex - 1,  // north
		currentPoint->rowIndex + 1,  // south
		currentPoint->rowIndex,      // east
		currentPoint->rowIndex       // west
	};

	int colIndexes[] = {
		currentPoint->colIndex,          // north
		currentPoint->colIndex,          // south
		currentPoint->colIndex - 1,      // east
		currentPoint->colIndex + 1       // west
	};

	for (int i = 0; i < MAX_PATHS_FROM_ELEVATION; i++)
	{
		if (this->IsReachable(currentPoint, rowIndexes[i], colIndexes[i]))
		{
			return true;
		}
	}

	return false;
}

SkiHop* SkiResort::CreateSkiHop(int r, int c)
{
	int runningIndex = this->GetRunningIndex(r,c);

	// first check if node exists on cached nodes
	SkiHop * ptr = this->CachedNodes[runningIndex];
	if (ptr && ptr->rowIndex == r && ptr->colIndex == c)
	{
		;
	}
	else
	{
		// if not found, create one and cache it for future use
		ptr = SkiHop::Create(r, c, this->data[r][c]);
		this->CachedNodes[runningIndex] = ptr;
	}
	return ptr;
}

bool SkiResort::IsMovePossible(const SkiHop *currentPoint, int tr, int tc)
{
	if (tr < 0 || tc < 0 || tr >= gridRows || tc >= gridCols)
	{
		return false;
	}

	int toElevation = this->data[tr][tc];
	// can only move to a point with lower elevation than current elevation value
	return (toElevation < currentPoint->elevation) ? true : false;
}

bool SkiResort::IsReachable(const SkiHop *currentPoint, int tr, int tc)
{
	if (tr < 0 || tc < 0 || tr >= gridRows || tc >= gridCols)
	{
		return false;
	}

	int fromElevation = this->data[tr][tc];

	return (fromElevation > currentPoint->elevation) ? true : false;
}

void SkiResort::ClearCachedNodes()
{
	int nodesDeleted = 0;
	for (int x = 0; x < this->gridRows * this->gridCols; x++)
	{
		const SkiHop *toDel = this->CachedNodes[x];
		if (toDel)
		{
			delete toDel;
			toDel = NULL;
			this->CachedNodes[x] = NULL;
			nodesDeleted++;
		}
	}

	delete[] this->CachedNodes;
	this->CachedNodes = NULL;
	cout << "( " << nodesDeleted << " nodes deleted from cache. )" << endl;
}

// first find out if it make sense to go down current hop path
// we can do this, by calculating hop->maxPathCount + this->skiPathVectorIndex and comparing it
// with this->prevNavPathCount; If going down current path, won't beat previous Nav Path Count
// we don't want to go down current hop point;
bool SkiResort::IsNavigatingMakeSense(const SkiHop *hop)
{
	// we don't know the best path count from hop - so go ahead and explore it.
	// Also, note that if we are checking >=, this is because even if the path
	// count is going to be same, the exploration from hop might beat on elevation drop;
	if (hop->maxPathCount == -1 ||
		this->skiPathVectorIndex + hop->maxPathCount >= this->bestSkiPathCount)
	{
		return true;
	}

	return false;
}

short SkiResort::GetBestSkiPathCount()
{
	return this->bestSkiPathCount;
}
short SkiResort::GetBestSkiPathElevation()
{
	return this->bestSkiPathSteepValue;
}

// caller is responsible for supplying array of right size.
void SkiResort::GetBestSkiPath(short *paths)
{
	if (paths == NULL)
	{
		return;
	}

	for (int i = 0; i < this->bestSkiPathCount; i++)
	{
		const SkiHop *mp = this->CachedNodes[this->bestSkiPath[i]];
		if (mp)
		{
			*(paths + i) = mp->elevation;
		}
	}
}

int SkiResort::GetRunningIndex(int row, int col)
{
	return row * this->gridCols + col;
}