#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include <SFML/Graphics.hpp>

class EnemyBase {
protected:
    sf::Sprite sprite;
    int health;
    bool isAlive;
public:
    EnemyBase() : health(8), isAlive(true) {}
    virtual ~EnemyBase() = default;
    virtual void setSprite(sf::Sprite spriteIn) { sprite = spriteIn; }
    virtual sf::Sprite& getSprite() { return sprite; }

    virtual void setHealth(int healthIn) { health = healthIn; }
    virtual int getHealth() { return health; }
    virtual void lowerHealth();

    virtual void setIsAlive(bool aliveIn) { isAlive = aliveIn; }
    virtual bool getIsAlive() { return isAlive; }

};

#endif
