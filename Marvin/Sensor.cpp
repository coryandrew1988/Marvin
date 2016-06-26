#include "Sensor.h"

Sensor::Sensor(Robot* robot, IOPins pin, Radians orientation, Inches offset) {
    // We assume that the Sensor is pointed directly away from the center of the robot. Is this true? Yes
    this->robot = robot;
    this->pin = pin;
    this->orientation = orientation;
    this->offset = offset;
}

Inches microsecondsToInches(Microseconds microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return (microseconds / (73.746 * 2));
}

Inches Sensor::getFreeDistance() {
  Inches total = 0;
  Inches high = -1;
  Inches low = -1;
  //Serial.begin(9600);
  for(int i = 0; i < SAMPLE_SIZE; i++){
    Inches sample = this->readSample();
    //Serial.println(sample);
    if (low == -1 || sample < low) { low = sample; }
    if (high == -1 || sample > high) { high = sample; }
    total += sample;
  }
  //Serial.flush();
  //Serial.end();
  total -= low;
  total -= high;
  return total / (SAMPLE_SIZE - 2);
}
Inches Sensor::readSample(){
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  IOPins pin = this->pin;
  Microseconds duration;
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

Point Sensor::getObstacle() {
    //Serial.begin(9600);    
    Inches sampleDistance = this->getFreeDistance();
    //Serial.println(sampleDistance);  
    Radians sensorFacing = this->robot->getOrientation() + this->orientation;
    //Serial.println(sensorFacing);    
    Point center = this->robot->getCenter();
    //Serial.println(center.toString()); 
    Inches sampleDistanceFromCenter = this->offset + sampleDistance;
    //Serial.println(sampleDistanceFromCenter);  
    
    Radians angleFactorX = cos(sensorFacing);
    Radians angleFactorY = sin(sensorFacing);
   // Serial.println(angleFactorX); 
    //Serial.println(angleFactorY); 
        
    Inches obsX = center.x + sampleDistanceFromCenter * angleFactorX;
    Inches obsY = center.y + sampleDistanceFromCenter * angleFactorY;
    //Serial.println(obsX); 
    //Serial.println(obsY); 
        
    Point obstacle;
    obstacle.x = obsX;
    obstacle.y = obsY;
            
    return obstacle;    
}

