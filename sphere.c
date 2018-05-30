#include "objects.h"
#include "sphere.h"
#include <math.h>
#include <stdio.h>



OBJECT createSphere(double radius, POINT center, COLOR color, long double kd, long double ka,
					long double kn, long double ks, long double kr, long double o1, long double o2, long double o3){
	SPHERE sphere;
	sphere.radius = radius;
	sphere.center = center;


	OBJECT newObject;
	newObject.id = 'S';
	newObject.sphere = sphere;
	newObject.color = color;
	newObject.ka = ka;
	newObject.kd = kd;
	newObject.ks = ks;
	newObject.kn = kn;
	newObject.kr = kr; //coeficiente de refraccion
	newObject.o1 = o1;
	newObject.o2 = o2;
	newObject.o3 = o3;

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
