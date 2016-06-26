#ifndef Robot_h
#define Robot_h

class Robot;
#include "Servo.h"
#include "Arduino.h"
#include "math.h"
#include "Point.h"
#include "World.h"
#include "Sensor.h"
#include "IOPins.h"

const Inches verticalOffset = 3.6;
const Inches horizontalOffset = 3.1;
const Inches angleOffset = 4.6;

const long forwardSpeed = 4.808; //rotation for one second calculated from circumference
const int forwardLeftSpeed = 1700;
const int forwardRightSpeed = 1400;

const int leftTurnDuration = 800;
const int rightTurnAdjustment = 0;

class Robot {
public:
    Robot();
    void turn(Radians);
    void startPosition();
    void moveForward(double);
    Radians getOrientation();
    Point getCenter();
    void moveTo(Point);
    void adjustHeading(Point);
    void printRobot();
//private:
    Point center;
    Radians orientation;
    Servo servoLeft;                             
    Servo servoRight;

    Sensor* frontSensor;
    Sensor* backSensor;
    Sensor* rightSensor;
    Sensor* leftSensor;
    Sensor* frontRightSensor;
    Sensor* frontLeftSensor;
};

#endif
