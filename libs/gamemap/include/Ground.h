#ifndef GROUND_H
#define GROUND_H

#include "Cell.h"
#include "Coordinates.h"
#include "CellEnum.h"

// Ground is a Cell that can hold a tower
// getCoords and setCoords are defined in parent class Cell.h
class Ground : public Cell {
private:
    const CellEnum cellType;
    bool hasTower;
public:
    Ground(unsigned r, unsigned c) : Cell(r, c), cellType(CellEnum::GROUND), hasTower(false) {}

    // Setter
    void setHasTower(bool h) { hasTower = h; }

    // Getters
    bool getHasTower() const { return hasTower; }
    virtual CellEnum getCellType() const override { return cellType; }

};

#endif // GROUND_H
