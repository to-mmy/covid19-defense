#ifndef TOWER_H
#define TOWER_H

#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Tower {
private:
    sf::RectangleShape tower;
    std::string towerType; //soap or sanitizer
    bool shoot;
public:
    Tower(sf::Vector2f size, sf::Vector2f coords, std::string type);
    void move(sf::Vector2f dir);
    void setShoot(bool);
    bool getShoot();
    sf::RectangleShape& getRect();
    int getX();
    int getY();
    sf::Vector2f getPosition();
    std::string getType();
    void draw(sf::RenderWindow& window);
};

#endif // TOWER_H
