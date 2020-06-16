#ifndef CELL_H
#define CELL_H

#include "CellEnum.h"
#include "GameMap.h"
#include "coordsToPosition.h"
#include <SFML/Graphics.hpp>

namespace game_map { // forward declaration
extern const unsigned SIDE_PIX; // defined in GameMap.cpp
extern const float SIDE_FLT;// defined in GameMap.cpp
}

class Cell {
protected:
    sf::Vector2u coords;
    /*
    Map has 2-D array of Cells cells[row][col]
    row starts at 0, top to bottom
    col starts at 0, left to right
    */
    sf::Sprite cellSprite;
    sf::Vector2f position; // position relative to the map origin
    /*
    While 2-D arrays are by convention (row, col), i.e. (y, x), and coords uses that order,
    SFML positions are (x, y). Transformation from coord space to position space is thus defined
    as multiplying each dimension by the length of a side in pixels and swapping the order of
    the coordinates.
    */
public:
    Cell() = default;
    // r = row = y coordinate, c = col = x coordinate
    Cell(unsigned r, unsigned c) : coords(r,c),
                                   position(game_map::coordsToPosition(coords)) {}
    Cell(const sf::Vector2u& v) : coords(v), position(game_map::coordsToPosition(v)) {}

    virtual ~Cell() = default;

    // Setters, non-overrideable
    virtual void setCoords(const sf::Vector2u& indices) final;
    // Sprite texture
    virtual void setSpriteTexture(const sf::Texture& texture) final {
        cellSprite.setTexture(texture);
    }
    // Sprite position
    virtual void setSpritePosition(const sf::Vector2f& pos) final;
    // Sprite scale
    virtual void setSpriteScale(const sf::Vector2f& scale) final {
        cellSprite.setScale(scale);
    }

    // Getters, non-overrideable
    virtual sf::Vector2u getCoords() const final { return coords; }
    virtual sf::Vector2f getPosition() const final { return position; }
    virtual sf::Sprite getSprite() const final { return cellSprite; }

    // pure virtual getter
    virtual CellEnum getCellType() const = 0;
};

#endif // CELL_H
