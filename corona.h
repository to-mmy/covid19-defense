#ifndef CORONA_H
#define CORONA_H

#include "GameMap.h"
#include "Cell.h"
#include "CellEnum.h"
#include "Path.h"
#include "Ground.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

<<<<<<< HEAD
# include "enemyBase.h"

class corona : public enemyBase{
=======
# include "enemyBase.h"`

class Corona : public enemyBase{
>>>>>>> 7a83a56228ec532cef54eb81c711c0ce01ea075e
    float movespeed;
    bool drawCovid;

    Path* pathPtr;

    sf::Vector2f covidDirection;
    sf::Vector2f covidDestination;
    sf::Vector2f distanceToDestination;
<<<<<<< HEAD
    sf::Vector2f covidCoords;

public:
    corona(){}
    corona(sf::Texture textureIn, GameMap gameMap);
    ~corona(){}

    // getter/setters
    void setMoveSpeed(const float moveSpeedIn){movespeed = moveSpeedIn;}
    float getMoveSpeed(){return movespeed
=======
    sf::Vector2u covidCoords;

public:
    Corona(){}
    Corona(sf::Texture textureIn, GameMap gameMap, sf::Vector2f GAME_MAP_ORIGIN);
    ~Corona(){}

    // getter/setters
    void setMoveSpeed(const float moveSpeedIn){movespeed = moveSpeedIn;}
    float getMoveSpeed(){return movespeed;}
>>>>>>> 7a83a56228ec532cef54eb81c711c0ce01ea075e

    void setDrawCovid(bool drawCovidIn){drawCovid = drawCovidIn;}
    bool getDrawCovid(){return drawCovid;}

    void setPathPtr(Path* pathIn){pathPtr = pathIn;}
    Path* getPathPtr(){return pathPtr;}

    void setCovidDirection(sf::Vector2f directionIn){covidDirection = directionIn;}
    sf::Vector2f getCovidDirection(){return covidDirection;}

    void setCovidDestination(sf::Vector2f destinationIn){covidDestination = destinationIn;}
    sf::Vector2f getCovidDestination(){return covidDestination;}

    void setDistanceToDestination(sf::Vector2f distanceIn){distanceToDestination = distanceIn;}
    sf::Vector2f getDistanceToDestination(){return distanceToDestination;}

<<<<<<< HEAD
    void setCovidCoords(sf::Vector coordsIn){covidCoords = coordsIn;}
    sf::Vector2f getCovidCoords(){return covidCoords;}
};

corona::corona(sf::Texture textureIn, GameMap gameMap) : movespeed (0.05f), enemyBase() {
=======
    void setCovidCoords(sf::Vector2u coordsIn){covidCoords = coordsIn;}
    sf::Vector2u getCovidCoords(){return covidCoords;}
};

Corona::Corona(sf::Texture textureIn, GameMap gameMap, sf::Vector2f GAME_MAP_ORIGIN) : movespeed (0.05f), enemyBase() {
>>>>>>> 7a83a56228ec532cef54eb81c711c0ce01ea075e
    sprite.setTexture(textureIn);

/**/

<<<<<<< HEAD
    sprite.setTexture(covidTexture);
    covidCoords = gameMap.getStartCoords();
    sprite.setPosition(getCellPositionFromCoordinates(covidCoords, game_map::SIDE_FLT));
=======
    sprite.setTexture(textureIn);

    sf::Vector2u newCovidCoords(gameMap.getStartCoords());
    covidCoords = newCovidCoords;

    sprite.setPosition(GAME_MAP_ORIGIN + pathPtr->getPosition());
>>>>>>> 7a83a56228ec532cef54eb81c711c0ce01ea075e

    pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);


<<<<<<< HEAD
    covidDestination = getCellPositionFromCoordinates(pathPtr->getNextCoords(),
                                                                   game_map::SIDE_FLT);
    distanceToDestination = sf::Vector2f(std::abs(covidDestination.x - covid.getPosition().x),
                                       std::abs(covidDestination.y - covid.getPosition().y));
=======
    //covidDestination = getCellPositionFromCoordinates(pathPtr->getNextCoords(),
    //                                                               game_map::SIDE_FLT);
    distanceToDestination = sf::Vector2f(std::abs(covidDestination.x - sprite.getPosition().x),
                                       std::abs(covidDestination.y - sprite.getPosition().y));
>>>>>>> 7a83a56228ec532cef54eb81c711c0ce01ea075e

    drawCovid = true;


}

#endif
