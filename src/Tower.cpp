#include "Tower.h"

void Tower::setShoot(bool s) {
    shoot = s;
}

bool Tower::getShoot() {
    return shoot;
}

sf::RectangleShape& Tower::getRect() {
    return tower;
}


Tower::Tower(sf::Vector2f size, sf::Vector2f coords,
             std::string type) { //should receive coordinates too
    tower.setSize(size);
    tower.setPosition(coords);
    towerType = type;
}

void Tower::move(sf::Vector2f dir) {
    tower.setPosition(dir);
}

sf::Vector2f Tower::getPosition() {
    return tower.getPosition();
}

int Tower::getX() {
    return tower.getPosition().x;
}

int Tower::getY() {
    return tower.getPosition().y;
}

std::string Tower::getType() {
    return towerType;
}

void Tower::draw(sf::RenderWindow& window) {
    window.draw(tower);
}


//pass in all of the paths around the tower (i think we should just try to display in the instructions where the tower's range is)
//read through the enemy queue and get the coordinates of the first one within the tower's range
