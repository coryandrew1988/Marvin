#include "Point.h"
#include "math.h"



Point::Point() {
  this->x = 0;
  this->y = 0;
}

Point::Point(Inches x, Inches y) {
  this->x = x;
  this->y = y;
}

double Point::calculateDistance(Point other){
  return sqrt(square(other.x - this->x) + square(other.y - this->y));
}

String Point::toString() {
  //converts the point variables into a string
  char buff[6];
  char buff0[6];
  String strx = dtostrf(this->x, 4, 2, buff);
  String stry = dtostrf(this->y, 4, 2, buff0);
  String point = "(" + strx + "," + stry + ")";
  return point;
}
