#ifndef SPHERE_H_   
#define SPHERE_H_
#define EPSILON 0.00005

OBJECT createSphere(double radius, POINT center, COLOR color, long double kd, long double ka);
INTERSECTION findIntersection_sphere(VECTOR direction, POINT eye, POINT center, double radius);


#endif
