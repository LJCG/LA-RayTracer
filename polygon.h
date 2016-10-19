#ifndef POLYGON_H_   
#define POLYGON_H_



OBJECT createPolygon(POINT *vertices, int numVertices, COLOR color, long double kd, long double ka);
// void flattenPolygon();
double getD(VECTOR normal, POINT punto);
INTERSECTION findIntersection_polygon(VECTOR direction, POINT eye, POINT *points);

#endif
