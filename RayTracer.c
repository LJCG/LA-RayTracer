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
   p1.x = -3000.0;
   p1.y = 270.0;
   p1.z = -3000.0;

   p2.x = -8000.0;
   p2.y = 270.0;
   p2.z = 8000.0;

   p3.x = 8000.0;
   p3.y = 270.0;
   p3.z = 8000.0;

   p4.x = 8000.0;
   p4.y = 270.0;
   p4.z = -3000.0;


   cl.r = 0.792157;
   cl.g = 0.701961;
   cl.b = 0.533333;

   POINT points[4];
   points[0] = p1;
   points[1] = p2;
   points[2] = p3;
   points[3] = p4;

   OBJECT p = createPolygon(points, 4, cl, 0.7, 0.5, 0.8, 50.0);
   p.polygon.equation = reverse(p.polygon);

   addObject(p);
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
   cl.g = 1.0;
   cl.b = 0.0;

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
   cl.g = 1.0;
   cl.b = 0.0;

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
   cl.g = 1.0;
   cl.b = 0.0;

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
  p1.y = 520.0;
  p1.z = 8500.0;

  p2.x = -740.0;
  p2.y = 450.0;
  p2.z = 8500.0;

  p3.x = -850.0;
  p3.y = 400.0;
  p3.z = 8500.0;

  p4.x = -740.0;
  p4.y = 250.0;
  p4.z = 8500.0;

  p5.x = -710.0;
  p5.y = 380.0;
  p5.z = 8500.0;

  p6.x = -620.0;
  p6.y = 300.0;
  p6.z = 8500.0;

  p7.x = -420.0;
  p7.y = 300.0;
  p7.z = 8500.0;

  p8.x = -350.0;
  p8.y = 380.0;
  p8.z = 8500.0;

  p9.x = -300.0;
  p9.y = 250.0;
  p9.z = 8500.0;

  p10.x = -200.0;
  p10.y = 400.0;
  p10.z = 8500.0;

  p11.x = -300.0;
  p11.y = 450.0;
  p11.z = 8500.0;

  p12.x = -200.0;
  p12.y = 520.0;
  p12.z = 8500.0;

  p13.x = -350.0;
  p13.y = 700.0;
  p13.z = 8500.0;

  p14.x = -320.0;
  p14.y = 520.0;
  p14.z = 8500.0;

  p15.x = -710.0;
  p15.y = 520.0;
  p15.z = 8500.0;

  p16.x = -680.0;
  p16.y = 700.0;
  p16.z = 8500.0;

  p17.x = -850.0;
  p17.y = 520.0;
  p17.z = 8500.0;

  p18.x = -740.0;
  p18.y = 450.0;
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

// ---------------------------------------------------- PULPOS ---------------------------------------------------------------
   COLOR blanco;
   blanco.r = 0.8;
   blanco.g = 0.8;
   blanco.b = 0.8;

   c.x = 740.0;
   c.y = 850.0;
   c.z = 800.0;

   cl.r = 0.65;
   cl.g = 0.3;
   cl.b = 0.3; 
   addObject(createSphere(230, c, cl, 0.7, 0.6, 7.0, 0.8));

   //IZQ
   c.x = 670.0;
   c.y = 660.0;
   c.z = 710.0;

   addObject(createSphere(90, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ1
   c.x = 668.0;
   c.y = 550.0;
   c.z = 735.0;

   addObject(createSphere(100, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ2
   c.x = 598.0;
   c.y = 475.0;
   c.z = 735.0;

   addObject(createSphere(95, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ3
   c.x = 495.0;
   c.y = 400.0;
   c.z = 735.0;

   addObject(createSphere(85, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ4
   c.x = 405.0;
   c.y = 340.0;
   c.z = 735.0;

   addObject(createSphere(75, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ5
   c.x = 315.0;
   c.y = 375.0;
   c.z = 730.0;

   addObject(createSphere(65, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ6
   c.x = 275.0;
   c.y = 435.0;
   c.z = 730.0;

   addObject(createSphere(55, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ7
   c.x = 250.0;
   c.y = 475.0;
   c.z = 730.0;

   addObject(createSphere(45, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED
   c.x = 740.0;
   c.y = 650.0;
   c.z = 720.0;

   addObject(createSphere(105, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED1
   c.x = 738.0;
   c.y = 560.0;
   c.z = 715.0;

   addObject(createSphere(110, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED2
   c.x = 738.0;
   c.y = 470.0;
   c.z = 675.0;

   addObject(createSphere(95, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED3
   c.x = 738.0;
   c.y = 390.0;
   c.z = 625.0;

   addObject(createSphere(85, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED4
   c.x = 738.0;
   c.y = 345.0;
   c.z = 545.0;

   addObject(createSphere(75, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED5
   c.x = 725.0;
   c.y = 360.0;
   c.z = 500.0;

   addObject(createSphere(65, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED6
   c.x = 715.0;
   c.y = 390.0;
   c.z = 465.0;

   addObject(createSphere(55, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED7
   c.x = 707.0;
   c.y = 423.0;
   c.z = 438.0;

   addObject(createSphere(45, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER
   c.x = 810.0;
   c.y = 660.0;
   c.z = 710.0;

   addObject(createSphere(90, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER1
   c.x = 812.0;
   c.y = 550.0;
   c.z = 735.0;

   addObject(createSphere(100, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER2
   c.x = 882.0;
   c.y = 475.0;
   c.z = 735.0;

   addObject(createSphere(95, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER3
   c.x = 975.0;
   c.y = 395.0;
   c.z = 730.0;

   addObject(createSphere(85, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER4
   c.x = 1055.0;
   c.y = 348.0;
   c.z = 705.0;

   addObject(createSphere(75, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER5
   c.x = 1100.0;
   c.y = 380.0;
   c.z = 640.0;

   addObject(createSphere(65, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER6
   c.x = 1140.0;
   c.y = 395.0;
   c.z = 585.0;

   addObject(createSphere(55, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER7
   c.x = 1170.0;
   c.y = 390.0;
   c.z = 535.0;

   addObject(createSphere(45, c, cl, 0.7, 0.6, 5.0, 0.8));


   //BACK
   c.x = 738.0;
   c.y = 560.0;
   c.z = 780.0; 
   addObject(createSphere(105, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK1R
   c.x = 660.0;
   c.y = 485.0;
   c.z = 880.0; 
   addObject(createSphere(80, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK2R
   c.x = 610.0;
   c.y = 425.0;
   c.z = 935.0; 
   addObject(createSphere(75, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK3R
   c.x = 560.0;
   c.y = 365.0;
   c.z = 1025.0; 
   addObject(createSphere(65, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK4R
   c.x = 500.0;
   c.y = 325.0;
   c.z = 1075.0; 
   addObject(createSphere(55, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK5R
   c.x = 470.0;
   c.y = 360.0;
   c.z = 1120.0; 
   addObject(createSphere(50, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK6R
   c.x = 440.0;
   c.y = 420.0;
   c.z = 1130.0; 
   addObject(createSphere(45, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK7R
   c.x = 420.0;
   c.y = 460.0;
   c.z = 1130.0; 
   addObject(createSphere(40, c, cl, 0.7, 0.6, 5.0, 0.8));


   //BACK1L
   c.x = 800.0;
   c.y = 485.0;
   c.z = 880.0; 
   addObject(createSphere(80, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK2L
   c.x = 850.0;
   c.y = 425.0;
   c.z = 935.0; 
   addObject(createSphere(75, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK3L
   c.x = 900.0;
   c.y = 365.0;
   c.z = 1025.0; 
   addObject(createSphere(65, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK4L
   c.x = 960.0;
   c.y = 325.0;
   c.z = 1075.0; 
   addObject(createSphere(55, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK5L
   c.x = 990.0;
   c.y = 360.0;
   c.z = 1120.0; 
   addObject(createSphere(50, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK6L
   c.x = 1020.0;
   c.y = 420.0;
   c.z = 1130.0; 
   addObject(createSphere(45, c, cl, 0.7, 0.6, 5.0, 0.8));

   //BACK6L
   c.x = 1060.0;
   c.y = 460.0;
   c.z = 1130.0; 
   addObject(createSphere(40, c, cl, 0.7, 0.6, 5.0, 0.8));

   //ojo izq
   c.x = 655.0;
   c.y = 655.0;
   c.z = 670.0;

   addObject(createSphere(55, c, blanco, 0.7, 0.6, 5.0, 0.8));
  
   //ojo der
   c.x = 820.0;
   c.y = 655.0;
   c.z = 670.0;

   addObject(createSphere(55, c, blanco, 0.7, 0.6, 5.0, 0.8));

   //pupila izq

   COLOR negro;
   negro.r = 0.0;
   negro.g = 0.0;
   negro.b = 0.0;

   c.x = 655.0;
   c.y = 655.0;
   c.z = 630.0;

   addObject(createSphere(22, c, negro, 0.7, 0.6, 8.0, 0.8));

   //pupila der
   c.x = 820.0;
   c.y = 655.0;
   c.z = 630.0;

   addObject(createSphere(22, c, negro, 0.7, 0.6, 8.0, 0.8));


   // -------------------------------------- PULPO HIJO ------------------------------------------------

   //GRANDE
   c.x = 1400.0;
   c.y = 540.0;
   c.z = 510.0;

   addObject(createSphere(120, c, cl, 0.7, 0.6, 7.0, 0.8));

   //MED
   c.x = 1390.0;
   c.y = 420.0;
   c.z = 470.0;

   addObject(createSphere(65, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED1
   c.x = 1389.0;
   c.y = 362.0;
   c.z = 450.0;

   addObject(createSphere(60, c, cl, 0.7, 0.6, 5.0, 0.8));

   
   //MED2
   c.x = 1383.0;
   c.y = 315.0;
   c.z = 415.0;

   addObject(createSphere(50, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED3
   c.x = 1377.0;
   c.y = 310.0;
   c.z = 370.0;

   addObject(createSphere(40, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED4
   c.x = 1370.0;
   c.y = 325.0;
   c.z = 320.0;

   addObject(createSphere(30, c, cl, 0.7, 0.6, 5.0, 0.8));

   //MED5
   c.x = 1370.0;
   c.y = 355.0;
   c.z = 300.0;

   addObject(createSphere(20, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ
   c.x = 1350.0;
   c.y = 435.0;
   c.z = 460.0;

   addObject(createSphere(50, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ1
   c.x = 1355.0;
   c.y = 370.0;
   c.z = 480.0;

   addObject(createSphere(50, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ2
   c.x = 1310.0;
   c.y = 335.0;
   c.z = 490.0;

   addObject(createSphere(45, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ3
   c.x = 1265.0;
   c.y = 312.0;
   c.z = 490.0;

   addObject(createSphere(40, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ4
   c.x = 1230.0;
   c.y = 340.0;
   c.z = 470.0;

   addObject(createSphere(30, c, cl, 0.7, 0.6, 5.0, 0.8));

   //IZQ5
   c.x = 1215.0;
   c.y = 370.0;
   c.z = 455.0;

   addObject(createSphere(20, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER
   c.x = 1430.0;
   c.y = 435.0;
   c.z = 445.0;

   addObject(createSphere(50, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER1
   c.x = 1442.0;
   c.y = 370.0;
   c.z = 470.0;

   addObject(createSphere(50, c, cl, 0.7, 0.6, 5.0, 0.8));


   //DER2
   c.x = 1487.0;
   c.y = 340.0;
   c.z = 470.0;

   addObject(createSphere(45, c, cl, 0.7, 0.6, 5.0, 0.8));


   //DER3
   c.x = 1530.0;
   c.y = 310.0;
   c.z = 450.0;

   addObject(createSphere(40, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER4
   c.x = 1550.0;
   c.y = 340.0;
   c.z = 410.0;

   addObject(createSphere(30, c, cl, 0.7, 0.6, 5.0, 0.8));

   //DER5
   c.x = 1560.0;
   c.y = 370.0;
   c.z = 380.0;

   addObject(createSphere(20, c, cl, 0.7, 0.6, 5.0, 0.8));

   //ojo izq
   c.x = 1339.0;
   c.y = 435.0;
   c.z = 437.0;

   addObject(createSphere(32, c, blanco, 0.7, 0.6, 5.0, 0.8));

   //ojo der
   c.x = 1430.0;
   c.y = 435.0;
   c.z = 419.5;

   addObject(createSphere(32, c, blanco, 0.7, 0.6, 5.0, 0.8));

   //pupila izq
   c.x = 1334.0;
   c.y = 442.0;
   c.z = 418.0;

   addObject(createSphere(22, c, negro, 0.7, 0.6, 8.0, 0.8));

   //pupila der
   c.x = 1427.0;
   c.y = 441.0;
   c.z = 400.0;

   addObject(createSphere(22, c, negro, 0.7, 0.6, 8.0, 0.8));
// ------------------------------------------- DUNAS -------------------------------------------------------------
   cl.r = 0.792157;
   cl.g = 0.701961;
   cl.b = 0.533333;

   c.x = -500.0;
   c.y = 100.0;
   c.z = 2000.0;

   addObject(createSphere(250, c, cl, 0.6, 0.6, 20.0, 0.8));

   c.x = 2400.0;
   c.y = 60.0;
   c.z = 5000.0;

   addObject(createSphere(250, c, cl, 0.6, 0.6, 20.0, 0.8));

   c.x = 1000.0;
   c.y = 50.0;
   c.z = 200.0;

   addObject(createSphere(250, c, cl, 0.6, 0.6, 20.0, 0.8));

   
   cl.r = 0.06666;
   cl.g = 0.6980;
   cl.b = 0.3607;

   c.x = 1000.0;
   c.y = 297.0;
   c.z = 200.0;

   addObject(createSphere(10, c, cl, 0.6, 0.6, 20.0, 0.8));
// -------------------------------------- CILINDRO ---------------------------------------------------------
   POINT anchor; // ancla
   anchor.x = 300;
   anchor.y = 300;
   anchor.z = 0;

   VECTOR axis; 
   axis.x = 600.0;
   axis.y = 200.0;
   axis.z = -200.0;


 //  addObject(createCylinder(50, anchor, axis, 10.0, 140.0, cl, 0.7, 0.6, 2, 0.8));

   // ----------------------------------------- LUCES ------------------------------------------------------
   c.x = 600.0;
   c.y = 1000.0;
   c.z = -500.0;
   addLight(createLight(c, 1.0, 1.0, 0.0, 0.0));   


   Ia = 0.6;

   tracer();

   glutMainLoop();

}
