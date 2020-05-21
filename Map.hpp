#ifndef MAP_HPP
#define MAP_HPP
#include "Cell.hpp"
#include "Coordinates.hpp"

class Map {
private:
    Cell*** cells;
    unsigned side_length;
    Coordinates start_coords;
    Coordinates exit_coords;
public:
    Map(unsigned size = 8);
    ~Map();

    void setExitCoords(Coordinates c) { exit_coords = c; }
    void setStartCoords(Coordinates c) { start_coords = c; }

    Cell*** getCells() { return cells; }
    unsigned getSideLength() const { return side_length; }
    Coordinates getStartCoords() const { return start_coords; }
    Coordinates getExitCoords() const { return exit_coords; };
};


#endif // MAP_HPP
