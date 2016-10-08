#ifndef OPERATIONS_H_   
#define OPERATIONS_H_

POINT mapXY(int x, int y, int xmax, int ymax, int xmin, int ymin);

double pointProduct(POINT v1, POINT v2);

double getMagnitude(POINT vector);

VECTOR normalizeVector(POINT vector);

VECTOR getL(POINT intersection, LIGHT light);

#endif
