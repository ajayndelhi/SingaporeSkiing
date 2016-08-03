#include "stdafx.h"
#include "SkiResort.h"
#include <iostream>
#include "exception"
#include "coutRedirect.h"
#include "SkiHop.h"
#include <vector>
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
}

SkiResort::~SkiResort()
{
	this->data = NULL;
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
	this->prevNavPathCount = 0;
	this->prevNavPathSteepValue = 0;

	this->skiPathVector.clear();

	this->CachedNodes.clear();
	this->CachedNodes.resize(this->gridRows * this->gridCols);

	SkiHop hop;

	//int itemsProcessed = 0;

	for(int r = 0; r < gridRows; r++)
	{
		for(int c=0; c < gridCols; c++)
		{
			//itemsProcessed++;

			//if (itemsProcessed % 50000 == 0)
			//{
			//	cout << SkiHelper::CurrentDateTime() << " " << itemsProcessed << " items processed. (cache size: " << this->CachedNodes.size() << ")" << endl;
			//}

			hop.rowIndex = r;
			hop.colIndex = c;
			hop.elevation = this->data[r][c];;

			//std::cout << hop;

			if (this->IsReachableFromSorroundings(&hop) == false)
			{
				this->ProcessElevationPoint(&hop);
				this->ReadySkiPath();

				// note we can't delete hop as it is on stack
				// hence only unwind all nodes after hop
				this->UnWindNavPath(&hop);  
				this->skiPathVector.clear();
			}
		}
	}

	cout << "===========================================================" << endl;
	cout << "  Total " << this->totalPathsAnalyzed << " paths Analyzed" << endl;
	cout << "===========================================================" << endl;

	// this will output the final / result path 
	// PersistSkiPath() writes to the buffer at resultBuffer
	cout << this->resultBuffer.str();

	this->ClearCachedNodes();
}

void SkiResort::ProcessElevationPoint(const SkiHop * const hop)
{
	// we store ski route in this
	this->skiPathVector.push_back(hop);

	vector<SkiHop*> availableHops;
	int pathCount = this->FindAvailableHops(hop, availableHops);

	// if no paths from current point - return;
	for (int x = 0; x < pathCount; x++)
	{
		this->ProcessElevationPoint(availableHops[x]);
		if (x + 1 < pathCount) // more path exist;
		{
			// dump navPaths and then rewind upto current point
			this->ReadySkiPath();
			this->UnWindNavPath(hop);
		}
	}
}

void SkiResort::ReadySkiPath()
{
	int pathCount = this->skiPathVector.size();
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
	if (pathCount < this->prevNavPathCount)
	{
		return;
	}

	short elevationDropValue = this->CalculateElevationDrop(this->skiPathVector[0], this->skiPathVector[pathCount-1]);

	// compare with best values so far - if current values beat best so far, take current and move on.
	if (pathCount > this->prevNavPathCount || 
		(pathCount == this->prevNavPathCount && elevationDropValue > this->prevNavPathSteepValue))
	{
		this->PersistSkiPath();
		// save best values so far
		this->prevNavPathCount = pathCount;
		this->prevNavPathSteepValue = elevationDropValue;
	}
}

// write path to resultBuffer by clearing any previous contents of the buffer
// this way, resultBuffer always has latest and greatest ski path;
void SkiResort::PersistSkiPath()
{
	int pathCount = this->skiPathVector.size();
	short elevationDropValue = this->CalculateElevationDrop(this->skiPathVector[0], this->skiPathVector[pathCount-1]);

	if (pathCount == 0)
	{
		return;
	}

	// clear previous contents of result buffer
	this->resultBuffer.clear();
	this->resultBuffer.str(std::string());

	// this will redirect cout to buffer and on exit of (), will reset cout to as before
	// cout_redirect coutredir(this->resultBuffer.rdbuf());

	this->resultBuffer << "   *** Path Count = " << pathCount << "; Elevation Drop = " << elevationDropValue << "; ***" << endl;

	for(int i = 0; i<pathCount;i++)
	{
		const SkiHop *mp =  this->skiPathVector[i];
		//this->resultBuffer << "[" << mp->rowIndex << "," << mp->colIndex << "]" << mp->elevation;
		this->resultBuffer << mp->elevation;
		if (i+1 < pathCount)
		{
			this->resultBuffer << "-";
		}
	}

	this->resultBuffer << endl << endl ;
}

void SkiResort::DebugSkiPath()
{
	int pathCount = this->skiPathVector.size();

	if (pathCount == 0)
	{
		return;
	}

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
void SkiResort::UnWindNavPath(const SkiHop *hop)
{
	int pathCount = this->skiPathVector.size();
	for(int x = pathCount-1; x >=0; x--)
	{
		const SkiHop *ptr = this->skiPathVector[x];
		if(ptr->rowIndex == hop->rowIndex &&
			ptr->colIndex == hop->colIndex)
		{
			break;
		}

		// don't delete the node, as it is on this->cachedNodes vector 
		// and will be deleted at the end;
		//const SkiHop *toDel = this->skiPathVector[x];
		//delete toDel;
		//toDel = NULL;

		this->skiPathVector.pop_back();
	}
}

int SkiResort::FindAvailableHops(const SkiHop *currentPoint, vector<SkiHop *> &possibleHops)
{
	int targetRow, targetCol;
	int edgeCount = 0;

	// north
    targetRow = currentPoint->rowIndex-1;
	targetCol = currentPoint->colIndex;
	if (this->IsMovePossible(currentPoint, targetRow, targetCol))
	{
		possibleHops.push_back(this->CreateSkiHop(targetRow, targetCol));
		edgeCount++;
	}

	targetRow = currentPoint->rowIndex+1;
	targetCol = currentPoint->colIndex;
	if (this->IsMovePossible(currentPoint, targetRow, targetCol))
	{
		possibleHops.push_back(this->CreateSkiHop(targetRow, targetCol));
		edgeCount++;
	}

	targetRow = currentPoint->rowIndex;
	targetCol = currentPoint->colIndex-1;
	if (this->IsMovePossible(currentPoint, targetRow, targetCol))
	{
		possibleHops.push_back(this->CreateSkiHop(targetRow, targetCol));
		edgeCount++;
	}

	targetRow = currentPoint->rowIndex;
	targetCol = currentPoint->colIndex+1;
	if (this->IsMovePossible(currentPoint, targetRow, targetCol))
	{
		possibleHops.push_back(this->CreateSkiHop(targetRow, targetCol));
		edgeCount++;
	}

	return edgeCount;
}

// method to find if point is reachable from higher points - if so, we don't want to
// process it separately, as it is part of higher points and will be covered from higher points;
bool SkiResort::IsReachableFromSorroundings(const SkiHop *currentPoint)
{
	int targetRow, targetCol;

	// north
    targetRow = currentPoint->rowIndex-1;
	targetCol = currentPoint->colIndex;
	if (this->IsReachable(currentPoint, targetRow, targetCol))
	{
		return true;
	}

	targetRow = currentPoint->rowIndex+1;
	targetCol = currentPoint->colIndex;
	if (this->IsReachable(currentPoint, targetRow, targetCol))
	{
		return true;
	}

	targetRow = currentPoint->rowIndex;
	targetCol = currentPoint->colIndex-1;
	if (this->IsReachable(currentPoint, targetRow, targetCol))
	{
		return true;
	}

	targetRow = currentPoint->rowIndex;
	targetCol = currentPoint->colIndex+1;
	if (this->IsReachable(currentPoint, targetRow, targetCol))
	{
		return true;
	}

	return false;
}

SkiHop* SkiResort::CreateSkiHop(int r, int c)
{
	int runningIndex = r * this->gridCols + c;

	// first check if node exists on cached nodes
	SkiHop * ptr = this->CachedNodes[runningIndex];
	if (ptr && ptr->rowIndex == r && ptr->colIndex == c)
	{
		//cout << "Node retrieved from cache...[" << r << "][" << c << "]==" << this->CachedNodes[i]->elevation << endl;
		return ptr;
	}

	// if not found, create one and cache it for future use
	SkiHop *hop = SkiHop::Create(r, c, this->data[r][c]);
	this->CachedNodes[runningIndex] = hop;
	return hop;
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
	for(int x = 0; x< this->CachedNodes.size(); x++)
	{
		const SkiHop *toDel = this->CachedNodes[x];
		if (toDel)
		{
			delete toDel;
			toDel = NULL;
			nodesDeleted++;
		}
	}

	this->CachedNodes.clear();

	cout << "( " << nodesDeleted << " nodes deleted from cache. )" << endl;
}