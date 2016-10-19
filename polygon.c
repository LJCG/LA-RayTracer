#include "objects.h"
#include "operations.h"


OBJECT createPolygon(POINT *vertices, int numVertices, COLOR color, long double kd, long double ka){
	POLYGON p;
	p.points = vertices;
	p.sizePoints = numVertices;

	OBJECT newObject;
	newObject.id = 'P';
	newObject.polygon = p;
	newObject.color = color;
	newObject.ka = ka;
	newObject.kd = kd;

	return newObject;
}

// void flattenPolygon(){}

double getD(VECTOR normal, POINT punto){
	// D = -Ax -Bx -Cx
	return (-normal.x * punto.x) - (-normal.y * punto.y) - (-normal.z * punto.z);
}

INTERSECTION findIntersection_polygon(VECTOR direction, POINT eye, POINT *points){
	VECTOR v1;
	v1.x = points[2].x - points[0].x;
	v1.y = points[2].x - points[0].y;
	v1.z = points[2].x - points[0].z;

	VECTOR v2;
	v2.x = points[2].x - points[1].x;
	v2.y = points[2].x - points[1].y;
	v2.z = points[2].x - points[1].z;

	// Obtener la ecuación del plano
	VECTOR cruxP = cruxProduct(v1, v2); 							// Obtiene la normal [A, B, C]
	VECTOR ncruxP = normalizeVector(cruxP); 						// Normaliza la normal

	INTERSECTION intersection;
	if(pointProduct(ncruxP, direction) > EPSILON){ // hay interseccion	
		double nD = getD(cruxP, points[2])/getMagnitude(ncruxP);  // Obtiene D y lo normaliza
		// Calcular t
		double t = -(ncruxP.x*eye.x + ncruxP.y*eye.y + ncruxP.z*eye.z + nD)/(ncruxP.x*direction.x + ncruxP.y*direction.y + ncruxP.z*direction.z);		
		intersection.tmin = t;
		intersection.tmax = 0;
		intersection.flag = 1;
	}
	else{
		intersection.tmin = 0;
		intersection.tmax = 0;
		intersection.flag = 0;
	}
	return intersection;

}



