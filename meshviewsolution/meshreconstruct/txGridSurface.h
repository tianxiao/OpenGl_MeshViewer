#pragma once
#include "txOpenMeshWrapper.h"

#include <vector>

class txMeshReconstruct;

class txGridSurface
{



	typedef struct txBBox {
		float min[2];
		float max[2];
	} txBBox;

public:
	txGridSurface(float minx, float miny, float maxx, float maxy, int gridxsize, int gridysize, bool ccw=true);
	~txGridSurface(void);

	void NormalizeTriSurf(txMeshReconstruct *triSurf);

	// Construct shape mesh
	void ConstructShapeMesh();

	// debug
	void DumpMeshToOFF(char * f);


private:
	void AllocateIndicesVetrsArrayMem();
	void AssembleIndices(std::vector<int> &indices, bool ccw=true);
	void AssignVerts(std::vector<txMesh::Point> &verts);

	void CopyToMesh();

private:
	txMesh m_Mesh;
	std::vector<int> m_Indices;
	std::vector<txMesh::Point> m_Verts;

	txBBox m_BBox;
	int m_GX;
	int m_GY;
	
};

