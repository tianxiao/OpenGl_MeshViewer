#include "StdAfx.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "txMeshReconstruct.h"


txMeshReconstruct::txMeshReconstruct(void)
{
}


txMeshReconstruct::~txMeshReconstruct(void)
{
}

/**
Input the following cmd into gunplot you got a visualized surface
set xrange [-2:2]
set yrange [-2:2]
set pm3d at b
set ticslevel 0.8
set isosample 40, 40
splot -x*x*exp(-x*x)*y*y*exp(-y*y)
*/
double txFourPeakFunO(double x, double y)
{
	double rtn = 10*x*x*exp(-x*x)*y*y*exp(-y*y)+1;
	return rtn;
}

typedef double (*txZVFunction) (double, double);

void ScaleMesh(double lx, double ly, double llx, double lly, txMesh &m, txZVFunction f)
{
	txMesh::VertexIter v_it(m.vertices_begin()), v_end(m.vertices_end());
	double x,y;
	for(; v_it!=v_end; ++v_it) {
		x = llx*m.point(v_it)[0]/lx;
		y = lly*m.point(v_it)[1]/ly;
		m.point(v_it)[0] = (float)x;
		m.point(v_it)[1] = (float)y;
		m.point(v_it)[2] = (float)f(x,y);
	}
}

void ScaleMesh2(double lx, double ly, double lz, double llx, double lly, double llz,txMesh &m, txZVFunction f)
{
	txMesh::VertexIter v_it(m.vertices_begin()), v_end(m.vertices_end());
	double x,y,z;
	for(; v_it!=v_end; ++v_it) {
		x = llx*m.point(v_it)[0]/lx;
		y = lly*m.point(v_it)[1]/ly;
		z = llz*m.point(v_it)[2]/lz;
		m.point(v_it)[0] = (float)x;
		m.point(v_it)[1] = (float)y;
		m.point(v_it)[2] = (float)z;
	}
}

bool txMeshReconstruct::OpenMesh(const char* f)
{
	if (OpenMesh::IO::read_mesh(m_Mesh,f)) {
		txMesh::ConstVertexIter 
			v_it(m_Mesh.vertices_begin()),
			v_end(m_Mesh.vertices_end());
		// Calculate the raw bbox
		bbMin = bbMax = m_Mesh.point(v_it);
		for (; v_it!=v_end; ++v_it) {
			bbMin.minimize(m_Mesh.point(v_it));
			bbMax.maximize(m_Mesh.point(v_it));
		}

		ScaleMesh((bbMax[0]-bbMin[0])/2.0, (bbMax[1]-bbMin[2])/2.0, 2, 2, m_Mesh, txFourPeakFunO);

		// Calculate the scaled bbox 
		// TODO!!! The following code can be optimized by using the bbox instead of 
		// recalucate the bbox!
		v_it = m_Mesh.vertices_begin();
		bbMin = bbMax = m_Mesh.point(v_it);
		for (; v_it!=v_end; ++v_it) {
			bbMin.minimize(m_Mesh.point(v_it));
			bbMax.maximize(m_Mesh.point(v_it));
		}

		ConstructRTree();
		return true;
	}

	std::cout << "read:\t" << f << "failed!" << std::endl;
	return false;
}

bool txMeshReconstruct::WriteMesh(const char* f)
{
	if(OpenMesh::IO::write_mesh(m_Mesh, f)) {
		
		return true;
	}

	return false;
}

void txMeshReconstruct::ConstructRTree()
{
	txMesh::Point bbMinI, bbMaxI;

	txMesh::ConstFaceIter f_it(m_Mesh.faces_sbegin()), f_end(m_Mesh.faces_end());
	txMesh::ConstFaceVertexIter fv_it;

	for (; f_it!=f_end; ++f_it) {
		bbMinI = bbMaxI = m_Mesh.point((fv_it=m_Mesh.cfv_begin(f_it)));
		++fv_it;
		bbMinI.minimize(m_Mesh.point(fv_it));
		bbMaxI.maximize(m_Mesh.point(fv_it));
		++fv_it;
		bbMinI.minimize(m_Mesh.point(fv_it));
		bbMaxI.maximize(m_Mesh.point(fv_it));

		m_RTree.Insert(&(bbMinI[0]),&(bbMaxI[0]),f_it.handle().idx());
	}
}

void txMeshReconstruct::InterpretGridPoint()
{
	
}

bool txMeshReconstruct::OpenFinalMesh(const char *f)
{
	if (OpenMesh::IO::read_mesh(m_Mesh,f)) {
		txMesh::ConstVertexIter 
			v_it(m_Mesh.vertices_begin()),
			v_end(m_Mesh.vertices_end());
		// Calculate the raw bbox
		bbMin = bbMax = m_Mesh.point(v_it);
		for (; v_it!=v_end; ++v_it) {
			bbMin.minimize(m_Mesh.point(v_it));
			bbMax.maximize(m_Mesh.point(v_it));
		}

		ScaleMesh2((bbMax[0]-bbMin[0])/2.0, (bbMax[1]-bbMin[1])/2.0, (bbMax[2]-bbMin[2])/2.0,2, 2, 2, m_Mesh,NULL);

		// Calculate the scaled bbox 
		// TODO!!! The following code can be optimized by using the bbox instead of 
		// recalucate the bbox!
		v_it = m_Mesh.vertices_begin();
		bbMin = bbMax = m_Mesh.point(v_it);
		for (; v_it!=v_end; ++v_it) {
			bbMin.minimize(m_Mesh.point(v_it));
			bbMax.maximize(m_Mesh.point(v_it));
		}

		ConstructRTree();
		return true;
	}

	std::cout << "read:\t" << f << "failed!" << std::endl;
	return false;
}





