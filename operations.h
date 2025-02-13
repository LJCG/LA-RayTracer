#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include <stdbool.h>

long double min(long double val1, long double val2);

int minInt(int val1, int val2);

char max(double a, double b, double c);

POINT mapXY(int x, int y, int xmax, int ymax, int xmin, int ymin, float iValue, float jValue);

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

double vectorByVector(VECTOR N,VECTOR V);

VECTOR transparency(long double ot,double kr, VECTOR N, VECTOR V);

int sameColor(COLOR c1, COLOR c2);

double colorDistance(COLOR c1, COLOR c2);

COLOR avgColor(COLOR c1, COLOR c2, COLOR c3, COLOR c4);

VECTOR getG(VECTOR q);

POINT getZ(PEQUATION eq, double x, double y);

POINT getMidPoint(POINT p1, POINT p2);

bool isBlack(COLOR color);

#endif
