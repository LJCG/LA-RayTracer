#ifndef OPERATIONS_H_   
#define OPERATIONS_H_

int foo(int x);

POINT mapXY(int x, int y, int xmax, int ymax, int xmin, int ymin);

double pointProduct(POINT v1, POINT v2);

double getMagnitude(POINT vector);

POINT normalizeVector(POINT vector);

#endif
