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





// ------------------------------------------ BRESENHAM ------------------------------------------------
void line4_4_8(int x0, int y0, int x1, int y1){
	int d, delta_e, delta_se, xp, yp;
	
	delta_e = 2*(y0-y1);
	delta_se = 2*(y0-y1) + 2*(x0-x1);
	d = 2*(y1-y0) + (x1-x0);
	
	xp = x0;
	yp = y0;
	
	plot(xp,yp);
	
	while(xp < x1){
		if(d <= 0){
			xp++;
			d += delta_e;
		}	
		else{
			xp++;
			yp--;
			d += delta_se;
		}
		plot(xp,yp);
	}	
}

void line4_1_5(int x0, int y0, int x1, int y1){
	int d, delta_e, delta_ne, xp, yp;
	
	delta_ne = 2*(y1-y0)-2*(x1-x0);
	delta_e = 2*(y1-y0);
	d = 2*(y1-y0)-(x1-x0);
	
	xp = x0;
	yp =y0;
	plot(xp, yp);
	
	while(xp < x1){
		if(d <= 0){
			// pintar E
			xp++;
			d += delta_e;
		}
		else{
			//pinta NE
			xp++;
			yp++;
			d += delta_ne;
		}
		plot(xp, yp);
	}
}

void line4_3_7(int x0, int y0, int x1, int y1){
	int d, delta_s, delta_se, xp, yp;
	
	delta_s = -2*(x0-x1);
	delta_se = -2*(y0-y1) + -2*(x0-x1);
	d = (y1-y0) +2*(x1-x0);
	
	xp = x0;
	yp = y0;

	plot(xp,yp);
	
	while(yp != y1){
		if(d <= 0){
			yp--;
			d += delta_s;
		}
		else{
			yp--;
			xp++;
			d += delta_se;
		}
		plot(xp,yp);
	}
}

void line4_2_6(int x0, int y0, int x1, int y1){
	int d, delta_n, delta_ne, xp, yp;
	
	delta_n = 2*(x1-x0);
	delta_ne = -2*(y1-y0)+2*(x1-x0);
	d = (y1-y0) -2*(x1-x0);
	
	xp = x0;
	yp =y0;
	
	plot(xp,yp);
	
	while(yp < y1){
		if(d <= 0){
			yp++;
			d += delta_n;
		}
		else{
			yp++;
			xp++;
			d += delta_ne;
		}
		plot(xp,yp);
	}
}

void line4(int x0, int y0, int x1, int y1){
	double m = (double)(y1 - y0) / (double)(x1 - x0);
	if(x0 > x1){	
		int temp = x0;
		x0 = x1;
		x1 = temp;
		
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	
	if(m >= 0){
		if(m > 1){
			line4_2_6(x0, y0, x1, y1);
		}	
		else{
			line4_1_5(x0, y0, x1, y1);
		}
	}
	else if(m < 0){
		if(m < -1){
			line4_3_7(x0, y0, x1, y1);
		}
		else{
			line4_4_8(x0, y0, x1, y1);
		}
	}
	else{
		line4_2_6(x0, y0, x1, y1);
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


