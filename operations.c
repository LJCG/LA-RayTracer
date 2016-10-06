#include "objects.h"
#include <math.h>


POINT mapXY(int x, int y, int xmax, int ymax, int xmin, int ymin){
	POINT point;
	point.z = 0.0;
	
	point.x = (((x + 0.5) * (xmax - xmin)) / H_SIZE) + xmin;
	point.y = (((y + 0.5) * (ymax - ymin)) / V_SIZE) + ymin;

	return point;
}

double pointProduct(POINT v1, POINT v2){
	double x = v1.x * v2.x;
	double y = v1.y * v2.y;
	double z = v1.z * v2.z;
	
	return x + y + z;
}

double getMagnitude(POINT vector){
	double magnitude = sqrt( pow(vector.x,2) + pow(vector.y,2) + pow(vector.z,2) );
	return magnitude;
}

POINT normalizeVector(POINT vector){
	double magnitude = getMagnitude(vector);
	vector.x = vector.x / magnitude;
	vector.y = vector.y / magnitude;
	vector.z = vector.z / magnitude;

	return vector;
}

POINT getL(POINT intersection, LIGHT light){
	POINT L;
	L.x = light.x - intersection.x;
	L.y = light.y - intersection.y; 
	L.z = light.z - intersection.z;
	
	// Normaliza el vector
	L = normalizeVector(L);
	return L;
}
	




