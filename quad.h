#ifndef QUAD_H_   
#define QUAD_H_


OBJECT createQuad(COLOR color, long double kd, long double ka, long double kn, 
	                long double ks, long double o1, long double o2, double A, double B, double C,
					double D, double E, double F, double G, double H, double J, double K);

INTERSECTION findIntersection_quad(VECTOR d, POINT eye, QUAD quad);

#endif
