#include "objects.h"
#include "operations.h"

long double getFatt(LIGHT light){
	long double temp = 1/(light.c1, light.c2, light.c3);
	long double fatt = min(1.0, temp);
}

long double getIntensity(double pointProd, OBJECT object, long double fatt, long double lightIntensity){
	long double intensity = pointProd * object.kd * fatt * lightIntensity;

	return intensity;
}