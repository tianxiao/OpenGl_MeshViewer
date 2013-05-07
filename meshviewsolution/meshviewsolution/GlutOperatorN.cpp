#include "StdAfx.h"
#include "GlutOperatorN.h"
#include <OpenMesh/Tools/Utils/Timer.hh>


GlutOperatorN::GlutOperatorN(const char *title, int width, int height)
	: GlutViewer(title, width, height)
{
	Init();

	// init mouse buttons
	for (int i=0; i<10; i++) {
		m_ButtonDown[i] = false;
	}

	// draw mode
	AddDrawMode("Wireframe");
	AddDrawMode("Hidden Line");
	AddDrawMode("Solid Flat");
	AddDrawMode("Solid Smooth");
	AddDrawMode("Solid Wire");
	SetDrawMode(1);
}


GlutOperatorN::~GlutOperatorN(void)
{
}

void GlutOperatorN::Init() 
{
	// OpenGL state
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDisable(GL_DITHER);
	glEnable(GL_DEPTH_TEST);

	// some performance setting
	// glEnable( GL_CULL_FACE);
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

	// material
	GLfloat mat_a[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat mat_d[] = {0.1, 0.4, 0.4, 1.0};
	GLfloat mat_s[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat shine[] = {128.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

	// lighting
	glLoadIdentity();
	GLfloat pos1[] = {0.1, 0.1, -0.02, 0.0};
	GLfloat pos2[] = {-0.1, 0.1, -0.02, 0.0};
	GLfloat pos3[] = {0.0, 0.0, 0.1, 0.0};
	GLfloat col1[] = {0.05, .05, .6, 1.0};
	GLfloat col2[] = {.6, .05, .05, 1.0};
	GLfloat col3[] = {1.0, 1.0, 1.0, 1.0};

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, col1);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, pos2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, col2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, col2);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, pos3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col3);

	// scene pos and size
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelViewMatrix);
	SetScene(Vec3f(0.0,0.0,0.0),1.0);

	// projection
	m_Near = 0.1;
	m_Far = 100.0;
	m_Fovy = 45.0;
}

void GlutOperatorN::Reshape(int w, int h) 
{
	m_Width = w;
	m_Height = h;
	glViewport(0,0,w,h);
	UpdateProjectionMatrix();
	glutPostRedisplay();
}

void GlutOperatorN::UpdateProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_Fovy, (GLfloat)m_Width/(GLfloat)m_Height, m_Near, m_Far);
	glGetDoublev(GL_PROJECTION_MATRIX, m_ProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
}

void GlutOperatorN::SetScene(const Vec3f &cog, float radius) 
{
	m_Center = cog;
	m_Radius = radius;

	m_Near = 0.01*radius;
	m_Far = 10.0 * radius;
	UpdateProjectionMatrix();

	ViewAll();
}

void GlutOperatorN::ViewAll()
{
	Translate(
		Vec3f(
		-( 
		m_ModelViewMatrix[0]*m_Center[0] +
		m_ModelViewMatrix[4]*m_Center[1] +
		m_ModelViewMatrix[8]*m_Center[2] +
		m_ModelViewMatrix[12]),
		-(
		m_ModelViewMatrix[1]*m_Center[0] +
		m_ModelViewMatrix[5]*m_Center[1] +
		m_ModelViewMatrix[9]*m_Center[2] +
		m_ModelViewMatrix[13]),
		-(
		m_ModelViewMatrix[2]*m_Center[0] +
		m_ModelViewMatrix[6]*m_Center[1] +
		m_ModelViewMatrix[10]*m_Center[2] +
		m_ModelViewMatrix[14] + 3.0*m_Radius)
		)
		);
}

bool GlutOperatorN::MapToShpere(const Vec2i &v2D, Vec3f &v3D)
{
	if ( (v2D[0]>=0) && (v2D[0]<=m_Width) &&
		(v2D[1]>=0) && (v2D[1]<=m_Height)) 
	{
		double x = (double) (v2D[0] - 0.5*m_Width) /(double)m_Width;
		double y = (double) (0.5*m_Height - v2D[1])/(double)m_Height;
		double sinx = sin(M_PI*x*0.5);
		double siny = sin(M_PI*y*0.5);
		double sinxsiny = sinx*sinx + siny*siny;

		v3D[0] = sinx;
		v3D[1] = siny;
		v3D[2] = sinxsiny <1.0?sqrt(1.0-sinxsiny):0.0;

		return true;
	} else {
		return false;
	}
}

void GlutOperatorN::Draw(const std::string &dm) 
{//Wireframe
	if (dm == "Wireframe") {
		glDisable(GL_LIGHTING);
		glutWireTeapot(0.5);
	} else if (dm == "Solid Flat") {
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		glutSolidTeapot(0.5);
	} else if (dm == "Solid Smooth") {
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glutSolidTeapot(0.5);
	} else if (dm == "Hidden Line") {
		glDisable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glColor3f(0.0, 0.0, 0.0);

		glDepthRange(0.01, 1.0);
		glutSolidTeapot(0.5);

		glColor3f(1.0, 1.0, 1.0);
		glDepthRange(0.0, 1.0);
		glutWireTeapot(0.5); 
	} else if (dm == "Solid Wire") {
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		glutWireTeapot(0.5);
		//glutSolidTeapot(0.5);
	} else {
		std::cout << "This view mode is not support for this geometry, you need to ..."<< std::endl;
	}
}

void GlutOperatorN::Mouse(int button, int state, int x, int y)
{
	// mouse press
	if (state == GLUT_DOWN) {
		this->m_LastPoint2D = Vec2i(x, y);
		this->m_LastPointOK = MapToShpere(this->m_LastPoint2D,this->m_LastPoint3D);
		this->m_ButtonDown[button] = true;
	}

	// mouse release
	else {
		this->m_LastPointOK = false;
		this->m_ButtonDown[button] = false;

		// GLUT: button 3 or 4 -> mouse wheel clicked
		if (button == 3) {
			Zoom(0, (int)(this->m_LastPoint2D[1] - 0.05*m_Width));
		} else if (button == 4) {
			Zoom(0, (int)(this->m_LastPoint2D[1] + 0.05*m_Width));
		}
	}

	glutPostRedisplay();
}

void GlutOperatorN::Motion(int x, int y)
{
	// zoom
	if (this->m_ButtonDown[0] && this->m_ButtonDown[1]) {
		Zoom(x,y);
	}

	// rotation
	else if (this->m_ButtonDown[0]) {
		Rotation(x, y);
	}

	// translation
	else if (this->m_ButtonDown[1]) {
		Translation(x, y);
	}

	this->m_LastPoint2D = Vec2i(x, y);
	this->m_LastPointOK = MapToShpere(m_LastPoint2D, m_LastPoint3D);

	glutPostRedisplay();
}

void GlutOperatorN::Rotation( int x, int y)
{
	if (m_LastPointOK) {
		Vec2i newP2D;
		Vec3f newP3D;
		bool  newPOK;

		newP2D = Vec2i(x, y);
		newPOK = MapToShpere(newP2D, newP3D);

		if (newPOK) {
			Vec3f axis = (m_LastPoint3D % newP3D);
			float cos_angle = (m_LastPoint3D | newP3D);

			if (fabs(cos_angle)<1.0) {
				float angle = 2.0*acos(cos_angle)*180.0/M_PI;
				Rotate(axis, angle);
			}
		}
	}
}

void GlutOperatorN::Translation(int x, int y)
{
	float dx = x - m_LastPoint2D[0];
	float dy = y - m_LastPoint2D[1];

	float z = -(
		m_ModelViewMatrix[2]*m_Center[0] +
		m_ModelViewMatrix[6]*m_Center[1] +
		m_ModelViewMatrix[10]*m_Center[2] + 
		m_ModelViewMatrix[14])/
		(
		m_ModelViewMatrix[3]*m_Center[0] + 
		m_ModelViewMatrix[7]*m_Center[1] +
		m_ModelViewMatrix[11]*m_Center[2] + 
		m_ModelViewMatrix[15]);

	float aspect = (float)m_Width/(float)m_Height;
	float up = tan(m_Fovy/2.0f*M_PI/180.f)*m_Near;
	float right = aspect*up;

	Translate(Vec3f(2.0*dx/m_Width*right/m_Near*z,
		-2.0*dy/m_Height*up/m_Near*z,
		0.0f));
}

void GlutOperatorN::Zoom(int x, int y)
{
	float dy = y - m_LastPoint2D[1];
	float h = m_Height;
	Translate(Vec3f(0.0, 0.0, m_Radius*dy*3.0/h));
}

void GlutOperatorN::Translate(const Vec3f &trans)
{
	glLoadIdentity();
	glTranslated(trans[0],trans[1],trans[2]);
	glMultMatrixd(m_ModelViewMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelViewMatrix);
}

void GlutOperatorN::Rotate(const Vec3f &axis, float angle)
{
		Vec3f t(
		(
		m_ModelViewMatrix[0]*m_Center[0] + 
		m_ModelViewMatrix[4]*m_Center[1] + 
		m_ModelViewMatrix[8]*m_Center[2] + 
		m_ModelViewMatrix[12]
		), 
		(
		m_ModelViewMatrix[1]*m_Center[0] + 
		m_ModelViewMatrix[5]*m_Center[1] + 
		m_ModelViewMatrix[9]*m_Center[2] + 
		m_ModelViewMatrix[13]),
		(
		m_ModelViewMatrix[2]*m_Center[0] + 
		m_ModelViewMatrix[6]*m_Center[1] + 
		m_ModelViewMatrix[10]*m_Center[2] + 
		m_ModelViewMatrix[14])
		);
	glLoadIdentity();
	glTranslatef(t[0], t[1], t[2]);
	glRotated(angle, axis[0], axis[1], axis[2]);
	glTranslatef(-t[0], -t[1], -t[2]);
	glMultMatrixd(m_ModelViewMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelViewMatrix);
}

void GlutOperatorN::KeyBoard(int key, int x, int y)
{
	switch(key)
	{
	case 'f':
		{
			std::cerr << "Performance test: ";
			double fps = MeasureFPS();
			std::cerr << fps << "FPS\n";
			break;
		}

	default:
		{
			GlutViewer::KeyBoard(key, x, y);
			break;
		}
	}
}

double GlutOperatorN::MeasureFPS()
{
	double fps(0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	unsigned int frames = 90;
	const float  angle = 360.0/(float)frames;
	unsigned int i;
	Vec3f axis;

	OpenMesh::Utils::Timer timer; timer.start();

	for (i=0, axis=Vec3f(1.0, 0.0, 0.0); i<frames; i++) {
		Rotate(axis, angle); Display();
	}
	for (i=0, axis=Vec3f(0.0, 1.0, 0.0); i<frames; i++) {
		Rotate(axis, angle); Display();
	}
	for (i=0, axis=Vec3f(0.0, 0.0, 1.0); i<frames; i++) {
		Rotate(axis, angle); Display();
	}

	glFinish();

	timer.stop();
	fps = (1000.0/timer.mseconds()*(3.0*frames));

	glPopMatrix();
	glutPostRedisplay();

	return fps;
}














