#include "objects.h"
#include "elipse.h"
#include "operations.h"
#include "polygon.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

OBJECT createElipse(double k, POINT F1, POINT F2, COLOR color, long double kd, long double ka, long double ks,
					long double kn, long double o1, long double o2, long double o3){
	ELIPSE elipse;
	elipse.F1 = F1;
	elipse.F2 = F2;
	elipse.k = k;

	//POINT center = getMidPoint(F1, F2);
	POINT center;
	center.x = 300;
	center.y = 300;
	center.z = 300;

	POINT points[3];
	points[0] = center;
	points[1] = F1;
	points[2] = F2;

	elipse.equation = getABCD(points);

	elipse.equation.a = elipse.equation.a * -1;
	elipse.equation.b = elipse.equation.b * -1;
	elipse.equation.c = elipse.equation.c * -1;
	elipse.equation.d = elipse.equation.d * -1;

	OBJECT newObject;
	newObject.id = 'E';
	newObject.elipse = elipse;
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


bool verifyPointE(POINT interPoint, double k, POINT F1, POINT F2){
	double d1 = getDistance(F1, interPoint);
	double d2 = getDistance(F2, interPoint);
	double suma = d1 + d2;
	//printf("%lf\n", suma);
	if((d1+d2) <= k){
		//printf("soy un punto que esta dentro de la elipse\n");
		return true;
	}
	return false;
}

INTERSECTION findIntersection_elipse(VECTOR direction, POINT eye, ELIPSE elipse){
	VECTOR norm = eq2vector(elipse.equation); // Construye la normal a partir de la ecuación de la elipse
	INTERSECTION intersection;

	// Primera fase: Revisa si hay intersección con el plano
	if(pointProduct(norm, direction) > EPSILON){ // Hay interseccion	

		// Calcula t
		double t = -(norm.x*eye.x + norm.y*eye.y + norm.z*eye.z + elipse.equation.d)/(norm.x*direction.x + norm.y*direction.y + norm.z*direction.z);		
		POINT intersectionPoint = getIntersectionPoint(pointToVector(eye), direction, t);
		//printf("%lf\n", t);
		// Segunda fase: Revisa si el punto está en el interior de la elipse
		if(verifyPointE(intersectionPoint, elipse.k, elipse.F1, elipse.F2)){
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
