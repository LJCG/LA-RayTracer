#include "objects.h"
#include "operations.h"
#include "textures.h"
#include <stdio.h>
#include <math.h>

POINT2D getRectangleTexture(OBJECT obj, POINT intersection){

	double H = getDistance(obj.polygon.points[0], obj.polygon.points[1]);
	double L = getDistance(obj.polygon.points[0], obj.polygon.points[3]);

	VECTOR U;
	U.x = (obj.polygon.points[1].x - obj.polygon.points[0].x)/H;
	U.y = (obj.polygon.points[1].y - obj.polygon.points[0].y)/H;
	U.z = (obj.polygon.points[1].z - obj.polygon.points[0].z)/H;

	VECTOR V;
	V.x = (obj.polygon.points[3].x - obj.polygon.points[0].x)/L;
	V.y = (obj.polygon.points[3].y - obj.polygon.points[0].y)/L;
	V.z = (obj.polygon.points[3].z - obj.polygon.points[0].z)/L;

	VECTOR d;
	d.x = intersection.x - obj.polygon.points[0].x;
	d.y = intersection.y - obj.polygon.points[0].y;
	d.z = intersection.z - obj.polygon.points[0].z;

	POINT2D coord;
	coord.u = pointProduct(d, U)/H;
	coord.v = pointProduct(d, V)/L;

	return coord;
}
