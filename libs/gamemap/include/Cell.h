#ifndef CELL_H
#define CELL_H

#include "Coordinates.h"
#include "CellEnum.h"

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

    // Setter, non-overrideable
    virtual void setCoords(unsigned r, unsigned c) final { coords.row = r; coords.col = c; }

    // Getter, non-overrideable
    virtual Coordinates getCoords() const final { return coords; }

    // pure virtual getter
    virtual CellEnum getCellType() const = 0;
};

#endif // CELL_H
