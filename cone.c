#include "objects.h"
#include "operations.h"
#include "cylinder.h"
#include <math.h>
#include <stdio.h>



long double getNormalCone(void){
}


OBJECT createCone(void){
}



INTERSECTION findIntersection_cone(VECTOR d, POINT e, POINT o, double radius, VECTOR q, double d1,double d2,double k1, double k2){
  //d direction; e ojo; o ancla; q eje; d1 tapa1,d2 tapa2,k1,k2;

  INTERSECTION intersection;

  long double var1,var2,var3,var4,var5,var6;

  var1 = (pow(q.x,2)*d.x)+(q.x*d.y*q.y)+(q.x*d.z*q.z)-d.x;
  var2 = (pow(q.y,2)*d.y)+(d.x*q.x*q.y)+(q.y*d.z*q.z)-d.y;
  var3 = (pow(q.z,2)*d.z)+(d.x*q.x*q.z)+(d.y*q.y*q.z)-d.z;
  var4 = (e.x-o.x);
  var5 = (e.y-o.y);
  var6 = (e.z-o.z);
  var7 = (d.x*q.x) + (d.y*q.y) + (d.z*q.z);
  var8 = pow((k2/k1),2);


  double a = pow(var1,2)+pow(var2,2)+pow(var3,2) - (var8*pow(var7,2));

  double b = (2*var1*var4*(pow(q.x,2)-1))+(2*var1*q.x*((q.y*var5)+(q.z*var6)))
  +(2*var2*var5*(pow(q.y,2)-1))+(2*var2*q.y*((q.x*var4)+(q.z*var6)))
  +(2*var3*var6*(pow(q.z,2)-1))+(2*var3*q.z*((q.x*var4)+(q.y*var5)))
  -(var8*2*var7*var4*q.x)-(var8*2*var7*var5*q.y)-(var8*2*var7*var6*q.z);

  double c = pow(((pow(q.x,2)-1)*var4),2)+pow((q.x*((q.y*var5)+(q.z*var6))),2)
  +pow(((pow(q.y,2)-1)*var5),2)+pow((q.y*((q.x*var4)+(q.z*var6))),2)
  +pow(((pow(q.z,2)-1)*var6),2)+pow((q.z*((q.x*var4)+(q.y*var5))),2)
  +(2*var4*q.x*(pow(q.x,2)-1)*((q.y*var5)+(q.z*var6)))
  +(2*var5*q.y*(pow(q.y,2)-1)*((q.x*var4)+(q.z*var6)))
  +(2*var6*q.z*(pow(q.z,2)-1)*((q.x*var4)+(q.y*var5)))
  -(var8*pow((q.x*var4),2))
  -(var8*pow((q.y*var5),2))
  -(var8*pow((q.z*var6),2))
  -(var8*2*var4*var5*q.x*q.y)
  -(var8*2*var4*var6*q.x*q.z)
  -(var8*2*var5*var6*q.y*q.z);



  double discriminante = pow(b, 2) - 4*a*c;


  if(discriminante > 0){

}}
