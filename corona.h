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

# include "enemyBase.h"

class corona : public enemyBase{
    float movespeed;
    bool drawCovid;

    Path* pathPtr;

    sf::Vector2f covidDirection;
    sf::Vector2f covidDestination;
    sf::Vector2f distanceToDestination;
    sf::Vector2f covidCoords;

public:
    corona(){}
    corona(sf::Texture textureIn, GameMap gameMap);
    ~corona(){}

    // getter/setters
    void setMoveSpeed(const float moveSpeedIn){movespeed = moveSpeedIn;}
    float getMoveSpeed(){return movespeed

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

    void setCovidCoords(sf::Vector coordsIn){covidCoords = coordsIn;}
    sf::Vector2f getCovidCoords(){return covidCoords;}
};

corona::corona(sf::Texture textureIn, GameMap gameMap) : movespeed (0.05f), enemyBase() {
    sprite.setTexture(textureIn);

/**/

    sprite.setTexture(covidTexture);
    covidCoords = gameMap.getStartCoords();
    sprite.setPosition(getCellPositionFromCoordinates(covidCoords, game_map::SIDE_FLT));

    pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);


    covidDestination = getCellPositionFromCoordinates(pathPtr->getNextCoords(),
                                                                   game_map::SIDE_FLT);
    distanceToDestination = sf::Vector2f(std::abs(covidDestination.x - covid.getPosition().x),
                                       std::abs(covidDestination.y - covid.getPosition().y));

    drawCovid = true;


}

#endif
