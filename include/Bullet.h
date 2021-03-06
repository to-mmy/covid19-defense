#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

class Bullet {
private:
	sf::Sprite bullet;
	std::string type; //bubble or squirt
	double towerX;
	double towerY;
	double enemyX;
	double enemyY;
public:
	Bullet(std::string type, double towerXCoordinate, double towerYCoordinate,
           double enemyXCoordinate, double enemyYCoordinate);
	void setPos(sf::Vector2f);
	void fire();
	int getRight();
	int getLeft();
	int getTop();
	int getBottom();
	void draw(sf::RenderWindow &window);
	void rotate();
	sf::Sprite& getSprite();
	bool shouldShoot();
};

#endif // BULLET_H
