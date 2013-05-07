#include "StdAfx.h"
#include "GlutViewer.h"



std::map<int, GlutViewer*> GlutViewer::m_Windows;

GlutViewer::GlutViewer(const char *wtitle, int width, int height)
	:m_Width(width)
	,m_Height(height)
{
	// create window
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA);
	glutInitWindowSize(width, height);
	m_WindowID = glutCreateWindow(wtitle);
	m_Windows[m_WindowID] = this;

	// register callbacks;
	glutDisplayFunc(SDisplay);
	glutKeyboardFunc(SKeyBoard);
	glutSpecialFunc(SSpecial);
	glutMouseFunc(SMouse);
	glutMotionFunc(SMotion);
	glutPassiveMotionFunc(SPassiveMotion);
	glutReshapeFunc(SReshape);
	glutVisibilityFunc(SVisibility);

	// create menu
	m_NDrawModes = 0;
	m_MenuID = glutCreateMenu(SProcessMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}


GlutViewer::~GlutViewer(void)
{
	glutDestroyWindow(m_WindowID);
	glutDestroyMenu(m_MenuID);
}


void GlutViewer::ClearDrawModes()
{
	for (unsigned int i=0; i<m_NDrawModes; i++) {
		glutRemoveMenuItem(1);
	}
	m_NDrawModes = 0;
	m_DrawModeNames.clear();
}

unsigned int GlutViewer::AddDrawMode(const std::string &s) 
{
	// insert in popup menu
	glutAddMenuEntry(s.c_str(), m_NDrawModes);

	++this->m_NDrawModes;
	this->m_DrawModeNames.push_back(s);

	return this->m_NDrawModes-1;
}

void GlutViewer::SetDrawMode(int id)
{
	m_DrawMode = id;
	glutPostRedisplay();
}

GlutViewer* GlutViewer::CurrentWidow() {
	return m_Windows[glutGetWindow()];
}

void GlutViewer::SDisplay(void) {
	CurrentWidow()->Display();
}

void GlutViewer::SIdle(void) {
	CurrentWidow()->Idle();
}

void GlutViewer::SKeyBoard(unsigned char key, int x, int y) {
	CurrentWidow()->KeyBoard(key, x, y);
}

void GlutViewer::SMotion(int x, int y) {
	CurrentWidow()->Motion(x, y);
}

void GlutViewer::SPassiveMotion(int x, int y) {
	CurrentWidow()->PassiveMotion(x, y);
}

void GlutViewer::SReshape(int w, int h) {
	CurrentWidow()->Reshape(w, h);
}

void GlutViewer::SSpecial(int key, int x, int y) {
	CurrentWidow()->Special(key, x, y);
}

void GlutViewer::SVisibility(int visible) {
	CurrentWidow()->Visibility(visible);
}

void GlutViewer::SProcessMenu(int id) {
	CurrentWidow()->ProcessMenu(id);
}

void GlutViewer::SMouse(int button, int state, int x, int y) {
	CurrentWidow()->Mouse(button, state, x, y);
}

void GlutViewer::Idle(void) {}
void GlutViewer::Motion(int x, int y) {}
void GlutViewer::Mouse(int button, int state, int x, int y) {}
void GlutViewer::PassiveMotion(int x, int y) {}
void GlutViewer::Visibility(int visible) {}
void GlutViewer::Reshape(int w, int h) {}
void GlutViewer::Special(int key, int x, int y) {}

void GlutViewer::ProcessMenu(int i) {
	this->SetDrawMode(i);
}

void GlutViewer::KeyBoard(int key, int x, int y)
{
	switch(key) {
	case 27:
		{
			exit(0);
			break;
		}

	case GLUT_KEY_F12:
		{
			if (!m_FullScreen){
				m_BakLeft = glutGet(GLUT_WINDOW_X);
				m_BakTop = glutGet(GLUT_WINDOW_Y);
				m_BakWidth = glutGet(GLUT_WINDOW_WIDTH);
				m_BakHeight = glutGet(GLUT_WINDOW_HEIGHT);
				glutFullScreen();
				m_FullScreen = true;
			} else {
				glutReshapeWindow(m_BakWidth, m_BakHeight);
				glutPositionWindow(m_BakLeft, m_BakTop);
				m_FullScreen = false;
			}
			break;
		}
	}
}

void GlutViewer::Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_DrawMode < m_DrawModeNames.size()) {
		Draw(m_DrawModeNames[m_DrawMode]);
	} else {
		Draw("");
	}

	glutSwapBuffers();
}





