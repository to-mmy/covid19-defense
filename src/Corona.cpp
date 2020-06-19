#include "corona.h"
#include "GamePlayScreen.h"

Corona::Corona(sf::Texture* textureIn, GameMap* gameMap, Path*& pathPtr) : EnemyBase(),
    movespeed (0.05f) {

    sprite.setTexture(*textureIn);

    sf::Vector2u newCovidCoords(gameMap->getStartCoords());
    covidCoords = newCovidCoords;

    sprite.setPosition(draw::GAME_MAP_ORIGIN + pathPtr->getPosition());

    sprite.setScale(game_map::SPRITE_SCALE);

    covidDestination = draw::GAME_MAP_ORIGIN + pathPtr->getNextPosition();

    distanceToDestination = sf::Vector2f(std::abs(covidDestination.x - sprite.getPosition().x),
                                         std::abs(covidDestination.y - sprite.getPosition().y));

    drawCovid = true;


}
