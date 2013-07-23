#include "StdAfx.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "txGridSurface.h"
#include "txMeshReconstruct.h"
#include "trTriangleUtil.h"



txGridSurface::txGridSurface(float minx, float miny, float maxx, float maxy, int gridxsize, int gridysize, bool ccw/*=true*/)
{
	this->m_BBox.min[0] = minx;
	this->m_BBox.min[1] = miny;
	this->m_BBox.max[0] = maxx;
	this->m_BBox.max[1] = maxy;

	this->m_GX = gridxsize;
	this->m_GY = gridysize;

	AllocateIndicesVetrsArrayMem();
	AssembleIndices(m_Indices,ccw);
	AssignVerts(m_Verts);
	
}


txGridSurface::~txGridSurface(void)
{
}

void txGridSurface::AllocateIndicesVetrsArrayMem()
{
	this->m_Indices.reserve( (m_GX-1)*(m_GY-1)*2*3);
	this->m_Verts.reserve(m_GX*m_GY);
}

void txGridSurface::AssembleIndices(std::vector<int> &indices, bool ccw)
{
	int ij, iij, iijj, ijj;
	for (int j=0; j<m_GY-1; j++) {
		for (int i=0; i<m_GX-1; i++) {
			ij   = j*m_GX + i;
			iij  = ij+1;
			ijj  = (j+1)*m_GX + i;
			iijj = ijj+1;

			if (ccw) {
				indices.push_back(ij);
				indices.push_back(iijj);
				indices.push_back(ijj);

				indices.push_back(ij);
				indices.push_back(iij);
				indices.push_back(iijj);
			} else {
				indices.push_back(ij);
				indices.push_back(ijj);
				indices.push_back(iijj);
				
				indices.push_back(ij);
				indices.push_back(iijj);
				indices.push_back(iij);
			}


		}
	}
}


void txGridSurface::AssignVerts(std::vector<txMesh::Point> &verts)
{
	const double dx = (m_BBox.max[0] - m_BBox.min[0])/(m_GX-1);
	const double dy = (m_BBox.max[1] - m_BBox.min[1])/(m_GY-1);
	double x = m_BBox.min[0];
	double y = m_BBox.min[1];
	txMesh::Point p;
	for (int j=0; j<m_GY; j++) {
		x = m_BBox.min[0];
		for (int i=0; i<m_GX; i++) {
			p[0] = (float)x;  p[1] = (float)y;  p[2] = 0.0;
			verts.push_back(p);
			x+=dx;
		}
		y+=dy;
	}
}

void txGridSurface::CopyToMesh()
{
	std::vector<txMesh::VertexHandle> vhandle;
	vhandle.reserve(m_Verts.size());

	for (size_t i=0; i<m_Verts.size(); i++) {
		vhandle.push_back(m_Mesh.add_vertex(m_Verts[i]));
	}

	std::vector<txMesh::VertexHandle> face_vHandles;
	for (size_t i=0; i<m_Indices.size()/3; i++) {
		face_vHandles.push_back(vhandle[m_Indices[i*3]]);
		face_vHandles.push_back(vhandle[m_Indices[i*3+1]]);
		face_vHandles.push_back(vhandle[m_Indices[i*3+2]]);
		m_Mesh.add_face(face_vHandles);
		face_vHandles.clear();
	}

	
}

void txGridSurface::DumpMeshToOFF(char * f)
{
	CopyToMesh();

	if(OpenMesh::IO::write_mesh(m_Mesh,f)) {
	
	}
	assert(true);
}

bool SearchCallBack(int id, void *arg)
{
	std::vector<int> *fidlist = static_cast<std::vector<int>*>(arg);
	fidlist->push_back(id);
	return true;
}


void txGridSurface::NormalizeTriSurf(txMeshReconstruct *triSurf)
{
	txMesh &mesh = triSurf->GetMesh();
	RTree<int, float, 2> &rtree = triSurf->GetRTree();
	txBBox bbox;
	std::vector<int> indices;
	txMesh::Point pa,pb,pc,pp;
	int nhits;
	for (size_t i=0; i<m_Verts.size(); i++) {
		bbox.min[0] = bbox.max[0] = m_Verts[i][0];
		bbox.min[1] = bbox.max[1] = m_Verts[i][1];
		indices.clear();
		nhits = rtree.Search(bbox.min, bbox.max,SearchCallBack,(void*)(&indices));
		if (nhits>0) {

			for (int fi = 0; fi<nhits; fi++) {
				txMesh::FaceHandle f =  mesh.face_handle(indices[fi]);
				txMesh::ConstFaceVertexIter fv_it;
				pa = mesh.point((fv_it=mesh.cfv_begin(f)));
				++fv_it;
				pb = mesh.point(fv_it);
				++fv_it;
				pc = mesh.point(fv_it);

				pp = m_Verts[i];

				double pad[] = {pa[0],pa[1],pa[2]};
				double pbd[] = {pb[0],pb[1],pb[2]};
				double pcd[] = {pc[0],pc[1],pc[2]};
				double ppd[] = {pp[0],pp[1],pp[2]};

				if (IsInPlanarTriangle(pad,pbd,pcd,ppd) ) {
					m_Verts[i][2] = (float)InterpolateZValue(pad,pbd,pcd,ppd);
					// Calculate one is enough if the point is lay on the triangle edge
					break;
				}
				//m_Verts[i][2] = 
			}

		}
	}
}

void PushHealingMesh(std::vector<int> &indices, std::vector<int> &mIndices, int offset, bool ccw=true)
{
	int ijj, iijj, ij, iij;
	for (size_t i=0; i<indices.size()-1; i++) {
		ijj  = indices[i];
		iijj = indices[i+1];
		ij   = ijj+offset;
		iij  = iijj+offset;
		if (ccw) {
			mIndices.push_back(ij);
			mIndices.push_back(iijj);
			mIndices.push_back(ijj);

			mIndices.push_back(ij);
			mIndices.push_back(iij);
			mIndices.push_back(iijj);
		} else {
			mIndices.push_back(ij);
			mIndices.push_back(ijj);
			mIndices.push_back(iijj);

			mIndices.push_back(ij);
			mIndices.push_back(iijj);
			mIndices.push_back(iij);
		}
	}
}

void txGridSurface::ConstructShapeMesh()
{
	// creat bottom mesh
	txGridSurface bottom(m_BBox.min[0],m_BBox.min[1],m_BBox.max[0],m_BBox.max[1],m_GX,m_GY,false);

	// lift the bottom
	for (size_t i=0; i<bottom.m_Verts.size(); i++) {
		bottom.m_Verts[i][2] = -1.0;
	}

	bottom.DumpMeshToOFF("Bottommesh.OFF");

	const int offset = m_GX*m_GY;


	// TODO! The following code will cause template error!!! try to fix it!
	//m_Indices.insert(bottom.m_Indices.begin(),bottom.m_Indices.end());
	//m_Verts.insert(bottom.m_Verts.begin(),bottom.m_Verts.end());
	for (size_t i=0; i<bottom.m_Indices.size(); i++) {
		m_Indices.push_back(bottom.m_Indices[i]+offset);
	}
	for (size_t i=0; i<bottom.m_Verts.size(); i++) {
		m_Verts.push_back(bottom.m_Verts[i]);
	}


	// constrcut healing connectivity
	// down
	std::vector<int> downIndices;
	// up 
	std::vector<int> upIndices;
	for (int i=0; i<m_GX; i++) {
		downIndices.push_back(i);
		upIndices.push_back(i+m_GX*(m_GY-1));
	}

	// left
	std::vector<int> leftIndices;
	// right
	std::vector<int> rightIndices;
	for (int i=0; i<m_GY; i++) {
		leftIndices.push_back(i*m_GX);
		rightIndices.push_back(i*m_GX + m_GX-1);
	}

	PushHealingMesh(downIndices,m_Indices,offset);
	PushHealingMesh(upIndices,m_Indices,offset,false);
	PushHealingMesh(leftIndices,m_Indices,offset,false);
	PushHealingMesh(rightIndices,m_Indices,offset);

}