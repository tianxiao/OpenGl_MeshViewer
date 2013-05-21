// shoesmodel2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "txRawDataLoader.h"


int _tmain(int argc, _TCHAR* argv[])
{

	txRawDataLoader loader();


	txRawDataLoader loader2;
	char *file = "603.nc";
	loader2.LoadFile(file, 300);

	loader2.OutPutXYZ("603.xyz");
	loader2.OutPutOff("603.off");
	return 0;
}

