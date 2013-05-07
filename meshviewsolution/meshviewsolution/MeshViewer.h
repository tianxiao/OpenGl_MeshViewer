#pragma once
#include "glutoperatorn.h"
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

class MeshViewer :
	public GlutOperatorN
{
public:
	MeshViewer(const char *title, int width, int height);
	~MeshViewer(void);

	virtual bool OpenMesh(const char* fn);

	void UpdateFaceIndices();

	virtual void Draw(const std::string &dm);

protected:
	typedef OpenMesh::TriMesh_ArrayKernelT<> txMesh;

protected:
	txMesh m_Mesh;
	std::vector<unsigned int> m_Indices;
};

