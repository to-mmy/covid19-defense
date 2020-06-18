#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

class Bullet {
private:
	sf::RectangleShape bullet;
	double towerX;
	double towerY;
	double enemyX;
	double enemyY;
public:
	Bullet(sf::Vector2f size, double towerX, double towerY, double enemyX, double enemyY);
	void setPos(sf::Vector2f);
	void fire();
	int getRight();
	int getLeft();
	int getTop();
	int getBottom();
	void draw(sf::RenderWindow &window);
	void rotate();
	sf::RectangleShape& getBullet();
	bool shouldShoot();
};

#endif // BULLET_H
