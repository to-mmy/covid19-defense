#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Animation.h"
#include "Ground.h"

class Tower
{
private:
	sf::RectangleShape tower;
	std::string towerType; //soap or sanitizer
	bool haveTarget;
	//Enemy target;
	double bulletOffsetX; // offset makes it look like the squirt is coming from the bottle nozzle
	                      // and the bubbles are coming from inside the soap
	double bulletOffsetY;
	bool placedDown;
	Cell* groundLocation;
	int shootingGap = 0;
public:
	Tower(sf::Vector2f size, sf::Vector2f coords, std::string type, Cell* location);
	Cell*& getGroundLocation();
	void setPosition(sf::Vector2f dir);
	void setPlacedDown(bool);
	//void setShoot(bool);
	void setHaveTarget(bool);
	void incrementShootingGap();
	bool getHaveTarget();
	bool getPlacedDown();
	double getOffsetX();
	double getOffsetY();
	bool shouldShoot();
	sf::RectangleShape& getRect();
	int getX();
	int getY();
	sf::Vector2f getPosition();
	std::string getType();
	void draw(sf::RenderWindow &window);
};
