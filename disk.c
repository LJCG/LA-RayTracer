#include "objects.h"
#include "disk.h"
#include "operations.h"
#include "polygon.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

OBJECT createDisk(double radius, POINT center, COLOR color, POINT p1, POINT p2){
	
	DISK disk;
	disk.radius = radius;
	disk.center = center;

	POINT points[3];
	points[0] = center;
	points[1] = p1;
	points[2] = p2;

	disk.equation = getABCD(points);


	OBJECT newObject;
	newObject.id = 'D';
	newObject.disk = disk;
	newObject.color = color;

	return newObject;
}

INTERSECTION findIntersection_disk(VECTOR d, POINT eye, DISK disk){
	
	INTERSECTION intersection;
	POINT interPoint, center;
	double pointProd, t;

	pointProd = pointProduct(d, eq2vector(disk.equation));
	center = disk.center;

	if(pointProd == 0){
		intersection.flag = 0;
		intersection.tmax = 0;
		intersection.tmin = 0;
	}

	else{
		PEQUATION eq = disk.equation;
		t = -(eq.a*eye.x + eq.b*eye.y + eq.c*eye.z)/(eq.a*d.x + eq.b*d.y + eq.c*d.z);

		interPoint = getIntersectionPoint(pointToVector(eye), d, t);

		double distance = pow((interPoint.x - center.x),2) + pow((interPoint.y - center.y),2) 
						  + pow((interPoint.z - center.z),2) - pow(disk.radius,2);

		if(distance <= 0){
			intersection.flag = 1;
			intersection.tmin = t;
		}

		else{
			intersection.flag = 0;
			intersection.tmax = 0;
			intersection.tmin = 0;
		}

	}

	return intersection;
}



