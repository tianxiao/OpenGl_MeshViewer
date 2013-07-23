#pragma once

#include "txOpenMeshWrapper.h"
#include "RTree.h"



struct txRect2F
{
	txRect2F(float minx, float miny, float maxx, float maxy)
	{
		min[0] = minx;
		min[1] = miny;
		max[0] = maxx;
		max[1] = maxy;
	}

	float min[2];
	float max[2];
};

class txMeshReconstruct
{
public:
	txMeshReconstruct(void);
	~txMeshReconstruct(void);


	// The open write mesh is write a functional mesh like the 4 peak surface
	//----------------------------------------------------------------------------
	bool OpenMesh(const char* f);
	bool WriteMesh(const char* f);
	//----------------------------------------------------------------------------

	
	// The following is read the mesh from the OFF and construct its rtree datastruct
	//----------------------------------------------------------------------------
	bool OpenFinalMesh(const char *f);
	//----------------------------------------------------------------------------

	txMesh & GetMesh() { return this->m_Mesh; } 
	RTree<int, float, 2> & GetRTree() { return this->m_RTree; }

	txMesh::Point GetMinBBox() { return this->bbMin; }
	txMesh::Point GetMaxBBox() { return this->bbMax; }

private:
	void ConstructRTree();
	void InterpretGridPoint();

private:
	

private:
	txMesh m_Mesh;

	// R Tree for triangle mesh search
	RTree<int, float, 2> m_RTree;

	txMesh::Point bbMin, bbMax;
};

