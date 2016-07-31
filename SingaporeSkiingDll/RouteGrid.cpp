#include "stdafx.h"
#include "RouteGrid.h"
#include <iostream>
#include "exception"
#include "coutRedirect.h"
using namespace std;

RouteGrid::RouteGrid(int **data, int size)
{
	if (data == NULL)
	{
		throw new std::exception("Invalid data array");
	}

	this->data = data;
	this->gridSize = size;

	this->points = NULL;
	this->navPath = NULL;

	this->totalPathsAnalyzed = 0;
	this->standaloneElevationPointCount = 0;
}

bool RouteGrid::ValidateData(int lowestValue, int highestValue)
{
	// check that all values are in range
	for(int i = 0; i <this->gridSize; i++)
	{
		for(int j = 0; j < this->gridSize; j++)
		{
			int v = this->data[i][j];

			if (v < lowestValue || v > highestValue)
			{
				return false;
			}
		}
	}

	return true;
}

void RouteGrid::PrintData()
{
	for(int i = 0; i <this->gridSize; i++)
	{
		for(int j = 0; j < this->gridSize; j++)
		{
			int v = this->data[i][j];
			printf("%d ", v);
		}
		printf("\n");
	}
}

void RouteGrid::CreateEmptyElevationPointsArray()
{
	int arraySize = this->gridSize * this->gridSize;
	this->points = new MountainPoint*[arraySize];

	for (int r = 0; r < this->gridSize; r++)
	{
		for (int c = 0; c < this->gridSize; c++)
		{
			int runningIndex = (r*gridSize) + c;
			this->points[runningIndex] = MountainPoint::CreateMountainPoint(0, r, c, runningIndex, false);
		}
	}
}

void RouteGrid::CreateList()
{
	int arraySize = gridSize*gridSize;
	this->CreateEmptyElevationPointsArray();

	for(int i = 0; i < this->gridSize; i++)
	{
		for(int j = 0; j < this->gridSize; j++)
		{
			int runningIndex = (i*gridSize)+j;

			// get object from array - as it has been already created;
			MountainPoint *mp = this->points[runningIndex];
			mp->elevation = this->data[i][j];

			// Store possible moves from current elevation point
			StorePathIfValid(mp, i-1, j);  // north
			StorePathIfValid(mp, i+1, j);  // south
			StorePathIfValid(mp, i, j-1);  // east
			StorePathIfValid(mp, i, j+1);  // west
		}
	}

	// Debug code
	//int reachableNodes = 0;
	//for (int i = 0; i < arraySize; i++)
	//{
	//	if (this->points[i]->IsReachableFromHigherElevation)
	//	{
	//		reachableNodes++;
	//	}
	//}
	//cout << "Nodes Reachable from higher elevation and ignored = " << reachableNodes << endl;
}

void RouteGrid::StorePathIfValid(MountainPoint *mp, int tr, int tc)
{
	if (tr < 0 || tc < 0 || tr >= gridSize || tc >= gridSize)
	{
		return;
	}

	int toElevation = this->data[tr][tc];
	// can only move to a point with lower elevation than current elevation value
	if (toElevation < mp->elevation)
	{
		// calculate running index of toElevation
		int runningIndex = (tr*gridSize)+tc;
		mp->StorePath(runningIndex);

		// set IsReachableFromHigherElevation to true
		// for elevation point indicated by tr, tc, as it can be reached from mp
		points[runningIndex]->IsReachableFromHigherElevation = true;
	}
}

void RouteGrid::DenormalizePaths()
{
	//this->Dump(this->points);
	//cout << "================================\n";

	// this is the navigation path to store the paths available
	this->navPath = new List<int>();

	this->prevNavPathCount = 0;
	this->prevNavPathSteepValue = 0;
	
	for(int i = 0; i < (gridSize*gridSize); i++)
	{
		// if the current point can be reached from some higher elevation, 
		// we don't want to process current point, as it is already part of some other elevation point
		// thus part of a longer path already.
		// Also if no paths from current point - continue
		if (this->points[i]->IsReachableFromHigherElevation == false)
		{
			this->standaloneElevationPointCount++;
			this->ProcessElevationPoint(this->points[i]);
			this->ReadySkiPath(this->navPath);
			this->navPath->EmptyList();
		}
	}

	delete this->navPath;
	this->navPath = NULL;
	cout << "===========================================================" << endl;
	cout << "  Total " << this->totalPathsAnalyzed << " paths Analyzed=" << " from " << this->standaloneElevationPointCount << " elevation points." << endl;
	cout << "===========================================================" << endl;

	// this will output the final / result path 
	// PersistSkiPath() writes to the buffer at resultBuffer
	cout << this->resultBuffer.str();
}

void RouteGrid::ProcessElevationPoint(MountainPoint *elevationPoint)
{
	this->navPath->Add(elevationPoint->runningIndex);

	// no paths from current point - return;
	int pathCount = elevationPoint->GetPathCount();
	if (pathCount > 0)
	{
		int *path = elevationPoint->paths;
		for (int x = 0; x < MAX_PATHS_FROM_ELEVATION && path[x] >= 0; x++)
		{
			this->ProcessElevationPoint(this->points[path[x]]);
			if (x + 1 < pathCount) // more path exist;
			{
				// dump navPaths and then rewind upto current point
				this->ReadySkiPath(this->navPath);
				this->UnWindNavPath(this->navPath, elevationPoint);
			}
		}
	}
}

void RouteGrid::ReadySkiPath(List<int> *navPath)
{
	this->totalPathsAnalyzed++;

	int pathCount = navPath->Count();
	int elevationDropValue = points[navPath->GetItem()]->elevation - points[navPath->GetLast()->GetItem()]->elevation;

	// path is found, now apply business rules before the path is accepted/overrides 
	// previously found paths.
	if (pathCount == 0 || pathCount < this->prevNavPathCount)
	{
		return;
	}

	if (pathCount > this->prevNavPathCount || 
		(pathCount == this->prevNavPathCount && elevationDropValue > this->prevNavPathSteepValue))
	{
		this->PersistSkiPath(navPath, pathCount, elevationDropValue);
		this->prevNavPathCount = pathCount;
		this->prevNavPathSteepValue = elevationDropValue;
	}
}

// write path to resultBuffer by clearing any previous contents of the buffer
// this way, resultBuffer always has latest and greatest ski path;
void RouteGrid::PersistSkiPath(List<int> *navPath, int pathCount, int elevationDropValue)
{
	if (navPath->Count() == 0)
	{
		return;
	}

	// clear previous contents of result buffer
	this->resultBuffer.clear();
	this->resultBuffer.str(std::string());

	// this will redirect cout to buffer and on exit of (), will reset cout to as before
	// cout_redirect coutredir(this->resultBuffer.rdbuf());

	this->resultBuffer << "   *** Path Count = " << pathCount << "; Elevation Drop = " << elevationDropValue << "; ***" << endl;
	List<int> *p = navPath;
	while(p)
	{
		MountainPoint *mp = this->points[p->GetItem()];
		this->resultBuffer << "[" << mp->actualRow << "," << mp->actualCol << "]" << mp->elevation;
		//this->resultBuffer << "[" << mp->actualRow << "," << mp->actualCol << "]" << mp->runningIndex;
		p=p->GetNext();

		if (p)
		{
			this->resultBuffer << "-";
		}
	}

	this->resultBuffer << endl << endl ;
}

void RouteGrid::UnWindNavPath(List<int> *navPath, MountainPoint *mp)
{
	if (navPath->Count() == 0)
	{
		return;
	}

	while(navPath)
	{
		if (navPath->GetItem() == mp->runningIndex)
		{
			navPath->TerminateList();
		}

		navPath=navPath->GetNext();
	}
}

void RouteGrid::Dump(MountainPoint *mountainPoints[])
{
	for(int i = 0; i < (gridSize*gridSize); i++)
	{
		MountainPoint *currentPoint = mountainPoints[i];
		int pathCount = currentPoint->GetPathCount();
		cout << "  " << currentPoint->runningIndex;
		
		if (pathCount <= 0)
		{
			cout << " [ no paths ]";
		}
		else
		{
			cout << " [ paths-" << pathCount;
			cout << " ( ";
			//List<int> *path = currentPoint->paths;
			int *pathArray = currentPoint->paths;
			for(int x=0; x < 4 && pathArray[x] >= 0; x++)
			{
				cout << pathArray[x] << " ";
			}
			cout << ") ]";
		}

		cout << "\n";
	}
}

RouteGrid::~RouteGrid()
{
	if (this->points != NULL)
	{
		int arraySize = gridSize*gridSize;

		for (int i = 0; i < arraySize; i++)
		{
			delete this->points[i];
			this->points[i] = NULL;
		}

		delete this->points;
		this->points = NULL;
	}

	this->data = NULL;
}
