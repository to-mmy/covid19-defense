#pragma once
#include <SFML\Graphics.hpp>
#include <cmath>
#include <iostream>

class Bullet
{
private:
	sf::Sprite bullet;
	std::string type; //bubble or squirt
	double towerX;
	double towerY;
	double enemyX;
	double enemyY;
public:
	Bullet(std::string type, double towerXCoordinate, double towerYCoordinate, double enemyXCoordinate, double enemyYCoordinate);
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

//sf::RectangleShape& Bullet::getBullet()
//{
//	return bullet;
//}
//
//Bullet::Bullet(sf::Vector2f size, double tX, double tY, double eX, double eY)
//{
//	bullet.setSize(size);
//	towerX = tX;
//	towerY = tY;
//	enemyX = eX + 25; //add half of the width of enemy
//	enemyY = eY + 25; //add half of the height of enemy
//	rotate();
//	setPos(sf::Vector2f(tX, tY));
//}
//
//void Bullet::rotate()
//{
//	float vert = enemyY - towerY;
//	float hor = enemyX - towerX;
//	float diag = sqrt((vert*vert) + (hor*hor));
//	float angle = (asin(vert / diag) * 180) / acos(-1);
//
//	if ((towerX > enemyX && towerY > enemyY) || (towerX > enemyX && towerY < enemyY))
//	{
//		bullet.rotate(180 - angle);
//	}
//	else bullet.rotate(angle);
//}
//
//void Bullet::setPos(sf::Vector2f newPos)
//{
//	bullet.setPosition(newPos);
//}
//
//void Bullet::fire()  
//{
//	double x = enemyX - towerX;
//	double y = enemyY - towerY;
//	double slope = y / x;
//	double slowMultiplier = 0;
//	/*if (abs(y) > 2 * abs(x))
//	{
//		slowMultiplier = abs(x / 30);
//	}*/
//	/*else */slowMultiplier = abs(x / 60);
//	//std::cout << slope << std::endl;
//
//	if (y < 0 && x > 0)
//	{
//		bullet.move(slowMultiplier * 1, slowMultiplier * slope);
//	}
//	else if (x < 0 && y > 0)
//	{
//		bullet.move(slowMultiplier * -1, slowMultiplier * abs(slope));
//	}
//	else if (x < 0 && y < 0)
//	{
//		bullet.move(slowMultiplier * -1, slowMultiplier * -(slope));
//	}
//	else
//	{
//		bullet.move(slowMultiplier * 1, slowMultiplier * slope);
//	}
//}
//
//int Bullet::getRight()
//{
//	return bullet.getPosition().x + bullet.getSize().x;
//}
//
//int Bullet::getLeft()
//{
//	return bullet.getPosition().x;
//}
//
//int Bullet::getTop()
//{
//	return bullet.getPosition().y;
//}
//
//int Bullet::getBottom()
//{
//	return bullet.getPosition().y + bullet.getSize().y;
//}
//
//void Bullet::draw(sf::RenderWindow &window)
//{
//	window.draw(bullet);
//}
