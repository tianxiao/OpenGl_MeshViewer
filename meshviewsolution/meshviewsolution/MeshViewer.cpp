#include "StdAfx.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include "MeshViewer.h"



MeshViewer::MeshViewer(const char *title, int width, int height)
	:GlutOperatorN(title, width, height)
{
	m_Mesh.request_face_normals();
	m_Mesh.request_vertex_normals();

	ClearDrawModes();
	AddDrawMode("Wireframe");
	AddDrawMode("Hidden Line");
	AddDrawMode("Solid Flat");
	AddDrawMode("Solid Smooth");
	SetDrawMode(3);
}


MeshViewer::~MeshViewer(void)
{
}

bool MeshViewer::OpenMesh(const char* f)
{
	if (OpenMesh::IO::read_mesh(m_Mesh,f)){
		txMesh::ConstVertexIter 
			v_it(m_Mesh.vertices_begin()),
			v_end(m_Mesh.vertices_end());
		txMesh::Point bbMin, bbMax;
		bbMin = bbMax = m_Mesh.point(v_it);
		for (; v_it!=v_end; ++v_it) {
			bbMin.minimize(m_Mesh.point(v_it));
			bbMax.maximize(m_Mesh.point(v_it));
		}
		SetScene( (Vec3f)(bbMin+bbMax)*0.5, 0.5*(bbMin-bbMax).norm());

		m_Mesh.update_normals();

		UpdateFaceIndices();

		return true;
	}

	std::cerr << "read error\n";
	return false;
}

void MeshViewer::UpdateFaceIndices()
{
	txMesh::ConstFaceIter 
		f_it(m_Mesh.faces_sbegin()),
		f_end(m_Mesh.faces_end());

	txMesh::ConstFaceVertexIter fv_it;

	m_Indices = std::vector<unsigned int>();
	m_Indices.reserve(m_Mesh.n_faces()*3);
	std::cout << "mesh indices updated" << std::endl;

	for (; f_it!=f_end; ++f_it) {
		m_Indices.push_back((fv_it=m_Mesh.cfv_begin(f_it)).handle().idx());
		m_Indices.push_back((++fv_it).handle().idx());
		m_Indices.push_back((++fv_it).handle().idx());
	}
}

void MeshViewer::Draw(const std::string &dm)
{
	if (m_Indices.empty())
	{
		GlutOperatorN::Draw(dm);
		return;
	}

	if (dm == "Wireframe") {
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnableClientState(GL_VERTEX_ARRAY);
		GL::glVertexPointer(m_Mesh.points());

		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, &m_Indices[0]);

		glDisableClientState(GL_VERTEX_ARRAY);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
