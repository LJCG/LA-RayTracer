#ifndef DISK_H_   
#define DISK_H_

OBJECT createDisk(double radius, POINT center, COLOR color, POINT p1, POINT p2);

INTERSECTION findIntersection_disk(VECTOR d, POINT eye, DISK disk);

#endif