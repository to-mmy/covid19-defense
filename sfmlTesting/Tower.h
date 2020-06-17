#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Animation.h"

class Tower
{
private:
	sf::RectangleShape tower;
	std::string towerType; //soap or sanitizer
	bool shoot;
	//Enemy target;
	double bulletOffsetX;
	double bulletOffsetY;
public:
	Tower(sf::Vector2f size, sf::Vector2f coords, std::string type);
	void setPosition(sf::Vector2f dir);
	double getOffsetX();
	double getOffsetY();
	void setShoot(bool);
	bool getShoot();
	sf::RectangleShape& getRect();
	int getX();
	int getY();
	sf::Vector2f getPosition();
	std::string getType();
	void draw(sf::RenderWindow &window);
};

double Tower::getOffsetX()
{
	return bulletOffsetX;
}

double Tower::getOffsetY()
{
	return bulletOffsetY;
}

void Tower::setShoot(bool s)
{
	shoot = s;
}

bool Tower::getShoot()
{
	return shoot;
}

sf::RectangleShape& Tower::getRect()
{
	return tower;
}

Tower::Tower(sf::Vector2f size, sf::Vector2f coords, std::string type) //should receive coordinates too
{
	tower.setSize(size);
	tower.setPosition(coords);
	towerType = type;

	if (type == "sanitizer")
	{
		bulletOffsetX = tower.getSize().x / 3.8;
		bulletOffsetY = tower.getSize().y / 12;
	}
	else
	{
		bulletOffsetX = bulletOffsetY = tower.getSize().x / 2;
	}
}

void Tower::setPosition(sf::Vector2f dir)
{
	tower.setPosition(dir);
}

sf::Vector2f Tower::getPosition()
{
	return tower.getPosition();
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