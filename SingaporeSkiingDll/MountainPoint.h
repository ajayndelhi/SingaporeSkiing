#include "stdafx.h"
#define MAX_PATHS_FROM_ELEVATION 4
class MountainPoint
{
public:
	int runningIndex;
	short elevation;
	
	// there can be max 4 paths from one point -
	// north/south/east/west
	int paths[MAX_PATHS_FROM_ELEVATION];

	short actualRow;
	short actualCol;

	bool IsReachableFromHigherElevation;

	MountainPoint();
	void StorePath(int value);
	int GetPathCount();

	static MountainPoint *CreateMountainPoint(int elevation, int gridRow, int gridCol, int runningIndex, bool IsReachableFromHigherElevation);
};

