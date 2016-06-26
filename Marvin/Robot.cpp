#include "Robot.h"

Robot::Robot() {

    this->center.x = 0;
    this->center.y = 0;
    this->orientation = 0;
    
    this->frontSensor = new Sensor(this, PIN_FRONT, M_PI_2, verticalOffset);
    this->backSensor = new Sensor(this, PIN_BACK, (-M_PI_2), verticalOffset);
    this->rightSensor = new Sensor(this, PIN_RIGHT, 0, horizontalOffset);
    this->leftSensor = new Sensor(this, PIN_LEFT, M_PI, horizontalOffset);
    this->frontRightSensor = new Sensor(this, PIN_FRONT_RIGHT, (M_PI_4), angleOffset);
    this->frontLeftSensor = new Sensor(this, PIN_FRONT_LEFT, (M_PI_2 + M_PI_4), angleOffset);
    
    //this->sensors = new Sensor[]{ this->frontSensor, this->backSensor, this->rightSensor, this->leftSensor, this->frontRightSensor, this->frontLeftSensor };
}

// We don't need a destructor because we know we will only ever create one Robot instance, and it will live forever.

//Getters
Radians Robot::getOrientation() {
  return this->orientation;
}

Point Robot::getCenter() {
  return this->center;
}

void Robot::startPosition() {
  //This works only if no objects in the way of the four sensors.
  Inches front,left,right,back;
  Inches xDifference;
  Inches yDifference;
  back = this->backSensor->getFreeDistance() + verticalOffset;
  left = this->leftSensor->getFreeDistance() + horizontalOffset;
  this->center.y = back;
  this->center.x = left;
  //loops until it gets a good reading for center point since sensors vary in readings
  /*while (this->center.y == 0){
    front = this->frontSensor->getFreeDistance() + verticalOffset;
    back = this->backSensor->getFreeDistance() + verticalOffset;
    yDifference = yMax - front - back; 

    if (yDifference < .1){
      this->center.y = back;
  /*  }
  }
  while (this->center.x == 0){
    right = this->rightSensor->getFreeDistance() + horizontalOffset;
    left = this->leftSensor->getFreeDistance() + horizontalOffset;
    xDifference = xMax - right - left;
    
    if (xDifference < .1){
      this->center.x = left;
    }
  }*/
}

void Robot::moveForward(double distance){
  double duration = (distance/forwardSpeed) * 1000;

  this->servoLeft.attach(PIN_SERVO_LEFT); 
  this->servoRight.attach(PIN_SERVO_RIGHT);
  this->servoLeft.writeMicroseconds(forwardLeftSpeed);
  this->servoRight.writeMicroseconds(forwardRightSpeed);
  delay(duration);

  // Update current position while moving
  this->center.x += distance * cos(this->orientation + M_PI_2);
  this->center.y += distance * sin(this->orientation + M_PI_2);

  this->servoLeft.detach();
  this->servoRight.detach();
}
void Robot::moveTo(Point point){
  //Robot move toward a desired point in the World grid
  double distance;
  distance = sqrt(square(point.x - this->center.x) + square(point.y - this->center.y));
  adjustHeading(point);
  moveForward(distance);
  
}

void Robot::adjustHeading(Point point){
  // adjusts heading toward destination
  double xHeading = point.x - this->center.x;
  double yHeading = point.y - this->center.y;
  double headingRadians = atan2(yHeading, xHeading) - this->orientation - M_PI_2;
  this->turn(headingRadians);
}

void Robot::turn(Radians angle) {
  const int TAU = 2 * M_PI;
  while (angle > TAU) { angle -= TAU; }
  while (angle < -TAU) { angle += TAU; }

  //update robot facing for turning
  this->orientation += angle;
  
  // Turn Left angle radians / Turn Right -angle radians 
  boolean isLeft = angle > 0;
  
  // These values come from calibrating the robot
  int directionCode = isLeft ? 1300 : 1700;
  int duration = (int)(abs(angle) / M_PI_2 * leftTurnDuration) - (isLeft ? 0 : rightTurnAdjustment);
  
  this->servoLeft.attach(PIN_SERVO_LEFT); 
  this->servoRight.attach(PIN_SERVO_RIGHT); 
  this->servoLeft.writeMicroseconds(directionCode);
  this->servoRight.writeMicroseconds(directionCode);
  delay(duration);
  this->servoLeft.writeMicroseconds(1500);
  this->servoRight.writeMicroseconds(1500);
  this->servoLeft.detach();
  this->servoRight.detach();
}
void Robot::printRobot(){
    Serial.begin(9600);
    Serial.print("Center: ");
    Serial.print(this->center.toString());
    Serial.print("  Orientation: ");
    Serial.print(orientation);
    Serial.println();
    Serial.flush();
    Serial.end();
}
