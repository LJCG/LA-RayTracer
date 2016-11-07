#include "objects.h"
#include "operations.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

// Obtiene el valor de D
double getD(VECTOR normal, POINT punto){
	// D = -Ax -Bx -Cx
	return (-normal.x * punto.x) - (normal.y * punto.y) - (normal.z * punto.z);
}

// Calcula la ecuación del polígono
PEQUATION getABCD(POINT *points){
	VECTOR v1 = getVector(points[0], points[2]);
	VECTOR v2 = getVector(points[1], points[2]);

	VECTOR cruxP = cruxProduct(v1, v2);     // Obtiene la normal [A, B, C]
	VECTOR ncruxP = normalizeVector(cruxP); // Normaliza la normal

	PEQUATION eq;
	eq.a = ncruxP.x;
	eq.b = ncruxP.y;
	eq.c = ncruxP.z;
	eq.d = getD(cruxP, points[2])/getMagnitude(cruxP);


	return eq;
}

// Aplasta el polígono a 2D
void flattenPolygon(POLYGON p){
	int i;
	char m = p.tag;
	for(i = 0; i < p.sizePoints; i++){
		if(m == 'a'){
			p.points2D[i].u = p.points[i].y;
			p.points2D[i].v = p.points[i].z;
		
		}
		else if(m == 'b'){
			p.points2D[i].u = p.points[i].x;
			p.points2D[i].v = p.points[i].z;
			
		}
		else if(m == 'c'){
			p.points2D[i].u = p.points[i].x;
			p.points2D[i].v = p.points[i].y;
		}
	}
}

// Crea un polígono y lo agrega a un objeto
OBJECT createPolygon(POINT *vertices, int numVertices, COLOR color, long double kd, long double ka, 
				     long double ks, long double kn, long double o1, long double o2){
	POLYGON p;
	p.points = vertices;
	p.sizePoints = numVertices;
	p.equation = getABCD(p.points); // GUARDA LA ECUACIÓN DEL POLIGONO
	p.tag = max(fabs(p.equation.a), fabs(p.equation.b), fabs(p.equation.c));
	p.points2D = (POINT*)malloc(p.sizePoints*sizeof(POINT));        // APLASTA EL POLÍGONO
	flattenPolygon(p);

	OBJECT newObject;
	newObject.id = 'P';
	newObject.polygon = p;
	newObject.color = color;
	newObject.ka = ka;
	newObject.kd = kd;
	newObject.ks = ks;
	newObject.kn = kn;
	newObject.o1 = o1;
	newObject.o2 = o2;

	return newObject;
}

//------------------------------------------- CALCULAR INTERSECCIONES ------------------------------------------
POINT2D flattenPoint(POINT intersectionPoint, char tag){
	POINT2D newIntersectionPoint;
	if(tag == 'a'){
		newIntersectionPoint.u = intersectionPoint.y;
		newIntersectionPoint.v = intersectionPoint.z;
	}
	else if(tag == 'b'){
		newIntersectionPoint.u = intersectionPoint.x;
		newIntersectionPoint.v = intersectionPoint.z;
	}
	else if(tag == 'c'){
		newIntersectionPoint.u = intersectionPoint.x;
		newIntersectionPoint.v = intersectionPoint.y;
	}
	return newIntersectionPoint;
}

// Traslada los puntos al origen
void translatePoints(POINT2D* points, int sizePoints, POINT2D intersectionPoint){
	int i;
	for(i = 0; i<sizePoints; i++){
		points[i].u = points[i].u - intersectionPoint.u;
		points[i].v = points[i].v - intersectionPoint.v;
	}
}

int countEdges(POINT2D *points2D,  int sizePoints){
	int numEdges = 0;
	int i;
	POINT2D p1, p2;

    for(i = 0; i < sizePoints; i++){
      	if(i == sizePoints-1){
      		p1 = points2D[i];
      		p2 = points2D[0];
      	}
      	else{
      		p1 = points2D[i];
      		p2 = points2D[i+1];
      	}

      	int u_positive = p1.u >= 0 && p2.u >= 0;
      	int u_different = (p1.u >= 0 && p2.u < 0) || (p1.u < 0 && p2.u >= 0);
      	int v_different = (p1.v >= 0 && p2.v < 0) || (p1.v < 0 && p2.v >= 0);

      	if(u_positive && v_different){
      		numEdges++;
      	}
      	else if(u_different && v_different){
      		long double m = (p2.v - p1.v) / (p2.u - p1.u);
      		long double b = p2.v - m * p2.u;

      		long double x = -b/m;

      		if(x >= 0){
      			numEdges++;
      		}
      	} 
    }

    return (numEdges % 2);
}

bool verifyPoint(POINT2D *points2D, int sizePoints, POINT2D intersectionPoint){
	int i;
	POINT2D * newPoints2D = (POINT2D*)malloc(sizePoints*sizeof(POINT2D));
	for(i = 0; i < sizePoints; i++){
		newPoints2D[i].u = points2D[i].u;
		newPoints2D[i].v = points2D[i].v;
	}
	translatePoints(newPoints2D, sizePoints, intersectionPoint);

	if(countEdges(newPoints2D, sizePoints) == 1){ // Si la cantidad es impar
		return true;
	}
	return false;
}

PEQUATION reverse(POLYGON p){
	PEQUATION newEq;
	newEq.a = p.equation.a * -1;
	newEq.b = p.equation.b * -1;
	newEq.c = p.equation.c * -1;
	newEq.d = p.equation.d * -1;

	return newEq;
}

INTERSECTION findIntersection_polygon(VECTOR direction, POINT eye, POLYGON p){
	VECTOR norm = eq2vector(p.equation); // Construye la normal a partir de la ecuación del polígono
	INTERSECTION intersection;

	// Primera fase: Revisa si hay intersección con el plano
	if(pointProduct(norm, direction) > EPSILON){ // Hay interseccion	

		// Calcula t
		double t = -(norm.x*eye.x + norm.y*eye.y + norm.z*eye.z + p.equation.d)/(norm.x*direction.x + norm.y*direction.y + norm.z*direction.z);		
		POINT intersectionPoint = getIntersectionPoint(pointToVector(eye), direction, t);

		// Segunda fase: Revisa si el punto está en el interior del polígono
		if(verifyPoint(p.points2D, p.sizePoints, flattenPoint(intersectionPoint, p.tag))){
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


