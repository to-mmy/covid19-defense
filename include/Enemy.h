#ifndef ENEMY_H
#define ENEMY_H

#include "Bullet.h"

class Enemy { //FOR PERSONAL TESTING PURPOSES
private:
	sf::RectangleShape enemy;
public:
	Enemy(sf::Vector2f size);
	void setPos(sf::Vector2f newPos);
	void checkCollision(Bullet bullet);
	void draw(sf::RenderWindow &window);
	int getX();
	int getY();
	sf::RectangleShape& getEnemy();
};

#endif // ENEMY_H
