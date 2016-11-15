#ifndef DISK_H_   
#define DISK_H_

#include <stdbool.h>

OBJECT createDisk(double radius, POINT center, COLOR color, POINT p1, POINT p2, long double kd,
				 long double ka, long double ks, long double kn, long double o1, long double o2, long double o3);

bool verifyPointD(POINT interPoint, POINT center, double radius);

INTERSECTION findIntersection_disk(VECTOR d, POINT eye, DISK disk);

#endif