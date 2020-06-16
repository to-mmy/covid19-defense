#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Cell.h"
#include "coordsToPosition.h"
#include <SFML/Graphics.hpp>
#include <string>

// Declare constants for the map
namespace game_map {
extern const unsigned SIDE_PIX;
extern const float SIDE_FLT;
extern const sf::Vector2f SPRITE_SCALE;
extern const std::string RESOURCE_PATH;
}

class Cell; // forward declaration

class GameMap {
private:
    /*
    GameMap has 2-D array of Cell pointers cells[row][col]
    row starts at 0, top to bottom
    col starts at 0, left to right
    */
    Cell*** cells;
    unsigned sideLength;
    sf::Vector2u startCoords;
    sf::Vector2u exitCoords;
    sf::Vector2f drawOrigin;
    sf::Texture groundTexture;
    sf::Texture pathTexture;
public:
    // Constructor from file, default arg for drawOrigin is (0, 0)
    GameMap(const std::string& mapFileName, const sf::Vector2f& drOrigin = sf::Vector2f(0.f, 0.f));
    ~GameMap();

    // Setters
    void setExitCoords(sf::Vector2u c) { exitCoords = c; }
    void setStartCoords(sf::Vector2u c) { startCoords = c; }

    // Getters
    Cell*** getCells() { return cells; }
    unsigned getSideLength() const { return sideLength; }
    sf::Vector2u getStartCoords() const { return startCoords; }
    sf::Vector2u getExitCoords() const { return exitCoords; }
    sf::Vector2f getDrawOrigin() const { return drawOrigin; }

    // Draw to a RenderWindow.
    void draw(sf::RenderWindow& window) const;
};

#endif // GAMEMAP_H
