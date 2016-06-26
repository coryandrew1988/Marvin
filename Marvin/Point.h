#ifndef Point_h
#define Point_h
#include "Arduino.h"
typedef double Radians;
typedef float Inches;
typedef long Microseconds;

class Point {
public:
  Inches x, y;
  Point();
  Point(Inches, Inches);
  double calculateDistance(Point);
  String toString();  
};

#endif
