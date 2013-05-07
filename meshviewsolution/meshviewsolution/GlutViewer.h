#pragma once
/**
* Base class wrap the platform depend UI 
*/
#include "gl.hh"
#include <string>
#include <map>
#include <vector>

class GlutViewer
{
public:
	GlutViewer(const char *wtitle, int width, int height);
	virtual ~GlutViewer(void);

protected:
	virtual void Draw(const std::string &drawmode) = 0;
	void ClearDrawModes();
	unsigned int AddDrawMode(const std::string &s);
	void SetDrawMode(int id);

	virtual void Display(void);
	virtual void Idle(void);
	virtual void KeyBoard(int key, int x, int y);
	virtual void Motion(int x, int y);
	virtual void Mouse(int button, int state, int x, int y);
	virtual void PassiveMotion(int x, int y);
	virtual void Reshape(int w, int h);
	virtual void Special(int key, int x, int y);
	virtual void Visibility(int visible);
	virtual void ProcessMenu(int i);

	int m_Width, m_Height;

private:

	static void SDisplay(void);
	static void SIdle(void);
	static void SKeyBoard(unsigned char key, int x, int y);
	static void SMotion(int x, int y);
	static void SMouse(int button, int state, int x, int y);
	static void SPassiveMotion(int x, int y);
	static void SReshape(int w, int h);
	static void SSpecial(int key, int x, int y);
	static void SVisibility(int visible);
	static void SProcessMenu(int i);

	static std::map<int, GlutViewer*> m_Windows;
	static GlutViewer* CurrentWidow();

private:
	int m_WindowID, m_MenuID;

	bool m_FullScreen;
	int m_BakLeft, m_BakTop, m_BakWidth, m_BakHeight;

	unsigned int m_DrawMode;
	unsigned int m_NDrawModes;
	std::vector<std::string> m_DrawModeNames;
};

