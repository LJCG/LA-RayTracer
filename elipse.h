#ifndef ELIPSE_H_   
#define ELIPSE_H_

#include <stdbool.h>
OBJECT createElipse(double k, POINT F1, POINT F2, COLOR color, long double kd, long double ka, long double ks,
					long double kn, long double o1, long double o2, long double o3);

bool verifyPointE(POINT interPoint, double k, POINT F1, POINT F2);

INTERSECTION findIntersection_elipse(VECTOR d, POINT eye, ELIPSE elipse);

#endif