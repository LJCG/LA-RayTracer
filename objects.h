#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define V_SIZE 800
#define H_SIZE 800
#define PI 3.141592


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
	double x;
	double y;
	double z;
} POINT;

typedef struct{
	double radius;
	POINT center;
} SPHERE;

typedef struct{
	double radius;
} CONE;

typedef struct{
	double radius;
} CYLINDER;

typedef struct{
	POINT* points;
	int sizePoints;
} POLYGON;

typedef struct{
	char id;
	COLOR color;
	SPHERE sphere;
	CONO cone;
	CYLINDER cylinder;
} OBJECT;

typedef struct{
	POINT location;
	double intensity; 
	COLOR color;
	// C1, C2, C3...
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

