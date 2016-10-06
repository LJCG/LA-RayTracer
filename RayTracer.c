// Autores: Lucy Chaves, Andrés Salazar
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "objects.h"
#include "operations.h"

// Necesario para leer el archivo avs
#define SWAP(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))
         
  
COLOR **frameBuffer;
POINT eye;
COLOR background;

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

// Enciende un pixel en las coordenadas x, y
void plot(int x, int y){ 
	if(x < H_SIZE && y < V_SIZE){
		frameBuffer[x][y].r = 1.0;
		frameBuffer[x][y].g = 1.0;
		frameBuffer[x][y].b = 1.0;
		
		glColor3f (frameBuffer[x][y].r,frameBuffer[x][y].g,frameBuffer[x][y].b);
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
		//glFlush();		
	}
}



void tracer(){
	int i, j;
	POINT w; // (xw, yw, zw)
	POINT d; // (xd, yd, zd)
	COLOR color;
	
	
	for(i = 0; i < H_SIZE; i++){
		for(j = 0; j < V_SIZE; j++){
		
			//mapXY(int x, int y, int xmax, int ymax, int xmin, int ymin)
			w = mapXY(i, j, 10, 10, 0, 0); // no se cuales son los valores de xmin, ymin,...
			d = normalizeVector(w);
			//color = getColor(eye, d);
			frameBuffer[i][j] = color;
		}
	}
	
	//saveFile(frameBuffer);
}

void setBackground(double r, double g, double b){
   background.r = r;
   background.g = g;
   background.b = b;
}

void setWindow(int pxmin, int pymin, int pxmax, int pymax){
	xmin = pxmin;
	ymin = pymin;
	xmax = pxmax;
	ymax = pymax;

}

/*COLOR getColor(POINT vectorW, POINT vectorD){
	COLOR color;
	POINT intersection;

	intersection = firstIntersection(vectorW, vectorD);
	if(!intersection){
		color = background;
	}

	else{
		color = intersection -> objeto.color;
	}

	return color;

}*/



int main(int argc, char** argv){
   init_buffer();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(H_SIZE,V_SIZE);
   glutCreateWindow("Proyecto 2: RayTracer");
   glClear(GL_COLOR_BUFFER_BIT);
   gluOrtho2D(-0.5, H_SIZE +0.5, -0.5, V_SIZE + 0.5);
   glutDisplayFunc(draw_scene);
   

   

   glutMainLoop();
	
}


