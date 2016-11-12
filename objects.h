#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define V_SIZE 700
#define H_SIZE 1008
#define PI 3.141592
#define EPSILON 0.00005


typedef struct {
  double r;
  double g;
  double b;
} COLOR;

typedef struct{
  int x0;
  int y0;
  int x1;
  int y1;
} LINEA;

typedef struct{
	double u;
	double v;
} POINT2D;

typedef struct{
	double x;
	double y;
	double z;
} POINT;

typedef struct{
	double x;
	double y;
	double z;
} VECTOR;

typedef struct{
	double radius;
	POINT center;
} SPHERE;

typedef struct{
	double radius;
  POINT anchor;
  VECTOR axis;
  double d1;
  double d2;
  double k1;
  double k2;
} CONE;

typedef struct{
	double radius;
	POINT anchor; // ancla del cilindro
	VECTOR axis; // Q, eje del cilindro
	double d1;	// Tapa 1
	double d2;	// Tapa 2
} CYLINDER;

typedef struct{
	double a;
	double b;
	double c;
	double d;
} PEQUATION;

typedef struct{
	POINT* points;
	int sizePoints;
	POINT2D* points2D;
	PEQUATION equation;
	char tag;
} POLYGON;

typedef struct{
	double radius;
	POINT center;
	PEQUATION equation;
} DISK;

typedef struct{
	char id;
	long double kd; // coef reflexión difusa
	long double ka; // coef amb
	long double ks; // coef reflexion especular
	long double kn; // factor especular
	long double o1; // grado color del objeto
	long double o2; // grado color del reflejo
	COLOR color;
	SPHERE sphere;
	CONE cone;
	CYLINDER cylinder;
	POLYGON polygon;
	DISK disk;
} OBJECT;

typedef struct{
	POINT location;
	long double intensity;
	COLOR color;
	long double c1;
	long double c2;
	long double c3;
} LIGHT;

typedef struct{
	double t;
	POINT eye;
	POINT direction;
} RAY;

typedef struct{
	double tmin;
	double tmax;
	int flag;
} INTERSECTION;
