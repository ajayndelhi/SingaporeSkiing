#include "stdafx.h"
#include <iostream>

using  namespace std;

#ifndef __SkiHop__
#define __SkiHop__
class SkiHop
{
public:

	int rowIndex;
	int colIndex;
	short elevation;

	~SkiHop();

	static SkiHop* Create(int r, int c, short elevation);
	friend std::ostream& operator<<(std::ostream&, const SkiHop &);
};

SkiHop::~SkiHop()
{
	//cout << "object with elevation " << elevation << " being deleted." << endl;
}

SkiHop* SkiHop::Create(int r, int c, short elevation)
{
	SkiHop *hopPoint = new SkiHop();
	hopPoint->rowIndex = r;
	hopPoint->colIndex = c;
	hopPoint->elevation = elevation;

	return hopPoint;
}

std::ostream & operator<<(ostream & co, const SkiHop &sh)
{
	co << "[" << sh.rowIndex << "][" << sh.colIndex << "]==" << sh.elevation << endl;
	return co;
}

#endif