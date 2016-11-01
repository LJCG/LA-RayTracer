#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "objects.h"
#include "string.h"
#include "sphere.h"
#include "cylinder.h"
#include "cone.h"
#include "polygon.h"
#include "operations.h"

/*
  1- En caso de error: verificar que la identacion sea la correcta
  2- Que el tipo de datos coincida con el definido en loadInfo
  3- El orden debe ser el mismo que en el archivo
  4- Siempre hay una solucion, el suicidio.

*/


void loadInfo(){

  FILE *file;
  char line[100];
  char *temp = (char*)calloc(MAX_CHARS,sizeof(char));
  char *id = (char*)calloc(MAX_CHARS,sizeof(char));
  double r,g,b;//colores
  double cx,cy,cz; //centro esfera
  double x,y,z;
  double xe,ye,ze;//ojo
  //para todos los objetos
  long double kd;
  long double ks;
  long double kn;
  long double ka;
  long double radius;
  //solo para el cono
  long double k1;
  long double k2;
  //para cono y cilindro
  long double d1;
  long double d2;
  double pxmin,pymin,pxmax,pymax;//resolution
  double xa,ya,za; //axis de cono y cilindro
  int numVertices; //para poligonos
  double xp,yp,zp; //para guardar x,y,z de poligonos
  //centro y color de esfera
  POINT c;
  COLOR cl;

  POINT anchor;
  VECTOR axis;

  //rotacion en grados para el cono
  int degrees;


  file = fopen("data.txt","rt");

  while(fgets(line, sizeof(line),file) != NULL)
  {
    if(strcmp(line,"\n") == 0 || strncmp(line,"#",1) == 0)
      continue;
      if(strncmp(line,"$",1)==0)
      {
        strcpy(id,line);

        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tcolor = %lf,%lf,%lf",&r,&g,&b);

        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tkd = %Lg",&kd);

        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tka = %Lg",&ka);

        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tkn = %Lg",&kn);

        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tks = %Lg",&ks);


      }
/*
$Eye
  eye_position = 200.0,200.0,-1500

$Window
  resolution = 0,0,1008,567

$Background
  color = 0.0,0.0,0.0


      if(strcmp(id,"$Eye\n") == 0){
        fgets(line,MAX_CHARS,file);
        sscanf(line,"\teye_position = %lf,%lf,%lf",&xe,&ye,&ze);
        printf("%lf,%lf,%lf\n",xe,ye,ze);
        setEye(xe,ye,ze);
      }
      else if(strcmp(id,"$Window\n") == 0){
        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tresolution = %lf,%lf,%lf,%lf",&pxmin,&pymin,&pxmax,&pymax);
        printf("%lf,%lf,%lf,%lf\n",pxmin,pymin,pxmax,pymax);
        setWindow(pxmin,pymin,pxmax,pymax);
      }
      else if(strcmp(id,"$Background\n") == 0){
        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tcolor = %lf,%lf,%lf",&r,&g,&b);
        background.r = r;
        background.g = g;
        background.b = b;
        printf("%lf,%lf,%lf\n",r,g,b);
        setBackground(r,g,b);
      }*/
      if(strcmp(id,"$Sphere\n") == 0){

        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tradius = %Lg",&radius);

        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tcenter = %lf,%lf,%lf",&cx,&cy,&cz);
        //centro de esfera
        c.x = cx;
        c.y = cy;
        c.z = cz;
        //color de esfera
        cl.r = r;
        cl.g = g;
        cl.b = b;
        //crea objeto esfera
        addObject(createSphere(radius, c, cl, kd, ka, kn, ks));

      }
      else if(strcmp(id,"$Cylinder\n") == 0){

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\tradius = %Lg",&radius);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\tanchor = %lf,%lf,%lf",&x,&y,&z);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\taxis = %lf,%lf,%lf",&xa,&ya,&za);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\td1 = %Lf",&d1);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\td2 = %Lf",&d2);

          anchor.x = x;
          anchor.y = y;
          anchor.z = z;

          axis.x = xa;
          axis.y = ya;
          axis.z = za;

          cl.r = r;
          cl.g = g;
          cl.b = b;

          addObject(createCylinder(radius, anchor, axis, d1, d2, cl, kd, kn, ka, ks));

      }
      else if(strcmp(id,"$Cone\n") == 0){

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\tradius = %Lg",&radius);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\tanchor = %lf,%lf,%lf",&x,&y,&z);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\taxis = %lf,%lf,%lf",&xa,&ya,&za);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\trotation = %d",&degrees);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\td1 = %Lf",&d1);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\td2 = %Lf",&d2);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\tk1 = %Lf",&k1);

          fgets(line,MAX_CHARS,file);
          sscanf(line,"\tk2 = %Lf",&k2);

          anchor.x = x;
          anchor.y = y;
          anchor.z = z;

          axis.x = xa;
          axis.y = ya;
          axis.z = za;

          cl.r = r;
          cl.g = g;
          cl.b = b;

          addObject(createCone(radius, anchor, rotate_cone(axis,degrees),d1, d2,cl, k1, k2, kd, ka, kn, ks));

      }
      else if(strcmp(id,"$Polygon\n") == 0){

        fgets(line,MAX_CHARS,file);
        sscanf(line,"\tnum_vertices = %d",&numVertices);

        int pts_line=0;
        /*Memoria dinamica para guardar puntos [x,y,z]
        de tipo POINT a partir de numero de vertices*/
        POINT* points = (POINT*) calloc(numVertices,sizeof(POINT));
        fgets(line,100,file);
        temp = strtok(line, ",");
        //iteracion para guardar arreglos[x,y,z] en memoria dinamica
        for(int i=0;i<numVertices;i++)
        {
          sscanf (temp, "\t[%lf#%lf#%lf]", &xp,&yp,&zp);
          POINT point;
          point.x = xp;
          point.y = yp;
          point.z = zp;
          points[i] = point;

          if(pts_line==1){
            //maximo de caracteres grande por decimales
          	fgets(line, 100, file);
          	pts_line = 0;
          	temp = strtok(line, ",");
          }
            else{
            pts_line++;
            temp = strtok(NULL, ",");
          }

}

        cl.r = r;
        cl.g = g;
        cl.b = b;

        OBJECT p = createPolygon(points, numVertices, cl, kd, ka, ks, kn);
        p.polygon.equation = reverse(p.polygon);

        addObject(p);
        //libera memoria dinamica
        free(points);
      }


  }


}
