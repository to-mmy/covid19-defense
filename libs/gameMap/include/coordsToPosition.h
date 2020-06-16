#ifndef COORDSTOPOSITION_H
#define COORDSTOPOSITION_H

#include <SFML/Graphics.hpp>

namespace game_map {
extern const float SIDE_FLT;
inline sf::Vector2f coordsToPosition(const sf::Vector2u& coords) {
    return sf::Vector2f(static_cast<float>(coords.y) * SIDE_FLT,
                        static_cast<float>(coords.x) * SIDE_FLT);
}
}

#endif // COORDSTOPOSITION_H
