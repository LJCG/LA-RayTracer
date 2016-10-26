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
	cylinder.axis = axis; // EJE
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
    
	double a = ((pow(q.x, 2)+pow(q.y, 2)+pow(q.z, 2)-2)*pow((d.x*q.x + d.y*q.y + d.z*q.z), 2)+pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2));

	double b = 2*((q.x*(d.x*q.x + d.y*q.y + d.z*q.z) - d.x)*((o.x-e.x)*(1 - pow(q.x, 2)) + q.x*(q.y*(e.y-o.y)) + q.z*(e.z-o.z)) +
	  			  (q.y*(d.x*q.x + d.y*q.y + d.z*q.z) - d.y)*((o.y-e.y)*(1 - pow(q.y, 2)) + q.y*(q.x*(e.x-o.x)) + q.z*(e.z-o.z)) +
	   			  (q.z*(d.x*q.x + d.y*q.y + d.z*q.z) - d.z)*((o.z-e.z)*(1 - pow(q.z, 2)) + q.z*(q.x*(e.x-o.x)) + q.y*(e.y-o.y)));

	double c = pow(((o.x-e.x)*(1 - pow(q.x, 2))+ q.x*(e.y*q.y - o.y*q.y + e.z*q.z - o.z*q.z)), 2) +
    		   pow(((o.y-e.y)*(1 - pow(q.y, 2))+ q.y*(e.x*q.x - o.x*q.x + e.z*q.z - o.z*q.z)), 2) +
    		   pow(((o.z-e.z)*(1 - pow(q.z, 2))+ q.z*(e.x*q.x - o.x*q.x + e.y*q.y - o.y*q.y)), 2) - pow(radius, 2);
	
	printf("r: %lf\n", radius);
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
			intersection.tmin = 0;
			intersection.tmax = 0;
			intersection.flag = 0;
		}
	}
	return intersection;
}