#include "StdAfx.h"
#include "trTriangleUtil.h"
#include "exactpredicator.h"
#include "txEigenWrapper.h"

/**
/*               Return a positive value if the points pa, pb, and pc occur  */
/*               in counterclockwise order; a negative value if they occur   */
/*               in clockwise order; and zero if they are collinear.  The    */
/*               result is also a rough approximation of twice the signed    */
/*               area of the triangle defined by the three points.           */
/**
* Test Case for the In Circle Test!
	double pa[3] = {1,0,0};
	double pb[3] = {0,0,1};
	double pc[3] = {0,1,0};

	double pp[3] = {0.25,0.25,0.5};

	double ppout1[3] = {-5,0.25,0.5};
	double ppout2[3] = {5.0,0.25,0.5};
	double ppout3[3] = {0.25,5.0,0.5};

	bool isIn1 = IsInPlanarTriangle(pa,pb,pc,ppout1);
	bool isIn2 = IsInPlanarTriangle(pa,pb,pc,ppout2);
	bool isIn3 = IsInPlanarTriangle(pa,pb,pc,ppout3);

	bool isIn = IsInPlanarTriangle(pa,pb,pc,pp);
*/
bool IsInPlanarTriangle(double *pa, double *pb, double *pc, double *pp)
{
	if ( orient2d(pa,pb,pp)<0&& orient2d(pb,pc,pp)<0 && orient2d(pc,pa,pp)<0) {
		return false;
	}
	return true;
}

/**
* From Wikipedia Barycentric coordinate system
* Converting to barycentric coordinates  
*/
/**
* Test case
* 	double pa[3] = {1,0,0};
	double pb[3] = {0,0,1};
	double pc[3] = {0,1,0};

	double pp[3] = {0.25,0.25,0.5};

	double ppzv = InterpolateZValue(pa,pb,pc,pp);

	ppzv will be 0.5
*/

/**
* PS I use the colPivHouseholderQr here there is other options
Decomposition			 Request on Matrix					Speed Accuracy
--------------------------------------------------------------------
PartialPivLU			 Invertible							++	+ 
FullPivLU				 None								-   +++
HouseholderQR			 None								++  +
ColPivHouseholderQR		 None								+   ++
FullPivHouseholderQR	 None								-	+++
LLT						 Positive definite					+++ +
LDLT					 Positive or negative semidefinite  +++ ++

*/
double InterpolateZValue(double *pa, double *pb, double *pc, double *pp)
{
	txEgVec2f ra, rb, rc, r;
	ra<<pa[0],pa[1];
	rb<<pb[0],pb[1];
	rc<<pc[0],pc[1];
	r <<pp[0],pp[1];

	txEgVec2f rrc = r-rc;
	txEgVec2f rac = ra-rc;
	txEgVec2f rbc = rb-rc;
	txEgMatrix2X2 m;/*(ra-rc,rb-rc);*/
	m<<rac.x(),rbc.x(),
		rac.y(),rbc.y();
	txEgVec2f lambda = m.colPivHouseholderQr().solve(rrc);

	double rtn = pa[2]*lambda.x() + pb[2]*lambda.y() + (1-lambda.x()-lambda.y())*pc[2];
	return rtn;
}





