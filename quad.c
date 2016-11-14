#include "objects.h"
#include "sphere.h"
#include <math.h>
#include <stdio.h>



OBJECT createQuad(COLOR color, long double kd, long double ka, long double kn, 
	                long double ks, long double o1, long double o2, double A, double B, double C,
					double D, double E, double F, double G, double H, double J, double K){
	QUAD quad;
	quad.A = A;
	quad.B = B;
	quad.C = C;
	quad.D = D;
	quad.E = E;
	quad.F = F;
	quad.G = G;
	quad.H = H;
    quad.J = J;
	quad.K = K;


	OBJECT newObject;
	newObject.id = 'Q';
	newObject.quad = quad;
	newObject.color = color;
	newObject.ka = ka;
	newObject.kd = kd;
	newObject.ks = ks;
	newObject.kn = kn;
	newObject.o1 = o1;
	newObject.o2 = o2;

	return newObject;
}

INTERSECTION findIntersection_quad(VECTOR d, POINT eye, QUAD quad){
	INTERSECTION intersection;
	
	double a = quad.A*pow(d.x,2) + quad.B*pow(d.y,2) + quad.C*pow(d.z,2) + 2*(quad.D*d.x*d.y + quad.E*d.y*d.z + quad.F*d.x*d.z);

	double b = 2*(quad.A*eye.x*d.x + quad.B*eye.y*d.y + quad.C*eye.z*d.z + quad.D*eye.x*d.y + quad.D*eye.y*d.x 
				  + quad.E*eye.y*d.z + quad.E*eye.z*d.y + quad.F*eye.z*d.x + quad.F*eye.x*d.z + quad.G*d.x 
				  + quad.H*d.y + quad.J*d.z);

	double c = quad.A*pow(eye.x, 2) + quad.B*pow(eye.y, 2) + quad.C*pow(eye.z, 2) 
			   + 2*(quad.D*eye.x*eye.y + quad.E*eye.y*eye.z + quad.F*eye.z*eye.x
			        + quad.G*eye.x + quad.H*eye.y + quad.J*eye.z) + quad.K;
	
	double discriminante = pow(b, 2) - (4 * a * c);
	//printf("%lf\n", discriminante);
	if(discriminante < EPSILON){ // no hay interseccion
		intersection.tmin = 0;
		intersection.tmax = 0;
		intersection.flag = 0;

	}
	else if(discriminante > EPSILON){ // hay dos intersecciones
		double t1 = (-b + sqrt(discriminante))/2*a;
		double t2 = (-b - sqrt(discriminante))/2*a;

		if(t1 < t2){
			intersection.tmin = t1;
			intersection.tmax = t2;
		}
		else{
			intersection.tmin = t2;
			intersection.tmax = t1;
		}
		intersection.flag = 1;
		//printf("%lf\n",intersection.tmin);
	}
	else{ // hay una interseccion

		intersection.tmin = -b/2*a;
		intersection.tmax = 0;
		intersection.flag = 1;
	}
	
	return intersection;
}



