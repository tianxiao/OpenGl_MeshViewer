// meshviewsolution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gl.hh"

#include "GlutOperatorN.h"
#include "MeshViewer.h"


int _tmain(int argc, _TCHAR* argv[])
{
	glutInit(&argc, (char**)argv);

	//GlutOperatorN window("Try", 512, 512);
	//if (argc>1) {
	//	;
	//}

	MeshViewer win("Terrain", 512, 512);
	win.OpenMesh("delaunay1000.off");

	glutMainLoop();

	return 0;
}

