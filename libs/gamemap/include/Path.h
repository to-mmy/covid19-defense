#ifndef PATH_H
#define PATH_H

#include "Cell.h"
#include "CellEnum.h"
#include "coordsToPosition.h"
#include <SFML/Graphics.hpp>

// Path is a Cell that also has the coordinates of the next Path
class Path : public Cell {
private:
    const CellEnum cellType;
    sf::Vector2u nextPathCoords;
    sf::Vector2f nextPathPosition;
public:
    Path(unsigned r, unsigned c, unsigned nR, unsigned nC)
        : Cell(r, c), cellType(CellEnum::PATH), nextPathCoords(nR, nC),
          nextPathPosition(game_map::coordsToPosition(nextPathCoords)) {}
    Path(const sf::Vector2u& thisPathCoords, const sf::Vector2u& nextPathCoords)
        : Cell(thisPathCoords), cellType(CellEnum::PATH), nextPathCoords(nextPathCoords),
          nextPathPosition(game_map::coordsToPosition(nextPathCoords)) {}

    // Setters
    void setNext(unsigned r, unsigned c);
    void setNext(const sf::Vector2u& n);

    // Getters
    sf::Vector2u getNextCoords() const { return nextPathCoords; }
    sf::Vector2f getNextPosition() const { return nextPathPosition; }
    virtual CellEnum getCellType() const override { return cellType; }
};

#endif // PATH_H
