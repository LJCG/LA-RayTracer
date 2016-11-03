#include "objects.h"
#include "operations.h"
#include "math.h"


LIGHT createLight(POINT location, long double intensity, long double c1, long double c2, long double c3){
	LIGHT newLight;

	newLight.location = location;
	newLight.intensity = intensity;
	newLight.c1 = c1;
	newLight.c2 = c2;
	newLight.c3 = c3;

	return newLight;
}

long double getFatt(LIGHT light, VECTOR L){

	double dL = getMagnitude(L);
	long double temp = 1/(light.c1 + light.c2*dL + light.c3*pow(dL,2));
	long double fatt = min(1.0, temp);
	return fatt;
}

long double getIntensity(double pointProd, OBJECT object, long double fatt, long double lightIntensity){
	long double intensity = pointProd * object.kd * fatt * lightIntensity;

	return intensity;
}