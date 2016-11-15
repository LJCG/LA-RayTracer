#include "objects.h"
#include "operations.h"
#include "textures.h"
#include <stdio.h>
#include <math.h>

/*
POINT2D getRectangleTexture(OBJECT obj, POINT intersection){
	POLYGON polygon = obj.polygon;

	double H = getDistance(polygon.rectangle[0], polygon.rectangle[1]);
	double L = getDistance(polygon.rectangle[0], polygon.rectangle[4]);


	VECTOR U;
	U.x = (polygon.rectangle[1].x - polygon.rectangle[0].x)/H;
	U.y = (polygon.rectangle[1].y - polygon.rectangle[0].y)/H;
	U.z = (polygon.rectangle[1].z - polygon.rectangle[0].z)/H;

	VECTOR V;
	V.x = (polygon.rectangle[4].x - polygon.rectangle[0].x)/L;
	V.y = (polygon.rectangle[4].y - polygon.rectangle[0].y)/L;
	V.z = (polygon.rectangle[4].z - polygon.rectangle[0].z)/L;

	VECTOR d;
	d.x = intersection.x - polygon.rectangle[0].x;
	d.y = intersection.y - polygon.rectangle[0].y;
	d.z = intersection.z - polygon.rectangle[0].z;

	POINT2D coord;

	coord.u = pointProduct(d, U)/H;
	coord.v = pointProduct(d, V)/L;

	return coord;
}
*/

POINT2D getRectangleTexture(OBJECT obj, POINT intersection){
	POLYGON polygon = obj.polygon;

	POINT p = polygon.rectangle[4];

	double H = getDistance(polygon.points[0], polygon.points[1]);
	double L = getDistance(polygon.points[0], polygon.points[3]);

	VECTOR U;
	U.x = (polygon.points[1].x - polygon.points[0].x)/H;
	U.y = (polygon.points[1].y - polygon.points[0].y)/H;
	U.z = (polygon.points[1].z - polygon.points[0].z)/H;

	VECTOR V;
	V.x = (polygon.points[3].x - polygon.points[0].x)/L;
	V.y = (polygon.points[3].y - polygon.points[0].y)/L;
	V.z = (polygon.points[3].z - polygon.points[0].z)/L;

	VECTOR d;
	d.x = intersection.x - polygon.points[0].x;
	d.y = intersection.y - polygon.points[0].y;
	d.z = intersection.z - polygon.points[0].z;

	POINT2D coord;
	coord.u = pointProduct(d, U)/H;
	coord.v = pointProduct(d, V)/L;

	return coord;
}



POINT2D getCylinderTexture(OBJECT obj, POINT intersection){
	CYLINDER cylinder = obj.cylinder;

	VECTOR d;
	d.x = intersection.x - cylinder.anchor.x;
	d.y = intersection.y - cylinder.anchor.y;
	d.z = intersection.z - cylinder.anchor.z;

//	d = normalizeVector(d);

	double u = acos(pointProduct(getN(obj, intersection), cylinder.G))/(2*PI);

	POINT2D coord;
	coord.v = pointProduct(cylinder.axis, d)/(cylinder.d2 - cylinder.d1);

	PEQUATION eq = cylinder.equation;
	double darkSide = eq.a*intersection.x + eq.b*intersection.y + eq.c*intersection.z + eq.d;

	if(darkSide < 0){
		u = 1 - u;
	}
	coord.u = u;

	return coord;
}