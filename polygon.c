#include "objects.h"
#include "operations.h"
#include <stdbool.h>

// Obtiene el valor de D
double getD(VECTOR normal, POINT punto){
	// D = -Ax -Bx -Cx
	return (-normal.x * punto.x) - (-normal.y * punto.y) - (-normal.z * punto.z);
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
POLYGON flattenPolygon(POLYGON poly){
	int i;
	POINT2D newPoints[poly.sizePoints];
	char coord = max(abs(poly.equation.a), abs(poly.equation.b), abs(poly.equation.c));
	char m;

	for(i = 0; i<poly.sizePoints; i++){
		if(coord == 'a'){
			newPoints[i].u = poly.points[i].y;
			newPoints[i].v = poly.points[i].z;
			m = 'a';
		}
		else if(coord == 'b'){
			newPoints[i].u = poly.points[i].x;
			newPoints[i].v = poly.points[i].z;
			m = 'b';
		}
		else if(coord == 'c'){
			newPoints[i].u = poly.points[i].x;
			newPoints[i].v = poly.points[i].y;
			m = 'c';
		}
	}
	poly.points2D = newPoints;
	poly.tag = m;
	return poly;
}

// Crea un polígono y lo agrega a un objeto
OBJECT createPolygon(POINT *vertices, int numVertices, COLOR color, long double kd, long double ka){
	POLYGON p;
	p.points = vertices;
	p.sizePoints = numVertices;
	p.equation = getABCD(p.points); // GUARDA LA ECUACIÓN DEL POLIGONO
	p = flattenPolygon(p);        // APLASTA EL POLÍGONO

	OBJECT newObject;
	newObject.id = 'P';
	newObject.polygon = p;
	newObject.color = color;
	newObject.ka = ka;
	newObject.kd = kd;

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
POINT2D* translatePoints(POINT2D* points, int sizePoints, POINT2D intersectionPoint){
	int i;
	for(i = 0; i<sizePoints; i++){
		points[i].u = points[i].u - intersectionPoint.u;
		points[i].v = points[i].v - intersectionPoint.v;
	}
	return points;
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

      	//ACEPTACION TRIVIAL
	    if((p1.v > 0 && p2.v < 0) || (p1.v < 0 && p2.v > 0)){
		    if(p1.u > 0 && p2.u > 0){
		      	numEdges++;
		    }
		    else if(p1.u > 0 || p2.u > 0){
		      	double intersection = p1.u - p1.v*(p2.u - p1.u) / (p2.v - p1.v);
		      	if(intersection > 0){
		      		numEdges++;
		      	}
		    }
	    }    
    }

      return (numEdges % 2);
}

bool verifyPoint(POINT2D *points2D, int sizePoints, POINT2D intersectionPoint){
	points2D = translatePoints(points2D, sizePoints, intersectionPoint);

	if(countEdges(points2D, sizePoints) != 0){ // Si la cantidad es impar
		return true;
	}
	return false;
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
	}
	else{
		intersection.tmin = 0;
		intersection.tmax = 0;
		intersection.flag = 0;
	}
	return intersection;
}


