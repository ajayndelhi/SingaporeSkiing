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

	//this->resultBuffer = new streambuf();
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

	// initialize array created here
	for(int x = 0; x < arraySize; x++)
	{
		this->points[x] = NULL;
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
			int elevation = this->data[i][j];
			int runningIndex = (i*gridSize)+j;

			// try to get it from array - may be it has been created as part of storing paths for other elevation points
			MountainPoint *mp = this->points[runningIndex];
			if (mp == NULL)
			{
				this->points[runningIndex] = mp = MountainPoint::CreateMountainPoint(elevation, i, j, runningIndex, false);
			}

			// Store possible moves from current elevation point
			StorePathIfValid(elevation, i-1, j, mp);  // north
			StorePathIfValid(elevation, i+1, j, mp);  // south
			StorePathIfValid(elevation, i, j-1, mp);  // east
			StorePathIfValid(elevation, i, j+1, mp);  // west
		}
	}
}

void RouteGrid::StorePathIfValid(int fromElevation, int tr, int tc, MountainPoint *mp)
{
	if (tr < 0 || tc < 0 || tr >= gridSize || tc >= gridSize)
	{
		return;
	}

	int toElevation = this->data[tr][tc];
	// can only move to a point with lower elevation than current elevation value
	if (toElevation < fromElevation)
	{
		// calculate running index of toElevation
		int runningIndex = (tr*gridSize)+tc;
		mp->StorePath(runningIndex);

		// check if the MountainPoint at runningIndex has already been created
		// if yes - set its IsReachable property to true
		// if not - create new MountainPoint and with IsReachable as True

		MountainPoint *mp = points[runningIndex];
		if (mp != NULL)
		{
			mp->IsReachableFromHigherElevation = true;
		}
		else
		{
			points[runningIndex] = MountainPoint::CreateMountainPoint(toElevation,  tr, tc, runningIndex, true);
		}
	}
}

void RouteGrid::DenormalizePaths()
{
	//this->Dump(this->points);
	//cout << "================================\n";

	// this is the navigation path
	this->navPath = new List<int>();

	this->prevNavPathCount = 0;
	this->prevNavPathSteepValue = 0;
	
	for(int i = 0; i < (gridSize*gridSize); i++)
	{
		MountainPoint *currentPoint = this->points[i];

		int *path = currentPoint->paths;

		// if the current point can be reached from some higher elevation, 
		// we don't want to process current point, as it is already part of some other elevation point
		// thus part of a longer path already.
		// Also if no paths from current point - return;
		if (currentPoint->IsReachableFromHigherElevation == false && currentPoint->GetPathCount() > 0)
		{
			this->standaloneElevationPointCount++;
			this->navPath->Add(currentPoint->runningIndex);
			for(int x = 0; x < MAX_PATHS_FROM_ELEVATION && path[x] >= 0; x++)
			{
				this->ProcessChild(this->points[path[x]]);

				// dump navPaths and then rewind upto current point
				this->ReadyNavPath(this->navPath);
				this->UnWindNavPath(navPath, currentPoint);
			}
			navPath->EmptyList();
		}
	}

	delete this->navPath;
	this->navPath = NULL;
	cout << "===========================================================" << endl;
	cout << "  Total " << this->totalPathsAnalyzed << " paths Analyzed=" << " from " << this->standaloneElevationPointCount << " elevation points." << endl;
	cout << "===========================================================" << endl;

	cout << this->resultBuffer.str();

}

void RouteGrid::ProcessChild(MountainPoint *childPoint)
{
	//childPoint->IsVisited = true;
	this->navPath->Add(childPoint->runningIndex);
	this->ProcessGrandChildren(childPoint);
}

void RouteGrid::ProcessGrandChildren(MountainPoint *childPoint)
{
	//List<int> *path = childPoint->paths;
	int *path = childPoint->paths;

	// no paths from current point - return;
	int pathCount = childPoint->GetPathCount();
	if (pathCount <= 0)
	{
		return;
	}

	for(int x=0; x < MAX_PATHS_FROM_ELEVATION && path[x] >=0; x++)
	{
		this->ProcessChild(this->points[path[x]]);
		if (x+1 < pathCount) // more path exist;
		{
			// dump navPaths and then rewind upto current point
			this->ReadyNavPath(this->navPath);
			this->UnWindNavPath(this->navPath, childPoint);
		}
	}
}

void RouteGrid::ReadyNavPath(List<int> *navPath)
{
	this->totalPathsAnalyzed++;


	int pathCount = navPath->Count();
	int elevationDropValue = points[navPath->GetItem()]->elevation - points[navPath->GetLast()->GetItem()]->elevation;

	if (pathCount == 0 || pathCount < this->prevNavPathCount)
	{
		return;
	}

	bool persist = false;
	if (pathCount > this->prevNavPathCount)
	{
		persist = true;
	}

	if(pathCount == this->prevNavPathCount && elevationDropValue > this->prevNavPathSteepValue)
	{
		persist = true;
	}

	if (persist)
	{
		this->PersistNavPath(navPath, pathCount, elevationDropValue);
		this->prevNavPathCount = pathCount;
		this->prevNavPathSteepValue = elevationDropValue;
	}
}

void RouteGrid::PersistNavPath(List<int> *navPath, int pathCount, int elevationDropValue)
{
	if (navPath->Count() == 0)
	{
		return;
	}

	// clear previous contents of result buffer
	this->resultBuffer.str(std::string());

	// this will redirect cout to buffer and on exit of (), will reset cout to as before
	cout_redirect coutredir(this->resultBuffer.rdbuf());

	cout << "   *** Path Count = " << pathCount << "; Elevation Drop = " << elevationDropValue << "; ***" << endl;
	List<int> *p = navPath;
	while(p)
	{
		MountainPoint *mp = this->points[p->GetItem()];
		cout << "[" << mp->actualRow << "," << mp->actualCol << "]" << mp->elevation;
		p=p->GetNext();

		if (p)
		{
			cout << "-";
		}
	}

	cout << endl << endl << endl;
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

void RouteGrid::DeleteList()
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
}
