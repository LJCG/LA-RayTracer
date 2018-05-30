#ifndef CYLINDER_H_   
#define CYLINDER_H_

#include <stdbool.h>

OBJECT createCylinder(double radius, POINT anchor, VECTOR axis, double d1, double d2, COLOR color, long double kd, long double ka, 
					  long double kn, long double ks, long double o1, long double o2);

bool verifyFinitePoint(POINT intersectionPoint, double d1, double d2, POINT o, VECTOR q);

INTERSECTION findIntersection_cylinder(VECTOR d, POINT e, POINT o, double radius, VECTOR q, double d1, double d2);
#endif
