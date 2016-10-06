#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define V_SIZE 800
#define H_SIZE 800
#define PI 3.141592

typedef struct{
	int x;
	int y;
}COORD;


typedef struct{
	long double x;
	long double y;
}UCOORD;

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
	COLOR color;
	POINT center;
} SPHERE;

typedef struct{
	double t;
	POINT eye;
	POINT direction;
} RAY;



