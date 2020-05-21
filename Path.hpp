#ifndef PATH_HPP
#define PATH_HPP
#include "Cell.hpp"
#include "Coordinates.hpp"
#include "CellEnum.hpp"
#include <iostream>

class Path : public Cell {
private:
    const CellEnum cellType;
    Coordinates next_path;
public:
    Path(unsigned r, unsigned c, unsigned n_r, unsigned n_c) : Cell(r, c), cellType(CellEnum::PATH), next_path(n_r, n_c) {}
    Path(Coordinates this_path_coords, Coordinates next_path_coords) : Cell(this_path_coords), cellType(CellEnum::PATH), next_path(next_path_coords) {}

    void setNext(unsigned r, unsigned c) { next_path.row = r; next_path.col = c; }
    void setNext(Coordinates n) { next_path = n; }

    Coordinates getNextCoords() const { return next_path; }

    virtual CellEnum getCellType() const override { return cellType; }
};

#endif // PATH_HPP
