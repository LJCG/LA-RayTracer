#ifndef LIGHTS_H_   
#define LIGHTS_H_

LIGHT createLight(POINT location, long double intensity, long double c1, long double c2, long double c3);
long double getFatt(LIGHT light, VECTOR L);
long double getIntensity(double pointProd, OBJECT object, long double fatt, long double lightIntensity);

#endif