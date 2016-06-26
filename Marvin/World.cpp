#include "World.h"
#include "math.h"
World::World(Point destination) : destination(destination){
    for (int i = 0; i < scaledXMax; i++) {
        for (int j = 0; j < scaledYMax; j++) {
            this->oGrid[i][j] = 0;
        }
    }
    
    this->updateHGrid();
}

void World::resetHGrid(){
  for (int i = 0; i < hGridXMax; i++) {
    for (int j = 0; j < hGridYMax; j++) {
      this->hGrid[i][j] = -1;
    }
  }
}

void World::updateHGrid(){
  this->resetHGrid();
  this->offerHGridValue(this->destination, 0);
}

void World::offerHGridValue(GridPoint cell, int value){
    if(!isGridSectionFree(cell)) { return; }
    if(value > 30) { return; } 

    int gridValue = this->hGrid[cell.x - 1][cell.y - 1];
    if(0 <= gridValue && gridValue <= value) { return; }

    this->hGrid[cell.x - 1][cell.y - 1] = value;
    
    // Print the hGrid at this point in time to see how it all updates.
    //this->printHGrid();
    offerHGridValue(GridPoint(cell.x + 1, cell.y), value + 1);
    offerHGridValue(GridPoint(cell.x - 1, cell.y), value + 1);
    offerHGridValue(GridPoint(cell.x, cell.y + 1), value + 1);
    offerHGridValue(GridPoint(cell.x, cell.y - 1), value + 1);
}

void World::printHGrid(){
  Serial.begin(9600);
  Serial.println("Heuristic Grid:");
    for (int j = hGridYMax - 1; 0 < j; j--) {
        for (int i = 0; i < hGridXMax; i++){
            int value = this->hGrid[i][j];
            if(0 <= value && value < 10) {Serial.print(" ");}
            Serial.print(value);
            Serial.print("  ");
        }
        Serial.println();
    }
  Serial.println();
  Serial.flush();
  Serial.end();  
}

void World::printOGrid(){
  Serial.begin(9600);
  Serial.println("Obstacle Grid:");
    for (int j = scaledYMax - 1; 0 < j; j--) {
        for (int i = 0; i < scaledXMax; i++){
            Serial.print(this->oGrid[i][j]);
            Serial.print("  ");
        }
        Serial.println();
    }
  Serial.println();
  Serial.flush();  
  Serial.end();
}

void World::registerObstacle(Point obstaclePoint) {
    if (obstaclePoint.x <= xMax && obstaclePoint.y <= yMax){ 
        GridPoint gp(obstaclePoint);
        if (oGrid[gp.x][gp.y] == 0) {
            this->oGrid[gp.x][gp.y] = 1;
            updateHGrid();
        }
    }
}

boolean World::isFree(GridPoint point) {
    return (
        0 <= point.x && point.x < scaledXMax &&
        0 <= point.y && point.y < scaledYMax &&
        this->oGrid[point.x][point.y] == 0
    );
}

boolean World::isGridSectionFree(GridPoint point){
   //Check to make sure all points on the grid section are free
   
   GridPoint down(point.x, point.y - 1); 
   GridPoint left(point.x - 1, point.y);
   GridPoint downLeft(point.x - 1, point.y - 1);
      
   return isFree(point) && isFree(down) && isFree(left) && isFree(downLeft); 
}

Point World::nextStepNavigate(Point center) {
  HGridPoint start(center);

  // Get adjacent points
  HGridPoint up(start.x, start.y + 1);
  HGridPoint down(start.x, start.y - 1);
  HGridPoint right(start.x + 1, start.y);
  HGridPoint left(start.x - 1, start.y);

  //Print gridpoints for testing
  Serial.begin(9600);
  Serial.print(start.toString());
  Serial.println();
  
  const int optionsLength = 4;
  HGridPoint options[optionsLength] = {up, left, right, down};

  HGridPoint nearest;
  int nearestDistance = highestHValue; // Just a value too large to ever come out as the minimum.
  // Check all options to find the best one.
    for (int i = 0; i < optionsLength; i++){
        HGridPoint option = options[i];
        Serial.print("option: ");
        Serial.print(option.toString());
        Serial.println();
        if (hGrid[option.x][option.y] == -1) { continue; }
        
        int distance = calculateHeuristicDistance(option);
        Serial.print("distance: ");
        Serial.print(distance);
        Serial.println();
        if (distance >= nearestDistance) { continue; }
        
        nearestDistance = distance;
        Serial.print("nearestDistance: ");
        Serial.print(nearestDistance);
        Serial.println();
        nearest = option;
        Serial.print("nearest: ");
        Serial.print(nearest.toString());
        Serial.println();        
    }
  Serial.flush();
  Serial.end();
  return nearest.getPoint();
}
boolean World::isAtDestination(Point center){
    HGridPoint gp(center);
    return (calculateHeuristicDistance(gp) == 0);  
}
int World::calculateHeuristicDistance(HGridPoint gp) {
  if (0 > gp.x || gp.x >= hGridXMax || 0 > gp.y || gp.y >= hGridYMax) { return highestHValue; }
  return this->hGrid[gp.x][gp.y];
}
