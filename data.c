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

void globalConfig()
{
	FILE *file;
	char line[50];
	char *temp;
	char *id;
	double r;
	double g;
	double b;
  int pxmin;
  int pymin;
  int pxmax;
  int pymax;//resolution
  double xe;
  double ye;
  double ze;//ojo

	long double i;
	long double c1;
	long double c2;
	long double c3;
	double x;
	double y;
	double z;

	POINT c;

	file = fopen ("global_config.txt", "rt");

	while(fgets(line, 50, file) != NULL)
	{
		temp = strtok(line, ",");
		if(strcmp(temp,"\n") == 0 || strncmp(temp,"#",1) == 0)
		continue;

		id=(char*)temp;


		if(strcmp(temp,"$Eye")==0){

			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &xe);

			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &ye);

			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &ze);

      setEye(xe,ye,ze);
		}

		else if(strcmp(temp,"$Window")==0){

			temp = strtok(NULL, ",");
			sscanf (temp, "%d", &pxmin);

			temp = strtok(NULL, ",");
			sscanf (temp, "%d", &pymin);

			temp = strtok(NULL, ",");
			sscanf (temp, "%d", &pxmax);

			temp = strtok(NULL, ",");
			sscanf (temp, "%d", &pymax);

      setWindow(pxmin,pymin,pxmax,pymax);
		}

		else if(strcmp(temp,"$Background")==0){

			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &r);

			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &g);

			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &b);

      setBackground(r,g,b);


		}
/*
		else if(strcmp(temp,"$Light1")==0){

			//cx,cy,cz,i,c1,c2,c3
			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &x);
printf("%lf\n",x );

			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &y);
printf("%lf\n",y );
			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &z);
printf("%lf\n",z );
			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &i);
printf("%Lg\n",i );
			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &c1);
printf("%Lg\n",c1 );
			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &c2);
printf("%Lg\n",c2);
			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &c3);
printf("%Lg\n",c3 );
			c.x = x;
			c.y = y;
			c.z = z;
			addLight(createLight(c, i, c1, c2, c3));

		}

		else if(strcmp(temp,"$Light2")==0){
//cx,cy,cz,i,c1,c2,c3
			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &x);

			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &y);

			temp = strtok(NULL, ",");
			sscanf (temp, "%lf", &z);

			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &i);

			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &c1);

			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &c2);

			temp = strtok(NULL, ",");
			sscanf (temp, "%Lg", &c3);


			c.x = x;
			c.y = y;
			c.z = z;
			addLight(createLight(c, i, c1, c2, c3));*/



	}


}


void loadInfo(){

  FILE *file;
  char line[100];
  char *temp = (char*)calloc(MAX_CHARS,sizeof(char));
  char *id = (char*)calloc(MAX_CHARS,sizeof(char));
  double r;
  double g;
  double b;

  double cx;
  double cy;
  double cz; //centro esfera
  double x;
  double y;
  double z;

  //para todos los objetos
  long double kd;
  long double ka;
  long double kn;
  long double ks;


  long double radius;
  //solo para el cono
  long double k1;
  long double k2;
  //para cono y cilindro
  long double d1;
  long double d2;

  double xa;
  double ya;
  double za; //axis de cono y cilindro
  int numVertices; //para poligonos
  double xp;
  double yp;
  double zp; //para guardar x,y,z de poligonos
  //centro y color de esfera
  POINT c;
  COLOR cl;
  COLOR background;
  POINT eye;

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
        addObject(createSphere(radius, c, cl, kd, ka, kn, ks, 0.0, 0.0));

      }
      else if(strcmp(id,"$Cylinder\n") == 0){

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

          anchor.x = x;
          anchor.y = y;
          anchor.z = z;

          axis.x = xa;
          axis.y = ya;
          axis.z = za;

          cl.r = r;
          cl.g = g;
          cl.b = b;




          addObject(createCylinder(radius, anchor, rotate_cone(axis,degrees), d1, d2, cl, kd, ka, kn, ks, 0.0, 0.0));

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


          addObject(createCone(radius, anchor, rotate_cone(axis,degrees),d1, d2,cl, k1, k2, kd, ka, kn, ks, 0.0, 0.0));

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
          	fgets(line, 50, file);
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


        OBJECT p = createPolygon(points, numVertices, cl,kd,ka,ks,kn, 0.0, 0.0);
        p.polygon.equation = reverse(p.polygon);

        addObject(p);
        //libera memoria dinamica
        free(points);
      }


  }


}
