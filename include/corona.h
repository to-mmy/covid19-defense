#ifndef CORONA_H
#define CORONA_H

#include "enemyBase.h"
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

class Corona : public EnemyBase {
private:
    float movespeed;
    bool drawCovid;

    //Path* pathPtr;

    sf::Vector2f covidDirection;
    sf::Vector2f covidDestination;
    sf::Vector2f distanceToDestination;
    sf::Vector2u covidCoords;

public:
    Corona() {}
    Corona(sf::Texture* textureIn, GameMap* gameMap, Path*& pathPtr);
    ~Corona() {}

    // getter/setters
    void setMoveSpeed(const float moveSpeedIn) { movespeed = moveSpeedIn; }
    float getMoveSpeed() { return movespeed; }

    void setDrawCovid(bool drawCovidIn) { drawCovid = drawCovidIn; }
    bool getDrawCovid() { return drawCovid; }

    void setCovidDirection(sf::Vector2f directionIn) { covidDirection = directionIn; }
    sf::Vector2f getCovidDirection() { return covidDirection; }

    void setCovidDestination(sf::Vector2f destinationIn) { covidDestination = destinationIn; }
    sf::Vector2f getCovidDestination() { return covidDestination; }

    void setDistanceToDestination(sf::Vector2f distanceIn) { distanceToDestination = distanceIn; }
    sf::Vector2f getDistanceToDestination() { return distanceToDestination; }

    void setCovidCoords(sf::Vector2u coordsIn) { covidCoords = coordsIn; }
    sf::Vector2u getCovidCoords() { return covidCoords; }
};

#endif
