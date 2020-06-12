#ifndef MAP_H
#define MAP_H

#include "Cell.h"
#include "Coordinates.h"

class Map {
private:
    /*
    Map has 2-D array of Cell pointers cells[row][col]
    row starts at 0, top to bottom
    col starts at 0, left to right
    */
    Cell*** cells;
    unsigned side_length;
    Coordinates start_coords;
    Coordinates exit_coords;
public:
    // Constructor, Destructor
    Map(unsigned size = 8);
    ~Map();

    // Setters
    void setExitCoords(Coordinates c) { exit_coords = c; }
    void setStartCoords(Coordinates c) { start_coords = c; }

    // Getters
    Cell*** getCells() { return cells; }
    unsigned getSideLength() const { return side_length; }
    Coordinates getStartCoords() const { return start_coords; }
    Coordinates getExitCoords() const { return exit_coords; };
};

#endif // MAP_H
