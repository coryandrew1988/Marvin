#ifndef World_h
#define World_h

#include <Arduino.h>
#include "Point.h"
//used to convert values to strings


const int xMax = 48;
const int yMax = 78;

const int scaleFactor = 4;
const int scaledXMax = (xMax / scaleFactor) + 1;
const int scaledYMax = (yMax / scaleFactor) + 1;
const int hGridXMax = scaledXMax - 1;
const int hGridYMax = scaledYMax - 1;

const int highestHValue = 31;

class GridPoint {
public:
    int x, y;
    GridPoint() {
        this->x = this->y = 0;
    }
    GridPoint(int x, int y) {
        this->x = x;
        this->y = y;
    }
    GridPoint(Point point) {
        this->x = (int)(point.x / scaleFactor);
        this->y = (int)(point.y / scaleFactor);
    }
    Point getPoint() {
        return Point(this->x * scaleFactor, this->y * scaleFactor);
    }
    String toString(){
        //converts the point variables into a string
        Point point = this->getPoint();
        String pointstr = point.toString();
        String strx = String(this->x);
        String stry = String(this->y);        
        String Gpoint = "(" + strx + "," + stry + ")" + " " + pointstr;
        return Gpoint;
    }
};
class HGridPoint : public GridPoint {
public:
    HGridPoint() {
        this->x = this->y = 0;
    }
    HGridPoint(int x, int y) {
        this->x = x;
        this->y = y;
    }
    HGridPoint(Point point) {
        this->x = (int)(point.x / scaleFactor) - 1;
        this->y = (int)(point.y / scaleFactor) - 1;
    }
    Point getPoint() {
        return Point((this->x + 1) * scaleFactor, (this->y + 1) * scaleFactor);
    }
};
class World {
public:
    World(Point);
    void registerObstacle(Point);
    Point nextStepNavigate(Point);
    boolean isGridSectionFree(GridPoint);
    boolean isAtDestination(Point);
    void printHGrid();
    void printOGrid();
private:
    short oGrid[scaledXMax][scaledYMax];
    int calculateHeuristicDistance(HGridPoint);
    
    boolean isFree(GridPoint);

    // Heuristic Grid
    int hGrid[hGridXMax][hGridYMax];
    void resetHGrid();
    void updateHGrid();
    void offerHGridValue(GridPoint, int);
    GridPoint destination;
};

#endif
