#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include <SFML/Graphics.hpp>

class enemyBase{
protected:
    sf::Sprite sprite;
    int health;
    bool isAlive;
public:
    enemyBase();
    ~enemyBase(){}

    void setSprite(sf::Sprite spriteIn){sprite = spriteIn;}
    sf::Sprite getSprite(){return sprite;}

    void setHealth(int healthIn){health = healthIn;}
    int getHealth(){return health;}
    void lowerHealth();

    void setIsAlive(bool aliveIn){isAlive = aliveIn;}
    bool getIsAlive(){return isAlive;}
};

enemyBase::enemyBase() : health(15), isAlive(true) {}

void enemyBase::lowerHealth(){
    if (health < 0) health--;
    else isAlive = false;
}

#endif
