#include "objects.h"
#include <math.h>

long double min(long double val1, long double val2){ // Calcula el minimo entre dos valores
	if(val1 < val2){
		return val1;
	}
	return val2;
}

POINT mapXY(int i, int j, int xmax, int ymax, int xmin, int ymin){
	POINT point;
	point.z = 0.0;
	
	point.x = (((i + 0.5) * (xmax - xmin)) / H_SIZE) + xmin;
	point.y = (((j + 0.5) * (ymax - ymin)) / V_SIZE) + ymin;

	return point;
}

double pointProduct(VECTOR v1, VECTOR v2){
	double x = v1.x * v2.x;
	double y = v1.y * v2.y;
	double z = v1.z * v2.z;
	
	return x + y + z;
}

double getMagnitude(VECTOR vector){
	double magnitude = sqrt( pow(vector.x,2) + pow(vector.y,2) + pow(vector.z,2) );
	return magnitude;
}

VECTOR normalizeVector(VECTOR vector){
	double magnitude = getMagnitude(vector);
	vector.x = vector.x / magnitude;
	vector.y = vector.y / magnitude;
	vector.z = vector.z / magnitude;

	return vector;
}

VECTOR getL(POINT intersection, LIGHT light){
	VECTOR L;
	L.x = light.location.x - intersection.x;
	L.y = light.location.y - intersection.y; 
	L.z = light.location.z - intersection.z;
	
	// Normaliza el vector
	L = normalizeVector(L);
	return L;
}
	

VECTOR pointToVector(POINT point){
	VECTOR vector;
	vector.x = point.x;
	vector.y = point.y;
	vector.z = point.z;
	return vector;
}


