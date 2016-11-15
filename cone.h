#ifndef CONE_H_
#define CONE_H_

#include <stdbool.h>

OBJECT createCone(double, POINT, VECTOR, double,
                  double, COLOR, double, double, long double,
                  long double, long double, long double, long double,
                  long double, long double, long double);

INTERSECTION findIntersection_cone(VECTOR, POINT, POINT, double, VECTOR,
                                    double,double,double, double);

bool verifyFinitePoint(POINT intersectionPoint, double d1, double d2, POINT o, VECTOR q);
#endif
