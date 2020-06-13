#include "Cell.h"

void Cell::setCoords(const sf::Vector2u& indices) {
    coords = indices;
    position = sf::Vector2f(static_cast<float>(indices.y) * game_map::SIDE_FLT,
                            static_cast<float>(indices.x) * game_map::SIDE_FLT
                            );
}
