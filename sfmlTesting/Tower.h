#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Animation.h"

class Tower
{
private:
	sf::RectangleShape tower;
	std::string towerType; //soap or sanitizer
<<<<<<< HEAD
public:
	Tower(sf::Vector2f size, std::string type);
	void move(sf::Vector2f dir);
	sf::RectangleShape& getRect();
	int getX();
	int getY();
=======
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
>>>>>>> tower-v2
	std::string getType();
	void draw(sf::RenderWindow &window);
};

<<<<<<< HEAD
=======
void Tower::setShoot(bool s)
{
	shoot = s;
}

bool Tower::getShoot()
{
	return shoot;
}

>>>>>>> tower-v2
sf::RectangleShape& Tower::getRect()
{
	return tower;
}

<<<<<<< HEAD
Tower::Tower(sf::Vector2f size, std::string type)
{
	tower.setSize(size);
=======
Tower::Tower(sf::Vector2f size, sf::Vector2f coords, std::string type) //should receive coordinates too
{
	tower.setSize(size);
	tower.setPosition(coords);
>>>>>>> tower-v2
	towerType = type;
}

void Tower::move(sf::Vector2f dir)
{
<<<<<<< HEAD
	tower.move(dir);
=======
	tower.setPosition(dir);
}

sf::Vector2f Tower::getPosition()
{
	return tower.getPosition();
>>>>>>> tower-v2
}

int Tower::getX()
{
	return tower.getPosition().x;
}

int Tower::getY()
{
	return tower.getPosition().y;
}

std::string Tower::getType()
{
	return towerType;
}

void Tower::draw(sf::RenderWindow &window)
{
	window.draw(tower);
}


//pass in all of the paths around the tower (i think we should just try to display in the instructions where the tower's range is)
//read through the enemy queue and get the coordinates of the first one within the tower's range