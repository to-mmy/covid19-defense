#include "Path.h"

void Path::setNext(unsigned r, unsigned c) {
    nextPathCoords = sf::Vector2u(r, c);
    nextPathPosition = game_map::coordsToPosition(nextPathCoords);
}

void Path::setNext(const sf::Vector2u& n) {
    nextPathCoords = n;
    nextPathPosition = game_map::coordsToPosition(nextPathCoords);
}
