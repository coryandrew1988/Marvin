#ifndef Sensor_h
#define Sensor_h

class Sensor;

#include "Arduino.h"
#include "math.h"
#include "IOPins.h"
#include "Point.h"
#include "Robot.h"

const int SAMPLE_SIZE = 6;

class Sensor {
public:
    Sensor(Robot*, IOPins, Radians, Inches);
    Inches getFreeDistance();
    Point getObstacle();
//private:
    Robot* robot;
    IOPins pin;
    Radians orientation;
    Inches offset;
    Inches readSample();
};
#endif
