#ifndef GROUND_HPP
#define GROUND_HPP
#include "Cell.hpp"
#include "Coordinates.hpp"
#include "CellEnum.hpp"

class Ground : public Cell {
private:
    const CellEnum cellType;
    bool hasTower;
public:
    Ground(unsigned r, unsigned c) : Cell(r, c), cellType(CellEnum::GROUND), hasTower(false) {}

    virtual CellEnum getCellType() const override { return cellType; }

};

#endif // GROUND_HPP
