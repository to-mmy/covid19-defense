#ifndef CELL_HPP
#define CELL_HPP
#include "Coordinates.hpp"
#include "CellEnum.hpp"

class Cell {
protected:
    Coordinates coords;
    /*
    Map has 2-D array of Cells cells[row][col]
    row starts at 0, top to bottom
    col starts at 0, left to right
    */
public:
    Cell() = default;
    Cell(unsigned r, unsigned c) : coords(r,c) {}
    Cell(Coordinates c) : coords(c) {}

    virtual ~Cell() = default;

    void setCoords(unsigned r, unsigned c) { coords.row = r; coords.col = c; }

    virtual CellEnum getCellType() const = 0;
};

#endif // CELL_HPP
