#include "objects.h"
#include "operations.h"
#include "cylinder.h"
#include <math.h>
#include <stdio.h>

OBJECT createCylinder(double radius, POINT anchor, VECTOR axis, double d1, double d2, COLOR color, long double kd, long double ka,
					  long double kn, long double ks){
	CYLINDER cylinder;
	cylinder.radius = radius;
	cylinder.anchor = anchor; 			   // ANCLA
	cylinder.axis = normalizeVector(axis); // EJE
	cylinder.d1 = d1;		 			   // TAPA 1
	cylinder.d2 = d2;	     			   // TAPA 2


	OBJECT newObject;
	newObject.id = 'C';
	newObject.cylinder = cylinder;
	newObject.color = color;
	newObject.ka = ka;
	newObject.kd = kd;
	newObject.ks = ks;
	newObject.kn = kn;

	return newObject;
}

bool verifyFinitePoint(POINT intersectionPoint, double d1, double d2, POINT o, VECTOR q){
	double d = (intersectionPoint.x - o.x)*q.x + (intersectionPoint.y - o.y)*q.y + (intersectionPoint.z - o.z)*q.z;
	if(d1 <= d && d <= d2){
		return true;
	}
	return false;
}

INTERSECTION findIntersection_cylinder(VECTOR d, POINT e, POINT o, double radius, VECTOR q, double d1, double d2){
	// d es direction, e es ojo, o es el ancla, q es el eje. Se puso así para que la formula no quede enorme
    INTERSECTION intersection;

	long double var1,var2,var3,var4,var5,var6;

	var1 = (pow(q.x,2)*d.x)+(q.x*d.y*q.y)+(q.x*d.z*q.z)-d.x;
	var2 = (pow(q.y,2)*d.y)+(d.x*q.x*q.y)+(q.y*d.z*q.z)-d.y;
	var3 = (pow(q.z,2)*d.z)+(d.x*q.x*q.z)+(d.y*q.y*q.z)-d.z;
	var4 = (e.x-o.x);
	var5 = (e.y-o.y);
	var6 = (e.z-o.z);


	double a = pow(var1,2)+pow(var2,2)+pow(var3,2);

	double b = (2*var1*var4*(pow(q.x,2)-1))+(2*var1*q.x*((q.y*var5)+(q.z*var6)))
	+(2*var2*var5*(pow(q.y,2)-1))+(2*var2*q.y*((q.x*var4)+(q.z*var6)))
	+(2*var3*var6*(pow(q.z,2)-1))+(2*var3*q.z*((q.x*var4)+(q.y*var5)));

	double c = pow(((pow(q.x,2)-1)*var4),2)+pow((q.x*((q.y*var5)+(q.z*var6))),2)
	+pow(((pow(q.y,2)-1)*var5),2)+pow((q.y*((q.x*var4)+(q.z*var6))),2)
	+pow(((pow(q.z,2)-1)*var6),2)+pow((q.z*((q.x*var4)+(q.y*var5))),2)
	+(2*var4*q.x*(pow(q.x,2)-1)*((q.y*var5)+(q.z*var6)))
	+(2*var5*q.y*(pow(q.y,2)-1)*((q.x*var4)+(q.z*var6)))
	+(2*var6*q.z*(pow(q.z,2)-1)*((q.x*var4)+(q.y*var5)))-pow(radius,2);

    double discriminante = pow(b, 2) - 4*a*c;

	if(discriminante < EPSILON){ // No hay interseccion con el cilindro INFINITO
		intersection.tmin = 0;
		intersection.tmax = 0;
		intersection.flag = 0;
	}
	else{
		if(discriminante > EPSILON){ // Hay dos intersecciones
			double t1 = (-b + sqrt(discriminante))/2;
			double t2 = (-b - sqrt(discriminante))/2;

			if(t1 < t2){
				intersection.tmin = t1;
				intersection.tmax = t2;
			}
			else{
				intersection.tmin = t2;
				intersection.tmax = t1;
			}
			intersection.flag = 1;
		}
		else{ // Hay una interseccion
			double temp = (-b + sqrt(discriminante))/2;

			if(temp != EPSILON){
				intersection.tmin = temp;
			}
			else{
				intersection.tmin = (-b - sqrt(discriminante))/2;;
			}
			intersection.tmax = 0;
			intersection.flag = 1;
		}
		
		if(!verifyFinitePoint(getIntersectionPoint(pointToVector(e), d, intersection.tmin), d1, d2, o, q)){ // No hay intersección con el FINITO
			
			if(verifyFinitePoint(getIntersectionPoint(pointToVector(e), d, intersection.tmax), d1, d2, o, q)){
				intersection.tmin = intersection.tmax;
				intersection.tmax = 0;
				intersection.flag = 1;
			}
			else{
				intersection.tmin = 0;
				intersection.tmax = 0;
				intersection.flag = 0;
			}
		}
		
	}
	return intersection;
}
