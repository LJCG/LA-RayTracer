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



