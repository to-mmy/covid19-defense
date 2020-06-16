#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

class Player
{
private:
	sf::RectangleShape player;
	sf::Texture texture;
	Animation animation;
public:
	Player(sf::Vector2f size);
	void move(sf::Vector2f dir);
	void updateAnimation(float);
	sf::RectangleShape& getPlayer();
	sf::Texture& getTexture();
	void setAnimation(Animation&);
	void setTexture();
	Animation& getAnimation();
	int getX();
	int getY();
	void draw(sf::RenderWindow &window);
	void setTextureRect();
};

void Player::setTexture()
{
	sf::Texture playerTexture;
	playerTexture.loadFromFile("sanitizer_bottle1.png");
	player.setTexture(&playerTexture);
	//player.setTexture(&t);
}

void Player::setAnimation(Animation& a)
{
	animation = a;
	//std::cout << a.getSwitchTime() << std::endl;
	//std::cout << animation.getSwitchTime() << std::endl;
}

void Player::updateAnimation(float deltaTime)
{
	animation.Update(0, deltaTime);
}

sf::RectangleShape& Player::getPlayer()
{
	return player;
}

Player::Player(sf::Vector2f size)
{
	player.setSize(size);

	sf::Texture playerTexture;
	playerTexture.loadFromFile("bar_soap1.png");
	//player.setTexture(&playerTexture);
	texture = playerTexture;


	//animation = a;
	//animation = a;
	//player.setFillColor(sf::Color::Blue);
}

void Player::move(sf::Vector2f dir)
{
	player.move(dir);
}

Animation& Player::getAnimation()
{
	return animation;
}

sf::Texture& Player::getTexture()
{
	return texture;
}

int Player::getX()
{
	return player.getPosition().x;
}

int Player::getY()
{
	return player.getPosition().y;
}

void Player::draw(sf::RenderWindow &window)
{
	window.draw(player);
}

void Player::setTextureRect()
{
	player.setTextureRect((animation).uvRect);
}


//pass in all of the paths around the tower (i think we should just try to display in the instructions where the tower's range is)
//read through the enemy queue and get the coordinates of the first one within the tower's range

//MemoryMoon_space-blaster      by suonho (FOR BOTTLE)
//whoosh04.wav      FreqMan (FOR SOAP)
//tone beep.wav      pan14 (FOR EITHER)
//w4.5 b2 3 submarine a.wav      ERH    (FOR SOAP)
//Transformers Type SFX 2       hykenfreak   (FOR SOAP)
//Ping!        unfa  (FOR SOAP)
//Open button 2      kickhat  (FOR EITHER)
//8-Bit Bounce     SomeGuy22 (For SOAP)
//Fire.aif       Gemmellness (FOR EITHER)

