#include <Servo.h>
#include "Robot.h"
#include "World.h"
#include "Sensor.h"
#include "math.h"
#include "Point.h"
#include "IOPins.h"

Point destination(16, 52);
Robot robot;
World world(destination);
Point nextNavPoint;

void setup() {
  /*
  robot.startPosition();
  updateWorldObstacles();
  printInformation();
  */  
  /*Testing Movement
  robot.startPosition();
  robot.printRobot();
  Point test;
  Inches testX;
  Inches testY;  
  /*one spot to the left
  testX = (robot.getCenter().x) - 5;
  testY = (robot.getCenter().y);
  //test robot's moveTo method
  test.x = testX;
  test.y = testY;
  Serial.begin(9600);
  Serial.print(test.toString());
  robot.moveTo(test);
  /*/
  /*one spot to the right
  testX = (robot.getCenter().x) + 5;
  testY = (robot.getCenter().y);
  //test robot's moveTo method
  test.x = testX;
  test.y = testY;
  robot.moveTo(test);
  /*/
  /*one spot forward
  testX = (robot.getCenter().x);
  testY = (robot.getCenter().y) + 5;
  //test robot's moveTo method
  test.x = testX;
  test.y = testY;
  Serial.begin(9600);
  Serial.print(test.toString());
  robot.moveTo(test);
  /*/
  /*one spot backward
  testX = (robot.getCenter().x);
  testY = (robot.getCenter().y) - 5;
  //test robot's moveTo method
  test.x = testX;
  test.y = testY;
  Serial.begin(9600);
  Serial.print(test.toString());
  robot.moveTo(test);
  /*/
  
  //*
  robot.startPosition();
  robot.printRobot();
  nextNavPoint = world.nextStepNavigate(robot.getCenter());
  printNextPoint();
  robot.moveTo(nextNavPoint);
  printRobotandGrids();
  while(1){
      nextNavPoint = world.nextStepNavigate(robot.getCenter());
      printRobotandGrids();
      printNextPoint();
      robot.moveTo(nextNavPoint);
      updateWorldObstacles();
      //Check if reached destination
      if (world.isAtDestination(robot.getCenter())) { break; }
  }
  /*/
  /*
  Point test;
  Point obstacle;
  Inches testX;
  Inches testY;
  robot.startPosition();
  obstacle = robot.frontSensor->getObstacle();
  world.registerObstacle(obstacle);
  world.printOGrid();
  testX = (robot.getCenter().x);
  testY = (robot.getCenter().y) + 5;
  //test robot's moveTo method
  test.x = testX;
  test.y = testY;
  robot.moveTo(test);
  obstacle = robot.frontSensor->getObstacle();
  world.registerObstacle(obstacle);
  world.printOGrid();
  /*/
 
  /*
  int numTrials = 7;
  robot.startPosition();
  robot.printRobot();
  nextNavPoint = world.nextStepNavigate(robot.getCenter());
  robot.moveTo(nextNavPoint);
  for (int z=0; z < numTrials; z++){
    nextNavPoint = world.nextStepNavigate(robot.getCenter());
    printRobotandGrids();
    printNextPoint();
    robot.moveTo(nextNavPoint);
    updateWorldObstacles();   
  }
  /*/
  /*
  robot.startPosition();
  world.registerObstacle(robot.rightSensor->getObstacle());
  world.printOGrid();
  /*/
  /*
  //Test turning
  robot.turn(M_PI_2);
  robot.printRobot();
  delay(1000);
  robot.turn(-M_PI_2);
  robot.printRobot();
  /*/
  /*
  //Test Forward Speed
  robot.moveForward(1);
  */   
  //reached destination tone
  tone(PIN_BUZZER, 3000, 1000);
  delay(1000);
  
}

void loop() { }

void updateWorldObstacles() {
  world.registerObstacle(robot.frontSensor->getObstacle());
  //world.registerObstacle(robot.frontRightSensor->getObstacle());
  //world.registerObstacle(robot.frontLeftSensor->getObstacle());
  world.registerObstacle(robot.backSensor->getObstacle());
  world.registerObstacle(robot.rightSensor->getObstacle());
  world.registerObstacle(robot.leftSensor->getObstacle());
}

void printRobotandGrids(){
    robot.printRobot();
    world.printOGrid();
    world.printHGrid();
}
void printNextPoint(){
  Serial.begin(9600);
  Serial.print("Next Nav: ");
  Serial.print(nextNavPoint.toString());
  Serial.println();
  Serial.flush();
  Serial.end();
}


