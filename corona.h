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

# include "enemyBase.h"`

class Corona : public enemyBase{
    float movespeed;
    bool drawCovid;

    //Path* pathPtr;

    sf::Vector2f covidDirection;
    sf::Vector2f covidDestination;
    sf::Vector2f distanceToDestination;
    sf::Vector2u covidCoords;

public:
    Corona(){}
    Corona(sf::Texture textureIn, GameMap *gameMap, sf::Vector2f GAME_MAP_ORIGIN, Path* pathPtr);
    ~Corona(){}

    // getter/setters
    void setMoveSpeed(const float moveSpeedIn){movespeed = moveSpeedIn;}
    float getMoveSpeed(){return movespeed;}

    void setDrawCovid(bool drawCovidIn){drawCovid = drawCovidIn;}
    bool getDrawCovid(){return drawCovid;}

    //bypass
    //void setPathPtr(Path* pathIn){pathPtr = pathIn;}
    //Path* getPathPtr(){return pathPtr;}

    void setCovidDirection(sf::Vector2f directionIn){covidDirection = directionIn;}
    sf::Vector2f getCovidDirection(){return covidDirection;}

    void setCovidDestination(sf::Vector2f destinationIn){covidDestination = destinationIn;}
    sf::Vector2f getCovidDestination(){return covidDestination;}

    void setDistanceToDestination(sf::Vector2f distanceIn){distanceToDestination = distanceIn;}
    sf::Vector2f getDistanceToDestination(){return distanceToDestination;}

    void setCovidCoords(sf::Vector2u coordsIn){covidCoords = coordsIn;}
    sf::Vector2u getCovidCoords(){return covidCoords;}
};

Corona::Corona(sf::Texture textureIn, GameMap *gameMap, sf::Vector2f GAME_MAP_ORIGIN, Path* pathPtr) : movespeed (0.05f), enemyBase() {
    sprite.setTexture(textureIn);

/**/

    sprite.setTexture(textureIn);

    sf::Vector2u newCovidCoords(gameMap->getStartCoords());
    covidCoords = newCovidCoords;

    sprite.setPosition(GAME_MAP_ORIGIN + pathPtr->getPosition());

    pathPtr = dynamic_cast<Path*>(gameMap->getCells()[covidCoords.x][covidCoords.y]);


    //covidDestination = getCellPositionFromCoordinates(pathPtr->getNextCoords(),
    //                                                               game_map::SIDE_FLT);
    distanceToDestination = sf::Vector2f(std::abs(covidDestination.x - sprite.getPosition().x),
                                       std::abs(covidDestination.y - sprite.getPosition().y));

    drawCovid = true;


}

#endif
