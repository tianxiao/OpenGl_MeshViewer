// RTreetestProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/**
* The R Tree test project
* As I know there several sources of R-Tree implementation
* This template one http://superliminal.com/sources/sources.htm#C%20&%20C++%20Code
* CGAL
* BOOST Geometry
* GRASS GIS http://grass.osgeo.org/download/software/ms-windows/
*   http://www.chorochronos.org/
* 张建磊的日志 
*/


#include <stdio.h>
#include "RTree.h"


struct Rect
{
	Rect() {}

	Rect(int minx, int miny, int maxx, int maxy)
	{
		min[0] = minx;
		min[1] = miny;
		max[0] = maxx;
		max[1] = maxy;
	}

	int min[2];
	int max[2];
};

struct Rect rects[] = 
{
	Rect(0,0,2,2),
	Rect(5,5,7,7),
	Rect(8,5,9,6),
	Rect(7,1,9,2)
};

int nrects = sizeof(rects)/sizeof(rects[0]);

Rect search_rect(6,4,10,6);

bool MySearchCallBack(int id, void *arg)
{
	printf("Hit data rect %d \n", id);
	return true;
}

// case for the float precision
struct txVec3
{
	txVec3() {}

	txVec3(float x, float y, float z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	txVec3 operator+(const txVec3 &a) const
	{
		return txVec3(v[0]+a.v[0], v[1]+a.v[1], v[2]+a.v[2]);
	}

	float v[3];
};

static bool BoxesIntersect(const txVec3 &amin, txVec3 &amax, txVec3 &bmin, txVec3 &bmax)
{
	for (int i=0; i<3; ++i) {
		if (amin.v[i]>bmax.v[i] || amax.v[i]<bmin.v[i]) {
			return false;
		}
	}

	return true;
}

struct txBoundingBox
{
	txVec3 min;
	txVec3 max;
};

struct txRectF
{
	txRectF(float minx, float miny, float maxx, float maxy)
	{
		min[0] = minx;
		min[1] = miny;
		max[0] = maxx;
		max[1] = maxy;
	}

	float min[2];
	float max[2];
};

int _tmain(int argc, _TCHAR* argv[])
{
	RTree<int,int,2,float> tree;
	
	int i, nhits;
	printf("nrects = %d\n", nrects);

	for (i=0; i<nrects; i++) {
		tree.Insert(rects[i].min, rects[i].max,i);
	}

	nhits = tree.Search(search_rect.min, search_rect.max,MySearchCallBack,NULL);

	printf("Search resulted in %d hits\n", nhits);

	int itIndex = 0;
	RTree<int, int, 2, float>::Iterator it;
	for (tree.GetFirst(it);!tree.IsNull(it); tree.GetNext(it)) {
		int value = tree.GetAt(it);

		int boundsMin[2] = {0,0};
		int boundsMax[2] = {0,0};
		it.GetBounds(boundsMin, boundsMax);
		printf("it[%d] %d = (%d,%d,%d,%d)\n", itIndex++, value, boundsMin[0], boundsMax[1], boundsMax[0], boundsMax[1]);
	}

	itIndex = 0;
	tree.GetFirst(it);
	while(!it.IsNull()) {
		int value = *it;
		++it;
		printf("it[%d] %d\n",itIndex++, value);
	}

	// float 3 dimensional test
	RTree<int, float, 3> ftree;

	txBoundingBox bs[4] = 
	{
		{txVec3(0.0,0.0,0.0),txVec3(2.0,2.0,0.0)},
		{txVec3(5.0,5.0,0.0),txVec3(7.0,7.0,0.0)},
		{txVec3(8.0,5.0,0.0),txVec3(9.0,6.0,0.0)},
		{txVec3(7.0,1.0,0.0),txVec3(9.0,2.0,0.0)},
	};

	for ( int i=0; i<4; i++) {
		
		ftree.Insert(bs[i].min.v, bs[i].max.v, i);
	}

	int nfhits = ftree.Search(txVec3(6.0,4.0,0.0).v,txVec3(10.0,6.0,0.0).v,MySearchCallBack,NULL);

	
	// float 2 dimensional float
	RTree<int, float, 2> f2tree;
	
	struct txRectF f2rects[4] = 
	{
		txRectF(0,0,2,2),
		txRectF(5,5,7,7),
		txRectF(8,5,9,6),
		txRectF(7,1,9,2)
	};

	txRectF searchb(6,6,6,6);

	for (int i=0; i<4; i++) {
		f2tree.Insert(f2rects[i].min, f2rects[i].max,i);
	}

	int nf2hits = f2tree.Search(searchb.min, searchb.max,MySearchCallBack,NULL);

	return 0;
}

