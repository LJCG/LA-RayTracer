#ifndef OPERATIONS_H_
#define OPERATIONS_H_

long double min(long double val1, long double val2);

char max(double a, double b, double c);

POINT mapXY(int x, int y, int xmax, int ymax, int xmin, int ymin);

double pointProduct(VECTOR v1, VECTOR v2);

VECTOR cruxProduct(VECTOR v, VECTOR w);

double getMagnitude(VECTOR vector);

double getDistance(POINT p1, POINT p2);

VECTOR getVector(POINT p1, POINT p2);

VECTOR normalizeVector(VECTOR vector);

VECTOR getL(POINT intersection, LIGHT light);

VECTOR getN(OBJECT obj, POINT intersection);

VECTOR pointToVector(POINT point);

VECTOR numberByVector(VECTOR vector, double number);

VECTOR substractVectors(VECTOR v1, VECTOR v2);

POINT getIntersectionPoint(VECTOR vectorW, VECTOR vectorD, double t);

VECTOR eq2vector(PEQUATION eq);

VECTOR rotate_cone(VECTOR axis, int grados);

#endif
