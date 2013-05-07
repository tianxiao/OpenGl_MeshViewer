#pragma once
#include "glutviewer.h"
#include <OpenMesh/Core/Geometry/VectorT.hh>

using namespace OpenMesh;

class GlutOperatorN :
	public GlutViewer
{
public:
	GlutOperatorN(const char *title, int width, int height);
	virtual ~GlutOperatorN(void);

	void SetScene(const Vec3f& center, float radius);
	void ViewAll();
	double MeasureFPS();

protected:
	virtual void Init();
	virtual void Draw(const std::string &drawMode);

	// overload glut functions
	virtual void Motion(int x, int y);
	virtual void Mouse(int button, int state, int x, int y);
	virtual void Reshape(int w, int h);
	virtual void KeyBoard(int key, int x, int y);

	// update projection matrix
	void UpdateProjectionMatrix();
	// translate the scene and update modelview matrix
	void Translate(const Vec3f& trans);
	// rotate the scene (around its center) and update modelview matrix
	void Rotate(const Vec3f& axis, float angle);

	// virtual trackball: map 2D screen point to unit shpere
	bool MapToShpere(const Vec2i& point, Vec3f& result);

	// mouse processing functions
	void Rotation(int x, int y);
	void Translation(int x, int y);
	void Zoom(int x, int y);

protected:
	// scene position and dimension
	Vec3f m_Center;
	float m_Radius;

	// projection parameters
	float m_Near, m_Far, m_Fovy;

	// OpenGL matrices
	double m_ProjectionMatrix[16],
		m_ModelViewMatrix[16];

	// trackball helpers
	Vec2i m_LastPoint2D;
	Vec3f m_LastPoint3D;
	bool m_LastPointOK;
	bool m_ButtonDown[10];

};

