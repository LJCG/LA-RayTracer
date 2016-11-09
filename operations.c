#include "objects.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

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

     if(a >= b && a >= c){
     	return 'a';
     }

     else if(b >= a && b >= c){
     	return 'b';
     }

     return 'c';
}

POINT mapXY(int i, int j, int xmax, int ymax, int xmin, int ymin, float iValue, float jValue){
	POINT point;

	point.x = (((i + iValue) * (xmax - xmin)) / H_SIZE) + xmin;
	point.y = (((j + jValue) * (ymax - ymin)) / V_SIZE) + ymin;
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
	else if(obj.id == 'C'){
		POINT o = obj.cylinder.anchor;
		VECTOR q = obj.cylinder.axis;
		double x = intersection.x;
		double y = intersection.y;
		double z = intersection.z;

		N.x = 2*(pow(q.x, 2)-1)*(q.x*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.x-x)+
			  2*q.x*q.y*(q.y*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.y-y)+
			  2*q.x*q.z*(q.z*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.z-z);

		N.y = 2*(pow(q.y, 2)-1)*(q.y*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.y-y)+
			  2*q.x*q.y*(q.x*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.x-x)+
			  2*q.y*q.z*(q.z*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.z-z);

		N.z = 2*(pow(q.z, 2)-1)*(q.z*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.z-z)+
			  2*q.x*q.z*(q.x*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.x-x)+
			  2*q.y*q.z*(q.y*(q.x*(x-o.x)+q.y*(y-o.y)+q.z*(z-o.z))+o.y-y);

		N = normalizeVector(N);
	}
	else if(obj.id == 'N'){
		POINT o = obj.cone.anchor;
		VECTOR q = obj.cone.axis;
		double x = intersection.x;
		double y = intersection.y;
		double z = intersection.z;
		double xl,yl,zl,l,H,lq;

		xl = x-o.x;
		yl = y-o.y;
		zl = z-o.z;

		//AQUI SE DESPICHA LA HOSTIA

		l = sqrt(pow(xl,2)+pow(yl,2)+pow(zl,2));

		xl = xl/l;
		yl = yl/l;
		zl = zl/l;

		lq = (q.x*xl + q.y*yl + q.z*zl);


		H = l/lq;

		N.x = x-(o.x + H*q.x);
		N.y = y-(o.y + H*q.y);
		N.z = z-(o.z + H*q.z);

		l = sqrt(pow(N.x,2) + pow(N.y,2) + pow(N.z,2));

		N.x = N.x/l;
		N.y = N.y/l;
		N.z = N.z/l;


		N = normalizeVector(N);
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

VECTOR numberByVector(VECTOR vector, double number){
	vector.x = vector.x * number;
	vector.y = vector.y * number;
	vector.z = vector.z * number;
	return vector;
}

VECTOR substractVectors(VECTOR v1, VECTOR v2){
	v1.x = v1.x - v2.x;
	v1.y = v1.y - v2.y;
	v1.z = v1.z - v2.z;
	return v1;
}

POINT getIntersectionPoint(VECTOR vectorW, VECTOR vectorD, double t){
	POINT point;

	point.x = vectorW.x + t*vectorD.x;
	point.y = vectorW.y + t*vectorD.y;
	point.z = vectorW.z + t*vectorD.z;
	return point;
}

VECTOR rotate_cone(VECTOR axis, int grados){
//Pasa de radianes a grados primero

 VECTOR axis_aux;
 double grados_cos = cos( grados*(PI / 180.0) );
 double grados_sen = sin( grados*(PI / 180.0) );
 axis_aux.x = (grados_cos * axis.x) - (grados_sen * axis.y);
 axis_aux.y = (grados_sen * axis.x) + (grados_cos * axis.y);
 axis_aux.z = 0;

 return axis_aux;
}

int sameColor(COLOR c1, COLOR c2){
	if((c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b)){
		return 1;
	}

	return 0;
}

double colorDistance(COLOR c1, COLOR c2){
	double distance = sqrt(pow((c1.r - c2.r),2) + pow((c1.g - c2.g),2) + pow((c1.b - c2.b),2));
	return distance;
}

COLOR avgColor(COLOR c1, COLOR c2, COLOR c3, COLOR c4){

	COLOR average;
	average.r = (c1.r + c2.r + c3.r + c4.r) / 4;
	average.g = (c1.g + c2.g + c3.g + c4.g) / 4;
	average.b = (c1.b + c2.b + c3.b + c4.b) / 4;

	return average;
}
