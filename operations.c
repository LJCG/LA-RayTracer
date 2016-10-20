#include "objects.h"
#include <math.h>

// Construye la normal a partir de la ecuación del polígono
VECTOR eq2vector(PEQUATION eq){
	VECTOR normal;
	normal.x = eq.a;
	normal.y = eq.b;
	normal.z = eq.c;

	return normal;
}

long double min(long double val1, long double val2){ // Calcula el minimo entre dos valores
	if(val1 < val2){
		return val1;
	}
	return val2;
}

char max(double a, double b, double c){ // Calcula el máximo entre tres valores
     char coord = 'a';
     double m = a;
     (m < b) && (m = b) && (coord = 'b'); 
     (m < c) && (m = c) && (coord = 'c');
     return coord;
}

POINT mapXY(int i, int j, int xmax, int ymax, int xmin, int ymin){
	POINT point;

	point.x = (((i + 0.5) * (xmax - xmin)) / H_SIZE) + xmin;
	point.y = (((j + 0.5) * (ymax - ymin)) / V_SIZE) + ymin;
	point.z = 0.0;
	return point;
}

double pointProduct(VECTOR v1, VECTOR v2){
	double x = v1.x * v2.x;
	double y = v1.y * v2.y;
	double z = v1.z * v2.z;
	
	return x + y + z;
}

VECTOR cruxProduct(VECTOR v, VECTOR w){
	VECTOR result;
	result.x = (v.y * w.z) - (v.z * w.y);
	result.y = (v.z * w.x) - (v.x * w.z);
	result.z = (v.x * w.y) - (v.y * w.x);

	return result;
}

double getMagnitude(VECTOR vector){
	double magnitude = sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
	return magnitude;
}

double getDistance(POINT p1, POINT p2){
	VECTOR distance;
	distance.x = p2.x - p1.x;
	distance.y = p2.y - p1.y;
	distance.z = p2.z - p1.z;

	return getMagnitude(distance);
}

VECTOR getVector(POINT p1, POINT p2){
	VECTOR vector;
	vector.x = p2.x - p1.x;
	vector.y = p2.y - p1.y;
	vector.z = p2.z - p1.z;

	return vector;
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

VECTOR getN(OBJECT obj, POINT intersection){
	VECTOR N;
	if(obj.id == 'S'){
		N.x = (intersection.x - obj.sphere.center.x)/obj.sphere.radius;
		N.y = (intersection.y - obj.sphere.center.y)/obj.sphere.radius; 
		N.z = (intersection.z - obj.sphere.center.z)/obj.sphere.radius;
	}
	else if(obj.id == 'P'){
		N = eq2vector(obj.polygon.equation);
	}
	
	return N;
}
	

VECTOR pointToVector(POINT point){
	VECTOR vector;
	vector.x = point.x;
	vector.y = point.y;
	vector.z = point.z;
	return vector;
}

POINT getIntersectionPoint(VECTOR vectorW, VECTOR vectorD, double t){
	POINT point;

	point.x = vectorW.x + t*vectorD.x;
	point.y = vectorW.y + t*vectorD.y;
	point.z = vectorW.z + t*vectorD.z;
	return point;
}


