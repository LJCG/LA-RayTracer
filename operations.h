#ifndef OPERATIONS_H_   
#define OPERATIONS_H_

long double min(long double val1, long double val2);

POINT mapXY(int x, int y, int xmax, int ymax, int xmin, int ymin);

double pointProduct(VECTOR v1, VECTOR v2);

double getMagnitude(POINT vector);

VECTOR normalizeVector(POINT vector);

VECTOR getL(POINT intersection, LIGHT light);

VECTOR pointToVector(POINT point);

#endif
