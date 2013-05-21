#pragma once
#include <vector>
#include <string>
#include <math.h>

//#include <iostream>
//#include <sstream>
//
//
//template <class Type>  
//Type stringToNum(const std::string& str)  
//{  
//    std::istringstream iss(str);  
//    Type num;  
//    iss >> num;  
//    return num;      
//}



struct txPoint{
	double x,y,z;
	double SquareDistance(txPoint &p) {
		double lx, ly, lz;
		lx = x-p.x;
		ly = y-p.y;
		lz = z-p.z;
		return lx*lx+ly*ly+lz*lz;
	}
	double Distance(txPoint &p) {
		return sqrt(SquareDistance(p));
	}

	double SelfSquare() {
		return x*x+y*y+z*z;
	}
	
};

inline txPoint operator-(txPoint &p0, txPoint &p1)
{
	txPoint temp;
	temp.x = p0.x-p1.x;
	temp.y = p0.y-p1.y;
	temp.z = p0.z-p1.z;
	return temp;
};



inline txPoint Cross(txPoint &p0, txPoint &p1) {
	txPoint temp;
	temp.x = p0.y*p1.z-p0.z*p1.x;
	temp.y = p1.z*p0.x-p1.x*p0.z;
	temp.z = p0.x*p1.y-p0.y*p1.x;

	return temp;
};

class txRawDataLoader
{
public:
	txRawDataLoader() {;};
	//txRawDataLoader() {  char filename[] = "E:\\gitProject\\OpenGl_MeshViewer\\meshviewsolution\\shoesmodel\\603.nc"; LoadFile(filename);};
	txRawDataLoader(char *file, double rtoaxis);
	txRawDataLoader(std::string file) {;};
	~txRawDataLoader(void);




public:
	void LoadFile(char *file, double l);
	void OutPutXYZ(const char *file);

	void OutPutOff(char *file);

private:
	void OutPutXYZ(const char *file, int s, int e);

private:
	void ConstructMesh();

	void CheckValidateFace();


private:
	double rtoaxis;
	std::vector<txPoint> pointlist;
	std::vector<int> rnlist;

	//std::vector<txPoint> cpntlist;
	std::vector<int> faceindex;
};

