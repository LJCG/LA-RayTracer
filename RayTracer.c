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
#include "polygon.h"
#include "cylinder.h"
#include "cone.h"
#include "data.h"

#define SWAP(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))

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

// ---------------------------------- GENERAR ARCHIVO AVS --------------------------------------
void save(COLOR **frameBuffer){
   int i, j, a, width, height;
   double r, g, b;
   FILE *fptr;

   // Abre el archivo
   if((fptr = fopen("result.avs","w")) != NULL) {

   		width = H_SIZE;
		height = V_SIZE;

	    width = FIX(width);
		fwrite(&width, sizeof(unsigned int), 1, fptr);

		height = FIX(height);
		fwrite(&height, sizeof(unsigned int), 1, fptr);


		for(j = V_SIZE -1; j >= 0; j--) {
     		for(i = 0; i < H_SIZE; i++) {
     			COLOR cl = frameBuffer[i][j];
				fputc(255, fptr);       //alpha
				fputc(cl.r*255, fptr);	//R
				fputc(cl.g*255, fptr);	//G
				fputc(cl.b*255, fptr);	//B

   			}
   		}


   		fclose(fptr);

   	}
   	else{
   		printf("Saving failed!");
   	}
}

// ---------------------------------- GENERAR ESCENA --------------------------------------
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

// ---------------------------------- OBTENER INTERSECCIONES --------------------------------------
OBJECT obj;
int intersectionFlag = 0;
POINT firstIntersection(VECTOR vectorW, VECTOR vectorD, POINT point, int pFlag){
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
			intersection = findIntersection_sphere(vectorD, point, sphere.center, sphere.radius);
		}
		else if(object.id == 'C'){
			//calcular interseccion cilindro
			CYLINDER cylinder = object.cylinder;
			intersection = findIntersection_cylinder(vectorD, point, cylinder.anchor, cylinder.radius, cylinder.axis, cylinder.d1, cylinder.d2);
		}
		else if(object.id == 'P'){
			//calcular interseccion poligono
			POLYGON polygon = object.polygon;
			if(pFlag == 1){
				polygon.equation = reverse(polygon);
			}
			intersection = findIntersection_polygon(vectorD, point, polygon);
		}
		else if(object.id == 'N'){
			//calcular interseccion cono
      CONE cone = object.cone;
			intersection = findIntersection_cone(vectorD, point, cone.anchor, cone.radius, cone.axis, cone.d1, cone.d2,cone.k1,cone.k2);
		}
		if(intersection.flag == 1 && intersection.tmin < tmin && intersection.tmin > EPSILON){

			tmin = intersection.tmin;
			obj = object;
			intersectionPoint = getIntersectionPoint(vectorW, vectorD, tmin);
			intersectionFlag = 1;
		}
	}
	return intersectionPoint;
}

// ---------------------------------- OBTENER COLOR --------------------------------------
COLOR getColor(VECTOR vectorW, VECTOR vectorD, POINT pEye){
	COLOR color;
	POINT intersection; //Interseccion del ojo con un objeto
	POINT obstacle;     //Obstaculo entre la luz y un objeto
	int k;
	long double fatt;   //Factor de atenuacion

	intersection = firstIntersection(vectorW, vectorD, pEye, 0);
	if(intersectionFlag == 0){ //No hay interseccion entonces se devuelve el color de fondo
		color = background;
	}

	else{
		
		color = obj.color;
		long double I = 0.0; // INTENSIDAD
		long double E = 0.0; // REFLEXION ESPECULAR
		VECTOR V = numberByVector(vectorD, -1); // Vector que va desde P hasta el ojo
		
		VECTOR R; // Vector de rebote de la luz
		bool Rflag = false;
		for(k=0; k < numLights; k++){ //Se recorren todas las luces

			VECTOR L = getL(intersection, lights[k]); //Vector entre la luz y el objeto
			VECTOR N = getN(obj, intersection);       //Vector normal
			

			if(obj.id == 'P' && pointProduct(N, vectorD) > 0){
				N = numberByVector(N, -1.0);
			}

			double pointProd = pointProduct(L, N);    //Producto punto de L y N

			if(pointProd > EPSILON){  //El coseno del angulo es mayor a EPSILON

				obstacle = firstIntersection(pointToVector(intersection),L, intersection, 1);

				if(intersectionFlag == 0 || getDistance(intersection, obstacle) > getDistance(intersection, lights[k].location)){
					//No hay obstaculos. Se toma en cuenta el aporte de la luz.

					fatt = getFatt(lights[k], L);
					I += getIntensity(pointProd, obj, fatt, lights[k].intensity);
					R = numberByVector(N, 2);
					R = numberByVector(R, pointProd);
					R = substractVectors(R, L);
					Rflag = true;

					double pointProdVR = pointProduct(V, R);
					if(pointProdVR > EPSILON){
						double factor = pow(pointProdVR, obj.kn);
						if(factor < EPSILON && obj.id == 'P'){
							factor = 0.02;
						}
						E += factor * fatt * obj.ks * lights[k].intensity;
					}
				}
			}
		}

		I += Ia*obj.ka;
		I = min(1.0, I);


		color.r = color.r*I;
		color.g = color.g*I;
		color.b = color.b*I;

		//ESPECULAR
		color.r = color.r + E * (1.0 - color.r);
		color.g = color.g + E * (1.0 - color.g);
		color.b = color.b + E * (1.0 - color.b);

		if((obj.o1 + obj.o2) == 1.0 && Rflag){
			OBJECT reflexObject = obj;
			COLOR reflexColor = getColor(pointToVector(intersection), R, intersection);
			
			if(sameColor(reflexColor, background) == 0){
				color.r = color.r*reflexObject.o1 + reflexColor.r*reflexObject.o2;
				color.g = color.g*reflexObject.o1 + reflexColor.g*reflexObject.o2;
				color.b = color.b*reflexObject.o1 + reflexColor.b*reflexObject.o2;
			}
		}

	}
	intersectionFlag = 0;
	return color;
}

// ---------------------------------- GENERAR DIBUJO --------------------------------------

COLOR antialiasing(float i, float j, float value){
	COLOR color, color1, color2, color3, color4;
	POINT w; // (xw, yw, zw)
	VECTOR d; // (xd, yd, zd)

	w = mapXY(i, j, xmax, ymax, xmin, ymin, 0.0, 0.0);
	d.x = w.x - eye.x;
	d.y = w.y - eye.y;
	d.z = w.z - eye.z;
	d = normalizeVector(d);
	color1 = getColor(pointToVector(eye), d, eye);

	w = mapXY(i, j, xmax, ymax, xmin, ymin, value, 0.0);
	d.x = w.x - eye.x;
	d.y = w.y - eye.y;
	d.z = w.z - eye.z;
	d = normalizeVector(d);
	color2 = getColor(pointToVector(eye), d, eye);

	w = mapXY(i, j, xmax, ymax, xmin, ymin, 0.0, value);
	d.x = w.x - eye.x;
	d.y = w.y - eye.y;
	d.z = w.z - eye.z;
	d = normalizeVector(d);
	color3 = getColor(pointToVector(eye), d, eye);

    w = mapXY(i, j, xmax, ymax, xmin, ymin, value, value);
	d.x = w.x - eye.x;
	d.y = w.y - eye.y;
	d.z = w.z - eye.z;
	d = normalizeVector(d);
	color4 = getColor(pointToVector(eye), d, eye);


	return avgColor(color1, color2, color3, color4); 
}

void tracer(){
	int i, j;
	POINT w; // (xw, yw, zw)
	VECTOR d; // (xd, yd, zd)
	COLOR color0, color1, color2, color3, color4;


	for(i = 0; i < H_SIZE; i++){
		for(j = 0; j < V_SIZE; j++){

			color1 = antialiasing(i, j, 0.5);
			color2 = antialiasing(i+0.5, j, 0.5);
			color3 = antialiasing(i, j+0.5, 0.5);
			color4 = antialiasing(i+0.5, j+0.5, 0.5);

			frameBuffer[i][j] = avgColor(color1, color2, color3, color4);

		}
	}

	save(frameBuffer);
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

   setBackground(0.0, 0.0, 0.0);
   setEye(750.0, 800.0, -1500.0);
   setWindow(0, 0, 1500, 1000);


      //globalConfig();
      //loadInfo();
   //createSphere(double radius, POINT center, COLOR color, long double kd, long double ka,
  //			long double kn, long double ks, long double o1, long double o2);

    POINT c;
    c.x = 750.0;
    c.y = 700.0;
    c.z = 300.0;

    COLOR cl;
    cl.r = 0.6;
    cl.g = 0.0;
    cl.b = 0.1;

    addObject(createSphere(150, c, cl, 0.7, 0.6, 5, 0.5, 0.5, 0.5));


  
    c.x = 600.0;
    c.y = 750.0;
    c.z = 100.0;

    cl.r = 0.4;
    cl.g = 0.2;
    cl.b = 1.0;

    addObject(createSphere(80, c, cl, 0.7, 0.6, 5, 0.5, 0.0, 0.0));



// ----------------------------------------- LUCES ------------------------------------------------------
    c.x = 900.0;
    c.y = 700.0;
    c.z = -800.0;
    addLight(createLight(c, 0.4, 0.0, 0.0, 0.4));

    c.x = 1600.0;
    c.y = 1000.0;
    c.z = -1200.0;
    //addLight(createLight(c, 0.6, 0.0, 0.0, 0.3));

    Ia = 0.65;

   tracer();

   glutMainLoop();

}
