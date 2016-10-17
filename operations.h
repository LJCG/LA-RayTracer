#ifndef OPERATIONS_H_   
#define OPERATIONS_H_

long double min(long double val1, long double val2);

POINT mapXY(int x, int y, int xmax, int ymax, int xmin, int ymin);

double pointProduct(VECTOR v1, VECTOR v2);

double getMagnitude(VECTOR vector);

double getDistance(POINT p1, POINT p2);

VECTOR normalizeVector(VECTOR vector);

VECTOR getL(POINT intersection, LIGHT light);

VECTOR getN(OBJECT obj, POINT intersection);

VECTOR pointToVector(POINT point);

#endif
