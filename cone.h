#ifndef CONE_H_
#define CONE_H_


OBJECT createCone(double, POINT, VECTOR, double,
                  double, COLOR, double, double, long double,
                  long double, long double, long double);

INTERSECTION findIntersection_cone(VECTOR, POINT, POINT, double, VECTOR,
                                    double,double,double, double);

long double getNormalCone(void);


#endif
