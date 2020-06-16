#include "Cell.h"

void Cell::setCoords(const sf::Vector2u& indices) {
    coords = indices;
    position = sf::Vector2f(static_cast<float>(indices.y) * game_map::SIDE_FLT,
                            static_cast<float>(indices.x) * game_map::SIDE_FLT
                            );
}

void Cell::setSpritePosition(const sf::Vector2f& pos) {
        position = pos;
        cellSprite.setPosition(pos);
    }
