#ifndef PATH_H
#define PATH_H

#include "Cell.h"
#include "CellEnum.h"
#include <SFML/Graphics.hpp>

// Path is a Cell that also has the coordinates of the next Path
class Path : public Cell {
private:
    const CellEnum cellType;
    sf::Vector2u nextPathCoords;
public:
    Path(unsigned r, unsigned c, unsigned nR, unsigned nC)
        : Cell(r, c), cellType(CellEnum::PATH), nextPathCoords(nR, nC) {}
    Path(const sf::Vector2u& thisPathCoords, const sf::Vector2u& nextPathCoords)
        : Cell(thisPathCoords), cellType(CellEnum::PATH), nextPathCoords(nextPathCoords) {}

    // Setters
    void setNext(unsigned r, unsigned c) { nextPathCoords = sf::Vector2u(r, c); }
    void setNext(sf::Vector2u n) { nextPathCoords = n; }

    // Getters
    sf::Vector2u getNextCoords() const { return nextPathCoords; }
    virtual CellEnum getCellType() const override { return cellType; }
};

#endif // PATH_H
