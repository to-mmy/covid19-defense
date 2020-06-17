#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include <SFML/Graphics.hpp>

class enemyBase{
protected:
    sf::Sprite sprite;
public:
    enemyBase(){}
    ~enemyBase(){}
    void setSprite(sf::Sprite spriteIn){sprite = spriteIn;}
    sf::Sprite getSprite(){return sprite;}

};

#endif
