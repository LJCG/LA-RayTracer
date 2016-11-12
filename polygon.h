#ifndef POLYGON_H_   
#define POLYGON_H_

OBJECT createPolygon(POINT *vertices, int numVertices, COLOR color, long double kd, long double ka, long double ks, long double kn, long double o1, long double o2);

double getD(VECTOR normal, POINT punto);

PEQUATION getABCD(POINT *points);

INTERSECTION findIntersection_polygon(VECTOR direction, POINT eye, POLYGON p);

void flattenPolygon(POLYGON p);

POINT2D flattenPoint(POINT intersectionPoint, char tag);

POINT2D* translatePoints(POINT2D* points, int sizePoints, POINT2D intersectionPoint);

bool verifyPoint(POINT2D *points2D, int sizePoints, POINT2D intersectionPoint);

PEQUATION reverse(POLYGON p);

#endif
