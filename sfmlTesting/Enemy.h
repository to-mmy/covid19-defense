#pragma once
#include "Bullet.h"

class Enemy //FOR PERSONAL TESTING PURPOSES
{
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

sf::RectangleShape& Enemy::getEnemy()
{
	return enemy;
}

int Enemy::getX()
{
	return enemy.getPosition().x;
}

int Enemy::getY()
{
	return enemy.getPosition().y;
}

Enemy::Enemy(sf::Vector2f size)
{
	enemy.setSize(size);
	enemy.setFillColor(sf::Color::Red);
}

void Enemy::setPos(sf::Vector2f newPos)
{
	enemy.setPosition(newPos);
}

void Enemy::checkCollision(Bullet bullet)
{
	if (bullet.getRight() > enemy.getPosition().x && bullet.getTop() < enemy.getPosition().y + enemy.getSize().y && bullet.getBottom() > enemy.getPosition().y)
	{
		enemy.setPosition(sf::Vector2f(4234432, 4233423));
	}
}

void Enemy::draw(sf::RenderWindow &window)
{
	window.draw(enemy);
}