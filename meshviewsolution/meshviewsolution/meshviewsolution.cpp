// meshviewsolution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gl.hh"

#include "GlutOperatorN.h"


int _tmain(int argc, _TCHAR* argv[])
{
	glutInit(&argc, (char**)argv);

	GlutOperatorN window("Try", 512, 512);
	if (argc>1) {
		;
	}

	glutMainLoop();

	return 0;
}

