#include "stdafx.h"
#include "MountainPoint.h"

MountainPoint * MountainPoint::CreateMountainPoint(int elevation, int gridRow, int gridCol, int runningIndex, bool IsReachableFromHigherElevation)
{
	MountainPoint *mp = new MountainPoint();
	mp->elevation = elevation;
	mp->runningIndex = runningIndex;
	mp->IsReachableFromHigherElevation = IsReachableFromHigherElevation;
	mp->actualRow = gridRow;
	mp->actualCol = gridCol;

	return mp;
}

MountainPoint::MountainPoint()
{
	//IsVisited = false;
	IsReachableFromHigherElevation = false;
	for(int j = 0; j < MAX_PATHS_FROM_ELEVATION; j++)
	{
		paths[j] = -1;
	}
}

void MountainPoint::StorePath(int runningIndex)
{
	for(int j = 0; j < MAX_PATHS_FROM_ELEVATION; j++)
	{
		if (paths[j] < 0) // indicates first available slot
		{
			paths[j] = runningIndex;
			break;
		}
	}
}

int MountainPoint::GetPathCount()
{
	int count = 0;
	for(int j = 0; j < MAX_PATHS_FROM_ELEVATION && paths[j] >=0; j++, count++);
	return count;
}
