#include "objects.h"
#include "disk.h"
#include "operations.h"
#include "polygon.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

OBJECT createDisk(double radius, POINT center, COLOR color, POINT p1, POINT p2, long double kd, long double ka, 
	long double ks, long double kn, long double o1, long double o2, long double o3){
	
	DISK disk;
	disk.radius = radius;
	disk.center = center;

	POINT points[3];
	points[0] = center;
	points[1] = p1;
	points[2] = p2;

	disk.equation = getABCD(points);

	disk.equation.a = disk.equation.a * -1;
	disk.equation.b = disk.equation.b * -1;
	disk.equation.c = disk.equation.c * -1;
	disk.equation.d = disk.equation.d * -1;

	OBJECT newObject;
	newObject.id = 'D';
	newObject.disk = disk;
	newObject.color = color;
	newObject.ka = ka;
	newObject.kd = kd;
	newObject.ks = ks;
	newObject.kn = kn;
	newObject.o1 = o1;
	newObject.o2 = o2;
	newObject.o2 = o3;


	return newObject;
}


bool verifyPointD(POINT interPoint, POINT center, double radius){
	double distance = pow((interPoint.x - center.x),2) + pow((interPoint.y - center.y),2) + pow((interPoint.z - center.z),2) - pow(radius,2);
	if(distance <= 0){
		return true;
	}
	return false;
}

INTERSECTION findIntersection_disk(VECTOR direction, POINT eye, DISK disk){
	VECTOR norm = eq2vector(disk.equation); // Construye la normal a partir de la ecuación del disco
	INTERSECTION intersection;

	// Primera fase: Revisa si hay intersección con el plano
	if(pointProduct(norm, direction) > EPSILON){ // Hay interseccion	

		// Calcula t
		double t = -(norm.x*eye.x + norm.y*eye.y + norm.z*eye.z + disk.equation.d)/(norm.x*direction.x + norm.y*direction.y + norm.z*direction.z);		
		POINT intersectionPoint = getIntersectionPoint(pointToVector(eye), direction, t);

		// Segunda fase: Revisa si el punto está en el interior del círculo
		if(verifyPointD(intersectionPoint, disk.center, disk.radius)){
			intersection.tmin = t;
			intersection.tmax = 0;
			intersection.flag = 1;
			
		}
		else{
			intersection.tmin = 0;
			intersection.tmax = 0;
			intersection.flag = 0;
		}
	}
	else{
		intersection.tmin = 0;
		intersection.tmax = 0;
		intersection.flag = 0;
	}
	return intersection;
}

