// meshreconstruct.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "txMeshReconstruct.h"
#include "trTriangleUtil.h"
#include "txGridSurface.h"


bool MySearchCallBack(int id, void *arg)
{
	printf("Hit data rect %d \n", id);
	return true;
}

void test1() {

	txMeshReconstruct mr;
	mr.OpenMesh("delaunay10000.off");
	mr.WriteMesh("Scaleddelaunay1000.off");

	txMesh::Point minbbox = mr.GetMinBBox();
	txMesh::Point maxbbox = mr.GetMaxBBox();
	// test
	RTree<int, float, 2> &rTree = mr.GetRTree();
	
	// following code are only for testing!
	//minbbox[0] = -2.5;
	//minbbox[1] = -4.5;
	//maxbbox[0] = 2.5;
	//maxbbox[1] = 4.5;

	//float min[2] = {0.0,0.0};
	//float max[2] = {min[0],min[1]};
	//int nhits = rTree.Search(min,max,MySearchCallBack,NULL);

	txGridSurface gridsurf(minbbox[0],minbbox[1],maxbbox[0],maxbbox[1],30,60);
	gridsurf.NormalizeTriSurf(&mr);
	gridsurf.ConstructShapeMesh();
	gridsurf.DumpMeshToOFF("Grid10X10.off");
}

void test2() {

	txMeshReconstruct mr;
	mr.OpenMesh("diDelaunay.off");
	mr.WriteMesh("ScaleddiDelaunay.off");

	txMesh::Point minbbox = mr.GetMinBBox();
	txMesh::Point maxbbox = mr.GetMaxBBox();
	
	txGridSurface gridsurf(minbbox[0],minbbox[1],maxbbox[0],maxbbox[1],30,60);
	gridsurf.NormalizeTriSurf(&mr);
	gridsurf.ConstructShapeMesh();
	gridsurf.DumpMeshToOFF("GriddiDelaunay.off");
}


int _tmain(int argc, _TCHAR* argv[])
{
	test1();

	test2();

	return 0;
}

