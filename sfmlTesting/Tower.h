#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Animation.h"

class Tower
{
private:
	sf::RectangleShape tower;
	std::string towerType; //soap or sanitizer
public:
	Tower(sf::Vector2f size, std::string type);
	void move(sf::Vector2f dir);
	sf::RectangleShape& getRect();
	int getX();
	int getY();
	std::string getType();
	void draw(sf::RenderWindow &window);
};

sf::RectangleShape& Tower::getRect()
{
	return tower;
}

Tower::Tower(sf::Vector2f size, std::string type)
{
	tower.setSize(size);
	towerType = type;
}

void Tower::move(sf::Vector2f dir)
{
	tower.move(dir);
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