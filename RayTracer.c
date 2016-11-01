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
COLOR getColor(VECTOR vectorW, VECTOR vectorD){
	COLOR color;
	POINT intersection; //Interseccion del ojo con un objeto
	POINT obstacle;     //Obstaculo entre la luz y un objeto
	int k;
	long double fatt;   //Factor de atenuacion

	intersection = firstIntersection(vectorW, vectorD, eye, 0);
	if(intersectionFlag == 0){ //No hay interseccion entonces se devuelde el color de fondo
		color = background;
	}

	else{
		color = obj.color;
		long double I = 0.0; // INTENSIDAD
		long double E = 0.0; // REFLEXION ESPECULAR
		VECTOR V = numberByVector(vectorD, -1); // Vector que va desde P hasta el ojo

		for(k=0; k < numLights; k++){ //Se recorren todas las luces

			VECTOR L = getL(intersection, lights[k]); //Vector entre la luz y el objeto
			VECTOR N = getN(obj, intersection);       //Vector normal
			VECTOR R; // Vector de rebote de la luz

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

	}
	intersectionFlag = 0;
	return color;
}

// ---------------------------------- GENERAR DIBUJO --------------------------------------
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

   setBackground(0.0, 0.4, 0.8);
   setEye(750.0, 800.0, -1500.0);
   setWindow(0, 0, 1500, 1000);

      POINT c;
      COLOR cl;

   // POLIGONO ARENA  
    POINT p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18;

   // ------------------------------------------------- PEZ 1 -----------------------------------------------------
  p1.x = 1600.0;
  p1.y = 900.0;
  p1.z = 3000.0;

  p2.x = 1650.0;
  p2.y = 800.0;
  p2.z = 3000.0;

  p3.x = 1600.0;
  p3.y = 720.0;
  p3.z = 3000.0;

  p4.x = 1700.0;
  p4.y = 800.0;
  p4.z = 3000.0;

  p5.x = 1850.0;
  p5.y = 740.0;
  p5.z = 3000.0;

  p6.x = 1950.0;
  p6.y = 790.0;
  p6.z = 3000.0;

  p7.x = 1900.0;
  p7.y = 800.0;
  p7.z = 3000.0;

  p8.x = 1950.0;
  p8.y = 830.0;
  p8.z = 3000.0;

  p9.x = 1850.0;
  p9.y = 900.0;
  p9.z = 3000.0;

  p10.x = 1700.0;
  p10.y = 850.0;
  p10.z = 3000.0;

  // COLOR
   cl.r = 0.0;
   cl.g = 0.4;
   cl.b = 0.2;

   POINT points2[10];
   points2[0] = p1;
   points2[1] = p2;
   points2[2] = p3;
   points2[3] = p4;
   points2[4] = p5;
   points2[5] = p6;
   points2[6] = p7;
   points2[7] = p8;
   points2[8] = p9;
   points2[9] = p10;

   OBJECT po = createPolygon(points2, 10, cl, 0.7, 0.5, 0.8, 50.0);
   po.polygon.equation = reverse(po.polygon);

   addObject(po);

// ------------------------------------------------- PEZ 2 -----------------------------------------------------
  p1.x = 1800.0;
  p1.y = 1300.0;
  p1.z = 5000.0;

  p2.x = 1850.0;
  p2.y = 1200.0;
  p2.z = 5000.0;

  p3.x = 1800.0;
  p3.y = 1120.0;
  p3.z = 5000.0;

  p4.x = 1900.0;
  p4.y = 1200.0;
  p4.z = 5000.0;

  p5.x = 2050.0;
  p5.y = 1140.0;
  p5.z = 5000.0;

  p6.x = 2150.0;
  p6.y = 1190.0;
  p6.z = 5000.0;

  p7.x = 2100.0;
  p7.y = 1200.0;
  p7.z = 5000.0;

  p8.x = 2150.0;
  p8.y = 1230.0;
  p8.z = 5000.0;

  p9.x = 2050.0;
  p9.y = 1300.0;
  p9.z = 5000.0;

  p10.x = 1900.0;
  p10.y = 1250.0;
  p10.z = 5000.0;

  // COLOR
   cl.r = 0.0;
   cl.g = 0.4;
   cl.b = 0.2;

   points2[10];
   points2[0] = p1;
   points2[1] = p2;
   points2[2] = p3;
   points2[3] = p4;
   points2[4] = p5;
   points2[5] = p6;
   points2[6] = p7;
   points2[7] = p8;
   points2[8] = p9;
   points2[9] = p10;

   po = createPolygon(points2, 10, cl, 0.7, 0.5, 0.8, 50.0);
   po.polygon.equation = reverse(po.polygon);

   addObject(po);

// ------------------------------------------------- PEZ 3 -----------------------------------------------------
  p1.x = 2200.0;
  p1.y = 1400.0;
  p1.z = 5000.0;

  p2.x = 2250.0;
  p2.y = 1300.0;
  p2.z = 5000.0;

  p3.x = 2200.0;
  p3.y = 1220.0;
  p3.z = 5000.0;

  p4.x = 2300.0;
  p4.y = 1300.0;
  p4.z = 5000.0;

  p5.x = 2450.0;
  p5.y = 1240.0;
  p5.z = 5000.0;

  p6.x = 2550.0;
  p6.y = 1290.0;
  p6.z = 5000.0;

  p7.x = 2500.0;
  p7.y = 1300.0;
  p7.z = 5000.0;

  p8.x = 2550.0;
  p8.y = 1330.0;
  p8.z = 5000.0;

  p9.x = 2450.0;
  p9.y = 1400.0;
  p9.z = 5000.0;

  p10.x = 2300.0;
  p10.y = 1350.0;
  p10.z = 5000.0;

  // COLOR
   cl.r = 0.0;
   cl.g = 0.4;
   cl.b = 0.2;

   points2[10];
   points2[0] = p1;
   points2[1] = p2;
   points2[2] = p3;
   points2[3] = p4;
   points2[4] = p5;
   points2[5] = p6;
   points2[6] = p7;
   points2[7] = p8;
   points2[8] = p9;
   points2[9] = p10;

   po = createPolygon(points2, 10, cl, 0.7, 0.5, 0.8, 50.0);
   po.polygon.equation = reverse(po.polygon);

   addObject(po);
// ------------------------------------------------- CANGREJO 1 -----------------------------------------------------

// COLOR
   cl.r = 1.0;
   cl.g = 0.2706;
   cl.b = 0.0;

  p1.x = -740.0;
  p1.y = 510.0;
  p1.z = 8500.0;

  p2.x = -740.0;
  p2.y = 440.0;
  p2.z = 8500.0;

  p3.x = -850.0;
  p3.y = 390.0;
  p3.z = 8500.0;

  p4.x = -740.0;
  p4.y = 240.0;
  p4.z = 8500.0;

  p5.x = -710.0;
  p5.y = 370.0;
  p5.z = 8500.0;

  p6.x = -620.0;
  p6.y = 290.0;
  p6.z = 8500.0;

  p7.x = -420.0;
  p7.y = 290.0;
  p7.z = 8500.0;

  p8.x = -350.0;
  p8.y = 370.0;
  p8.z = 8500.0;

  p9.x = -300.0;
  p9.y = 240.0;
  p9.z = 8500.0;

  p10.x = -200.0;
  p10.y = 390.0;
  p10.z = 8500.0;

  p11.x = -300.0;
  p11.y = 440.0;
  p11.z = 8500.0;

  p12.x = -200.0;
  p12.y = 510.0;
  p12.z = 8500.0;

  p13.x = -350.0;
  p13.y = 690.0;
  p13.z = 8500.0;

  p14.x = -320.0;
  p14.y = 510.0;
  p14.z = 8500.0;

  p15.x = -710.0;
  p15.y = 510.0;
  p15.z = 8500.0;

  p16.x = -680.0;
  p16.y = 690.0;
  p16.z = 8500.0;

  p17.x = -850.0;
  p17.y = 510.0;
  p17.z = 8500.0;

  p18.x = -740.0;
  p18.y = 440.0;
  p18.z = 8500.0;

   points2[18];
   points2[0] = p1;
   points2[1] = p2;
   points2[2] = p3;
   points2[3] = p4;
   points2[4] = p5;
   points2[5] = p6;
   points2[6] = p7;
   points2[7] = p8;
   points2[8] = p9;
   points2[9] = p10;
   points2[10] = p11;
   points2[11] = p12;
   points2[12] = p13;
   points2[13] = p14;
   points2[14] = p15;
   points2[15] = p16;
   points2[16] = p17;
   points2[17] = p18;

   po = createPolygon(points2, 18, cl, 0.7, 0.5, 0.8, 50.0);
   po.polygon.equation = reverse(po.polygon);

   addObject(po);

POINT anchor; // ancla
   anchor.x = -200;
   anchor.y = 850;
   anchor.z = 800;

   VECTOR axis; 
   axis.x = 1000.0;
   axis.y = 0.0;
   axis.z = -200.0;

   cl.r = 0.9019; 
   cl.g = 0.8745;
   cl.b = 0.0;

   addObject(createCylinder(50, anchor, axis, 20.0, 300.0, cl, 0.7, 0.6, 8, 0.3));

   c.x = 85;
   c.y = 850;
   c.z = 770;

   cl.r = 1.0; 
   cl.g = 0.8;
   cl.b = 0.0;

   addObject(createSphere(50, c, cl, 0.7, 0.6, 8, 0.3));


   c.x = 30;
   c.y = 850;
   c.z = 726;

   cl.r = 0.9647; 
   cl.g = 0.1058;
   cl.b = 0.1058;

   addObject(createSphere(30, c, cl, 0.7, 0.6, 8, 0.3));

   c.x = -50;
   c.y = 850;
   c.z = 745;

   addObject(createSphere(30, c, cl, 0.7, 0.6, 8, 0.3));

   c.x = -123;
   c.y = 850;
   c.z = 761;

   addObject(createSphere(30, c, cl, 0.7, 0.6, 8, 0.3));

//ventanas
   c.x = 24;
   c.y = 850;
   c.z = 714;

   cl.r = 0.4901; 
   cl.g = 0.8666;
   cl.b = 1.0;

   addObject(createSphere(20, c, cl, 0.7, 0.6, 8, 0.8));

   c.x = -55;
   c.y = 850;
   c.z = 734;

   addObject(createSphere(20, c, cl, 0.7, 0.6, 8, 0.8));

   c.x = -126;
   c.y = 850;
   c.z = 750.5;

   addObject(createSphere(20, c, cl, 0.7, 0.6, 8, 0.8));

//parte arriba

   anchor.x = 20;
   anchor.y = 860;
   anchor.z = 735;

   axis.x = 0.0;
   axis.y = 10000.0;
   axis.z = -200.0;

   cl.r = 0.9019; 
   cl.g = 0.8745;
   cl.b = 0.0;

   addObject(createCylinder(15, anchor, axis, 20.0, 80.0, cl, 0.7, 0.6, 8, 0.3));


   c.x = 20;
   c.y = 947;
   c.z = 735;




   addObject(createSphere(15, c, cl, 0.7, 0.6, 8, 0.8));
   cl.r = 0.9019; 
   cl.g = 0.8745;
   cl.b = 0.0;

   anchor.x = 20;
   anchor.y = 947;
   anchor.z = 712;

   axis.x = 10000.0;
   axis.y = 0.0;
   axis.z = -200.0;

   addObject(createCylinder(15, anchor, axis, 20.0, 60.0, cl, 0.7, 0.6, 8, 0.3));


   c.x = 72;
   c.y = 947;
   c.z = 715;

   cl.r = 0.4901; 
   cl.g = 0.8666;
   cl.b = 1.0;

   addObject(createSphere(15, c, cl, 0.7, 0.6, 8, 0.8));

   //parte atras

   anchor.x = -160;
   anchor.y = 850;
   anchor.z = 800;

   axis.x = -10000.0;
   axis.y = 0.0;
   axis.z = 900.0;

  cl.r = 1.0; 
   cl.g = 0.8;
   cl.b = 0.0;


   addObject(createCone(20, anchor, axis, 0, 50, cl, 10.4, 12.3, 0.7, 0.6, 8, 0.3));
   loadInfo();
// ----------------------------------------- PLANTITA 1 ------------------------------------------------------
   cl.r = 0.3333; 
   cl.g = 0.4196;
   cl.b = 0.1843;

   anchor.x = 1000;
   anchor.y = 400;
   anchor.z = -200;

   axis.x = 0.0;
   axis.y = -10000.0;
   axis.z = -200.0;

   addObject(createCylinder(5, anchor, rotate_cone(axis,0.0), 0.0, 140.0, cl, 0.7, 0.6, 8, 0.3)); 

   anchor.x = 1000;
   anchor.y = 350;
   anchor.z = -200;

   axis.x = 0.0;
   axis.y = 10000.0;
   axis.z = -200.0;

   addObject(createCylinder(5, anchor, rotate_cone(axis,45), 0.0, 50.0, cl, 0.7, 0.6, 8, 0.3)); 

   anchor.x = 1000;
   anchor.y = 330;
   anchor.z = -200;

   axis.x = 0.0;
   axis.y = 10000.0;
   axis.z = -200.0;

   addObject(createCylinder(5, anchor, rotate_cone(axis,-45), 0.0, 70.0, cl, 0.7, 0.6, 8, 0.3)); 

// ----------------------------------------- PLANTITA 2 ------------------------------------------------------
   cl.r = 0.0; 
   cl.g = 0.5020;
   cl.b = 1.0;

   anchor.x = 1000;
   anchor.y = 400;
   anchor.z = -100;

   axis.x = 0.0;
   axis.y = -10000.0;
   axis.z = -200.0;

   addObject(createCylinder(5, anchor, rotate_cone(axis,0.0), 0.0, 140.0, cl, 0.7, 0.6, 8, 0.3)); 

   anchor.x = 1000;
   anchor.y = 350;
   anchor.z = -100;

   axis.x = 0.0;
   axis.y = 10000.0;
   axis.z = -200.0;

   addObject(createCylinder(5, anchor, rotate_cone(axis,45), 0.0, 50.0, cl, 0.7, 0.6, 8, 0.3)); 

   anchor.x = 1000;
   anchor.y = 330;
   anchor.z = -100;

   axis.x = 0.0;
   axis.y = 10000.0;
   axis.z = -200.0;

   addObject(createCylinder(5, anchor, rotate_cone(axis,-45), 0.0, 70.0, cl, 0.7, 0.6, 8, 0.3)); 

// ----------------------------------------- PLANTITA 3 ------------------------------------------------------
   cl.r = 0.0; 
   cl.g = 0.5020;
   cl.b = 1.0;

   anchor.x = 1010;
   anchor.y = 400;
   anchor.z = -250;

   axis.x = 0.0;
   axis.y = -10000.0;
   axis.z = -200.0;

   addObject(createCylinder(5, anchor, rotate_cone(axis,0.0), 0.0, 140.0, cl, 0.7, 0.6, 8, 0.3)); 

   anchor.x = 1010;
   anchor.y = 350;
   anchor.z = -250;

   axis.x = 0.0;
   axis.y = 10000.0;
   axis.z = -200.0;

   addObject(createCylinder(5, anchor, rotate_cone(axis,45), 0.0, 50.0, cl, 0.7, 0.6, 8, 0.3)); 

   anchor.x = 1010;
   anchor.y = 330;
   anchor.z = -250;

   axis.x = 0.0;
   axis.y = 10000.0;
   axis.z = -200.0;

   addObject(createCylinder(5, anchor, rotate_cone(axis,-45), 0.0, 70.0, cl, 0.7, 0.6, 8, 0.3)); 
// ----------------------------------------- LUCES ------------------------------------------------------
   c.x = 0.0;
   c.y = 700.0;
   c.z = 0.0;
   addLight(createLight(c, 0.4, 0.3, 0.0, 0.0));   

   c.x = 1100.0;
   c.y = 800.0;
   c.z = -1000.0;
   addLight(createLight(c, 0.3, 1.0, 0.0, 0.0));
   Ia = 0.6;

   tracer();

   glutMainLoop();

}
