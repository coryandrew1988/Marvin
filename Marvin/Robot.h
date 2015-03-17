#ifndef Robot_h
#define Robot_h

#include "Point.h"
#include "World.h"
#include "Arduino.h"
#include "math.h"

typedef int Angle;

class Robot {
public:
    Robot();
    void turn(Angle);
    void startPosition();
    void moveForward(int);
    void detectObstacle(World&, Point);
//private:
    Point center;
    Angle facing;
    Servo servoLeft;                             
    Servo servoRight;
};

const int pinFront = 4;
const int pinRight = 2;
const int pinLeft = 8;
const int pinBack = 9;
const int pinFrontRight = 3;
const int pinFrontLeft = 10;

const int verticalOffset = 4;
const int horizontalOffset = 3;

const int pinServoRight = 11;
const int pinServoLeft = 12;

const long forwardSpeed = 5.0;
const int forwardLeftSpeed = 1700;
const int forwardRightSpeed = 1400;

const int leftTurnDuration = 1200;
const int rightTurnAdjustment = 400;

const int faceForward = 0;
const int faceLeft = 1;
const int faceRight = 3;
const int faceBackward = 2;

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return (long)(microseconds / (74 * 2));
}

long getFreeDistance(int pin) {
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  long duration;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
  //get reading from Front Sensor
  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH);
  delay(100);
  return microsecondsToInches(duration);
}

Robot::Robot() {
  this->center.x = 0;
  this->center.y = 0;
  this->facing = 0;
}

void Robot::startPosition() {
  long front,left,right,back;
  int xDifference;
  int yDifference;
  for (int i = 0; i < 6; i++) {
    front = getFreeDistance(pinFront) + verticalOffset;
    right = getFreeDistance(pinRight) + horizontalOffset;
    back = getFreeDistance(pinBack) + verticalOffset;
    left = getFreeDistance(pinLeft) + horizontalOffset;
    xDifference = xMax - right - left;
    yDifference = yMax - front - back; 

    if (yDifference == 0){
      this->center.y = back;
    }
    if (xDifference == 0){
      this->center.x = left;
    }
  }
}

void Robot::detectObstacle(World& world, Point obstacle){
  long front, frontLeft, frontRight;
  front = getFreeDistance(pinFront);
  frontLeft = getFreeDistance(pinFrontLeft);
  frontRight = getFreeDistance(pinFrontRight);
  
  if (this->facing == faceForward){
    //Front sensor obstacle
    obstacle.x = this->center.x;
    obstacle.y = this->center.y + front + verticalOffset;
    world.registerObstacle(obstacle);
    //Front Left sensor obstacle
    obstacle.x = this->center.x - (int)(frontLeft * sin(M_PI_4));
    obstacle.y = this->center.y + (int)(frontLeft * cos(M_PI_4));
    world.registerObstacle(obstacle);
    //Front Right sensor obstacle
    obstacle.x = this->center.x + (int)(frontRight * sin(M_PI_4));
    obstacle.y = this->center.y + (int)(frontRight * cos(M_PI_4));
    world.registerObstacle(obstacle);
  }
  if (this->facing == faceLeft){
    //Front sensor obstacle
    obstacle.y = this->center.y;
    obstacle.x = this->center.x - front - verticalOffset;
    world.registerObstacle(obstacle);
    //Front Left sensor obstacle
    obstacle.y = this->center.y - (int)(frontLeft * sin(M_PI_4));
    obstacle.x = this->center.x - (int)(frontLeft * cos(M_PI_4));
    world.registerObstacle(obstacle);
    //Front Right sensor obstacle
    obstacle.y = this->center.y + (int)(frontRight * sin(M_PI_4));
    obstacle.x = this->center.x - (int)(frontRight * cos(M_PI_4));
    world.registerObstacle(obstacle);
  }
  if (this->facing == faceRight){
    //Front sensor obstacle
    obstacle.y = this->center.y;
    obstacle.x = this->center.x + front + verticalOffset;
    world.registerObstacle(obstacle);
    //Front Left sensor obstacle
    obstacle.y = this->center.y + (int)(frontLeft * sin(M_PI_4));
    obstacle.x = this->center.x + (int)(frontLeft * cos(M_PI_4));
    world.registerObstacle(obstacle);
    //Front Right sensor obstacle
    obstacle.y = this->center.y - (int)(frontRight * sin(M_PI_4));
    obstacle.x = this->center.x + (int)(frontRight * cos(M_PI_4));
    world.registerObstacle(obstacle);
  }
  if (this->facing == faceBackward){
    //Front sensor obstacle
    obstacle.x = this->center.x;
    obstacle.y = this->center.y - front - verticalOffset;
    world.registerObstacle(obstacle);
    //Front Left sensor obstacle
    obstacle.x = this->center.x + (int)(frontLeft * sin(M_PI_4));
    obstacle.y = this->center.y - (int)(frontLeft * cos(M_PI_4));
    world.registerObstacle(obstacle);
    //Front Right sensor obstacle
    obstacle.x = this->center.x - (int)(frontRight * sin(M_PI_4));
    obstacle.y = this->center.y - (int)(frontRight * cos(M_PI_4));
    world.registerObstacle(obstacle);
  }
}

void Robot::moveForward(int distance){
  long duration;
  duration = (distance/forwardSpeed) * 1000;
  this->servoLeft.attach(pinServoLeft); 
  this->servoRight.attach(pinServoRight);
  this->servoLeft.writeMicroseconds(forwardLeftSpeed);
  this->servoRight.writeMicroseconds(forwardRightSpeed);
  delay(duration);
  if (this->facing == faceForward){
    this->center.y = this->center.y + distance;
  }
  if (this->facing == faceLeft){
    this->center.x = this->center.x - distance;
  }
  if (this->facing == faceBackward){
    this->center.y =this->center.y - distance;
  }
  if (this->facing == faceRight){
    this->center.x = this->center.x + distance;
  }
  this->servoLeft.detach();
  this->servoRight.detach();
}
void Robot::turn(Angle angle) {
  // Turn Left angle degrees / Turn Right -angle degrees

  boolean isLeft = angle > 0;

  angle = abs(angle);

  // These values come from calibrating the robot and stuff, you know whatevs.
  int directionCode = isLeft ? 1300 : 1700;
  int duration = (int)(angle / 90.0 * leftTurnDuration) - (isLeft ? 0 : rightTurnAdjustment);
  
  this->servoLeft.attach(pinServoLeft); 
  this->servoRight.attach(pinServoRight); 
  this->servoLeft.writeMicroseconds(directionCode);
  this->servoRight.writeMicroseconds(directionCode);
  delay(duration);
  this->servoLeft.writeMicroseconds(1500);
  this->servoRight.writeMicroseconds(1500);
  this->servoLeft.detach();
  this->servoRight.detach();
}

#endif

