#include "StdAfx.h"
#include "txRawDataLoader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <assert.h>


#define txPI 3.14159265358979323846264338327950288419716939937510


txRawDataLoader::txRawDataLoader(char *file, double rtoaxis)
{

	LoadFile(file, rtoaxis);
}

txRawDataLoader::~txRawDataLoader(void)
{
}

void txRawDataLoader::LoadFile(char *file, double l)
{
	rtoaxis=250;

	FILE *f = fopen(file,"r");
	char cache[50];
	std::string line;
	line.reserve(50);
	line="                                             ";
	
	double angle;
	int row = 0;
	int currntrow = 0;
	rnlist.push_back(0);

	if (f) {
		for (int i=0; i<32; i++) {
			fscanf(f,"%s\n",cache);
		}

		int numv;
		int xI;		int cI;		int zI;
		std::string xstt, cstt, zstt;
		double x, c, z;
		fscanf(f,"%s %d",cache, &numv);
		pointlist.reserve(numv);
		
		txPoint temp;
		double r, theata;

		// for itoa 
		char itoacache[25];


		for (int i=0; i<numv; i++) {
			fscanf(f,"%s",&line[0]);
			xI = line.find('X');
			cI = line.find('C');
			zI = line.find('Z');
			xstt = line.substr(xI+1,cI-xI-1);
			cstt = line.substr(cI+1,zI-cI-1);
			zstt = line.substr(zI+1,line.size()-zI-1);
			x = atof(xstt.c_str());
			c = atof(cstt.c_str());
			z = atof(zstt.c_str());

			r = rtoaxis - x;
			
			row = int(c/360);
			if (currntrow<row) {
				currntrow = row;
				rnlist.push_back(i);
#if 0
				// debug
				std::string filename = "603-";
				itoa(row,itoacache,10);
				filename +=itoacache;
				filename +=".xyz";
				OutPutXYZ(filename.c_str(), rnlist[row-1], rnlist[row]);
#endif
			}
			angle = c-360*(row);
			theata = angle/180*txPI;
			temp.x = r*cos(theata);
			temp.y = r*sin(theata);
			temp.z = z;
			pointlist.push_back(temp);
		}
		
	}

	fclose(f);
}

void txRawDataLoader::OutPutXYZ(const char *file)
{
	FILE *f = fopen(file,"w");

	for (size_t i=0; i<pointlist.size(); i++) {
		txPoint &temp = pointlist[i];
		fprintf(f,"%f %f %f\n",temp.x, temp.y, temp.z);
	}

	fclose(f);
}

int FindMinInRange(txPoint &p, std::vector<txPoint> &ptnlist, int s, int e)
{
	assert(e>s);
	assert((int)ptnlist.size()>e);
	double mindist;
	double temp;
	int rtn = s;
	mindist = p.SquareDistance(ptnlist[s]);
	for ( int i=s+1; i<=e; i++) {
		temp = p.SquareDistance(ptnlist[i]);
		if (mindist> temp) {
			mindist = temp;
			rtn = i;
		}
	}
	return rtn;
}

void txRawDataLoader::ConstructMesh()
{
	//cpntlist.reserve(pointlist.size());
	faceindex.reserve(pointlist.size()*7);
	int rows0;
	int rows1;
	int rows2;
	int lb, rb, rt, lt;
	for (size_t i=0; i<rnlist.size()-2; i++) {
		rows0 = rnlist[i];
		rows1 = rnlist[i+1];
		rows2 = rnlist[i+2];
		for (int j=rows0; j<rows1-1; j++) {
			lb = j;
			rb = j+1;
			lt = FindMinInRange(pointlist[lb],pointlist,rows1,rows2);
			if (rows2>lt) {
				rt = FindMinInRange(pointlist[rb],pointlist,lt,rows2);

				for (int k=lt; k<rt; k++) {
					faceindex.push_back(lb);
					faceindex.push_back(k+1);
					faceindex.push_back(k);
				}
			} else {
				rt = rows2;
			}

			faceindex.push_back(lb);
			faceindex.push_back(rb);
			faceindex.push_back(rt);
		}
		// seal face
		faceindex.push_back(rows1-1);
		faceindex.push_back(rows2);
		faceindex.push_back(rows2-1);

		faceindex.push_back(rows1-1);
		faceindex.push_back(rows1);
		faceindex.push_back(rows2);
	} 


}

// Copy the utility from the v2d in txV2DUtil.cpp
void OutPutDelaunayTrianglOFF(std::vector<int> &points, std::vector<size_t> &indexes, char *filename)
{
	FILE *f = fopen(filename, "w");

	fprintf(f,"OFF\n");

	fprintf(f,"%d\t%d\t%d\n", points.size()/2, indexes.size()/3, 0);

	for (size_t i=0; i<points.size()/2; i++) {
		fprintf(f,"%d\t%d\t%d\n", points[i*2], points[i*2+1], 0);
	}

	for (size_t i=0; i<indexes.size()/3; i++) {
		fprintf(f,"%d\t%d\t%d\t%d\n", 3,
			indexes[i*3],
			indexes[i*3+1],
			indexes[i*3+2]);
	}

	fclose(f);
}

void OutPutDelaunayTrianglOFF(std::vector<txPoint> &points, std::vector<int> &indexes, char *file) {
	FILE *f = fopen(file, "w");

	fprintf(f,"OFF\n");

	fprintf(f,"%d\t%d\t%d\n", points.size(), indexes.size()/3, 0);

	for (size_t i=0; i<points.size(); i++) {
		fprintf(f,"%f\t%f\t%f\n", points[i].x, points[i].y, points[i].z, 0);
	}

	for (size_t i=0; i<indexes.size()/3; i++) {
		fprintf(f,"%d\t%d\t%d\t%d\n", 3,
			indexes[i*3],
			indexes[i*3+1],
			indexes[i*3+2]);
	}

	fclose(f);	
}

void txRawDataLoader::OutPutOff(char *file)
{
	ConstructMesh();
	//CheckValidateFace();
	OutPutDelaunayTrianglOFF(pointlist,faceindex,file);
}

void txRawDataLoader::CheckValidateFace()
{
	std::vector<int> temface;
	temface.reserve(faceindex.size());
	int a,b,c;
	txPoint pa, pb, pc;
	txPoint ac, bc;
	txPoint normal;
	for (int i=0; i<faceindex.size()/3; i++) {
		a = faceindex[3*i];
		b = faceindex[3*i+1];
		c = faceindex[3*i+2];
		pa = pointlist[a];
		pb = pointlist[b];
		pc = pointlist[c];
		//if (a!=b&&b!=c&&a!=c) {
		//	temface.push_back(a);
		//	temface.push_back(b);
		//	temface.push_back(c);
		//}
		ac = pa-pc;
		bc = pb-pc;
		normal = Cross(ac, bc);
		if (normal.SelfSquare()>0.000001) {
			temface.push_back(a);
			temface.push_back(b);
			temface.push_back(c);			
		}
	}

	faceindex = temface;
}

void txRawDataLoader::OutPutXYZ(const char *file, int s, int e)
{
	

	FILE *f = fopen(file,"w");

	for (int i=s; i<e; i++) {
		txPoint &temp = pointlist[i];
		fprintf(f,"%f %f %f\n",temp.x, temp.y, temp.z);
	}

	fclose(f);	
}