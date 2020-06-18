#include "Tower.h"

void Tower::incrementShootingGap()
{
	shootingGap++;
}

void Tower::setHaveTarget(bool b)
{
	haveTarget = b;
}

bool Tower::getHaveTarget()
{
	return haveTarget;
}

Cell*& Tower::getGroundLocation()
{
	return groundLocation;
}

void Tower::setPlacedDown(bool p)
{
	placedDown = p;
}

bool Tower::getPlacedDown()
{
	return placedDown;
}

double Tower::getOffsetX()
{
	return bulletOffsetX;
}

double Tower::getOffsetY()
{
	return bulletOffsetY;
}

//void Tower::setShoot(bool s)
//{
//	shoot = s;
//}

bool Tower::shouldShoot() //this effectively creates gaps in time between each bullet a tower shoots 
{
	if (towerType == "sanitizer" && shootingGap % 500 == 0)
	{
		//shootingGap = 0;
		return true;
	}
	else if (towerType == "soap" && shootingGap % 800 == 0) //soap doesn't shoot as often
	{
		//shootingGap = 0;
		return true;
	}
	else return false;
}

sf::RectangleShape& Tower::getRect()
{
	return tower;
}

Tower::Tower(sf::Vector2f size, sf::Vector2f coords, std::string type, Cell* ground) //should receive coordinates too
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

	groundLocation = ground;
	shootingGap = 0;
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