#include "objects.h"
#include <math.h>
#include <stdio.h>
#define EPSILON 0.00005

OBJECT createSphere(double radius, POINT center, COLOR color, long double kd, long double ka){
	SPHERE sphere;
	sphere.radius = radius;
	sphere.center = center;

	OBJECT newObject;
	newObject.id = 'S';
	newObject.sphere = sphere;
	newObject.color = color;
	// newObject.ka = ka;
	// newObject.kd = kd;

	return newObject;
}

INTERSECTION findIntersection_sphere(VECTOR direction, POINT eye, POINT center, double radius){
	INTERSECTION intersection;
	
	// a = 1 en la esfera
	double b = 2*((direction.x * (eye.x - center.x)) + (direction.y * (eye.y - center.y)) + (direction.z * (eye.z - center.z)));
	double c =  pow((eye.x - center.x), 2) + pow((eye.y - center.y), 2) + pow((eye.z - center.z), 2) - pow(radius, 2);
	
	double discriminante = pow(b, 2) - (4 * c);
	//printf("%lf\n", discriminante);
	if(discriminante < EPSILON){ // no hay interseccion
		intersection.tmin = 0;
		intersection.tmax = 0;
		intersection.flag = 0;

	}
	else if(discriminante > EPSILON){ // hay dos intersecciones
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
	else{ // hay una interseccion
		intersection.tmin = -b/2;
		intersection.tmax = 0;
		intersection.flag = 1;
	}
	
	return intersection;
}

