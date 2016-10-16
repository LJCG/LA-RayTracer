// Autores: Lucy Chaves, Andrés Salazar
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "objects.h"
#include "operations.h"
#include "sphere.h"
#include "lights.h"

COLOR **frameBuffer;
POINT eye;
COLOR background;
long double Ia; // INTENSIDAD DEL AMBIENTE

OBJECT objects[3000];
int sizeObjects = 0;

LIGHT lights[3000];
int numLights = 0;

int xmax, ymax, xmin, ymin;

void init_buffer(){
   int i, j;
   frameBuffer = (COLOR **)malloc(H_SIZE * sizeof(COLOR*));
   for (i = 0; i < H_SIZE; i++){
        frameBuffer[i] = (COLOR *)malloc(V_SIZE * sizeof(COLOR));
       }

   for (i = 0; i < H_SIZE; i++) {
        for (j = 0; j < V_SIZE; j++)
            {
             frameBuffer[i][j].r = 0.0;
             frameBuffer[i][j].g = 0.0;
             frameBuffer[i][j].b = 0.0;
            }
       }
}

void draw_scene(){	
   static int last_x = 0;
   int i, j;
   COLOR color;

   for (i = 0; i < last_x; i++){
        for (j = 0; j < V_SIZE; j++)
            {
             glColor3f (0.0,0.0,0.0);
             glBegin (GL_POINTS);
             glVertex2i (i,j);
             glEnd();
            }
   }
   for (i = last_x; i < H_SIZE; i++){
   	for (j = 0; j < V_SIZE; j++){
          glColor3f (frameBuffer[i][j].r,frameBuffer[i][j].g,frameBuffer[i][j].b);
          glBegin(GL_POINTS);
          glVertex2i(i,j);
          glEnd();
          last_x = i;
         }
      }
      
   glFlush();
}

// Borra todo lo dibujado en la ventana
void clear(){
	int x,y;
	for(x = 0; x < H_SIZE; x++){
		for(y = 0; y < V_SIZE; y++){
			frameBuffer[x][y].r = 0.0;
			frameBuffer[x][y].g = 0.0;
			frameBuffer[x][y].b = 0.0;
		
			glColor3f (frameBuffer[x][y].r,frameBuffer[x][y].g,frameBuffer[x][y].b);
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();
		}
	}
	
	   glFlush();
}

void setBackground(double r, double g, double b){
   background.r = r;
   background.g = g;
   background.b = b;
}

void setEye(double x, double y, double z){
   eye.x = x;
   eye.y = y;
   eye.z = z;
}

void setWindow(int pxmin, int pymin, int pxmax, int pymax){
	xmin = pxmin;
	ymin = pymin;
	xmax = pxmax;
	ymax = pymax;
}

void addObject(OBJECT newObject){
	objects[sizeObjects] = newObject;
	sizeObjects++;
}

void addLight(LIGHT newLight){
	lights[numLights] = newLight;
	numLights++;
}

POINT getIntersectionPoint(VECTOR vectorW, VECTOR vectorD, double t){
	POINT point;

	point.x = vectorW.x + t*vectorD.x;
	point.y = vectorW.y + t*vectorD.y;
	point.z = vectorW.z + t*vectorD.z;
	return point;
}

OBJECT obj;
int intersectionFlag = 0;
POINT firstIntersection(VECTOR vectorW, VECTOR vectorD){
	INTERSECTION intersection;
	OBJECT object;
	POINT intersectionPoint;
	double tmin = 9000000;
	int i;

	intersectionFlag = 0;
	for(i = 0; i < sizeObjects; i++){
		object = objects[i];
		//calcular interseccion

		if(object.id == 'S'){
			//calcular interseccion esfera
			SPHERE sphere = object.sphere;
			intersection = findIntersection_sphere(vectorD, eye, sphere.center, sphere.radius);
		}
		else if(object.id == 'C'){
			//calcular interseccion cilindro
		}
		else if(object.id == 'P'){
			//calcular interseccion poligono
		}
		else if(object.id == 'N'){
			//calcular interseccion cono
		}
		if(intersection.flag == 1 && intersection.tmin < tmin){
			tmin = intersection.tmin;
			obj = object;
			intersectionPoint = getIntersectionPoint(vectorW, vectorD, tmin);
			intersectionFlag = 1;
		}
	}
	return intersectionPoint;
}

COLOR getColor(VECTOR vectorW, VECTOR vectorD){
	COLOR color;
	POINT intersection; //Interseccion del ojo con un objeto
	POINT obstacle;     //Obstaculo entre la luz y un objeto
	int k;
	long double fatt;   //Factor de atenuacion
	
	intersection = firstIntersection(vectorW, vectorD);
	if(intersectionFlag == 0){ //No hay interseccion entonces se devuelde el color de fondo
		color = background;
	}

	else{
		
		long double I = 0.0; // INTENSIDAD
		for(k=0; k<numLights; k++){ //Se recorren todas las luces

			VECTOR L = getL(intersection, lights[k]); //Vector entre la luz y el objeto
			VECTOR N = getN(obj, intersection);       //Vector normal
			double pointProd = pointProduct(L, N);    //Producto punto de L y N

			if(pointProd > 0.00005){  //El coseno del angulo es mayor a EPSILON

				obstacle = firstIntersection(pointToVector(intersection), L);

				if(intersectionFlag == 0 ){ 
					//No hay obstaculos. Se toma en cuenta el aporte de la luz.
					fatt = getFatt(lights[k], L);
					I += getIntensity(pointProd, obj, fatt, lights[k].intensity);
				}

				else{ 
					VECTOR vObstacle;        //Vector entre objeto y obstaculo.
					vObstacle.x = obstacle.x - intersection.x;
					vObstacle.y = obstacle.y - intersection.y;
					vObstacle.z = obstacle.z - intersection.z;

					double mL = getMagnitude(L);             //Magnitud de L
					double mObs = getMagnitude(vObstacle);   //Magnitud de objectObstacle

					if(mObs > mL){
						fatt = getFatt(lights[k], L);
						I += getIntensity(pointProd, obj, fatt, lights[k].intensity);
					}
				}
			}
		}
		I += Ia*obj.ka;
		I = min(1.0, I);

		color.r = obj.color.r*I; 
		color.g = obj.color.g*I; 
		color.b = obj.color.b*I; 
	}
	intersectionFlag = 0;
	return color;
}



void tracer(){
	int i, j;
	POINT w; // (xw, yw, zw)
	VECTOR d; // (xd, yd, zd)
	COLOR color;
	
	
	for(i = 0; i < H_SIZE; i++){
		for(j = 0; j < V_SIZE; j++){
			w = mapXY(i, j, xmax, ymax, xmin, ymin); 
			d.x = w.x - eye.x;
			d.y = w.y - eye.y;
			d.z = w.z - eye.z;
			d = normalizeVector(d);
			color = getColor(pointToVector(eye), d);
			frameBuffer[i][j] = color;
		}
	}
	
	//saveFile(frameBuffer);
}


int main(int argc, char** argv){
   init_buffer();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(H_SIZE,V_SIZE);
   glutCreateWindow("Proyecto 2: RayTracer");
   glClear(GL_COLOR_BUFFER_BIT);
   gluOrtho2D(-0.5, H_SIZE +0.5, -0.5, V_SIZE + 0.5);
   glutDisplayFunc(draw_scene);


   setBackground(0.8, 0.8, 0.8);
   setEye(200.0, 200.0, -2210.0);
   setWindow(-200, -200, 600, 600);

   POINT c;
   COLOR cl;

   c.x = 200.0;
   c.y = 400.0;
   c.z = 400.0;

   cl.r = 0.5;
   cl.g = 0.5;
   cl.b = 0.5;

		
   addObject(createSphere(100, c, cl, 0.8, 0.8));

   c.x = 100.0;
   c.y = 100.0;
   c.z = 650.0;

   cl.r = 0.2;
   cl.g = 0.6;
   cl.b = 0.1;

		
   addObject(createSphere(250, c, cl, 0.8, 0.8));

   c.x = 150.0;
   c.y = 600.0;
   c.z = 100.0;

   addLight(createLight(c, 0.8, 1.0, 0.0, 0.0));	
   Ia = 0.4;

   tracer();
   glutMainLoop();
	
}


