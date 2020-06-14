#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Animation.h"
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include <vector>

using namespace std;


int main()
{
	bool waveGoing = 0; //0 if the wave has finished/hasn't started yet, 1 if the wave of enemies is still going
	bool waveNumber = 0; //0 through 5
	int bulletGeneration = 0; //used to create gaps between the generation of each bullet

	//opening a window
	sf::RenderWindow window;
	sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2) - 755, (sf::VideoMode::getDesktopMode().height / 2) - 390);
	window.create(sf::VideoMode(1500, 700), "SFML Testing", sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(centerWindow);
	window.setKeyRepeatEnabled(true);


	//Defining objects
	Tower tower(sf::Vector2f(150, 150), "soap");
	Enemy enemy(sf::Vector2f(50, 50));
	std::vector<Bullet> bulletVec;
	std::vector<Tower> towerVec;
	std::vector<Tower> shootingTowers;
	bool isFiring = false;

	//set position of objects for testing
	enemy.setPos(sf::Vector2f(500, 200));
	tower.move(sf::Vector2f(100, 50));

	//loading textures for animated towers     //textures and animations have to be loaded outside of the main while loop
	sf::Texture soapTexture; 
	soapTexture.loadFromFile("bar_soap_transparent.png");    //bar_soap_transparent or sanitizer_bottle_transparent
	sf::Texture sanitizerTexture;
	sanitizerTexture.loadFromFile("sanitizer_bottle_transparent.png");

	//loading textures for tower selection
	sf::Texture soapSelectTexture;
	soapSelectTexture.loadFromFile("bar_soap_selection.png");
	sf::Texture sanitizerSelectTexture;
	sanitizerSelectTexture.loadFromFile("sanitizer_bottle_selection.png");

	//creating tower selection pictures
	sf::RectangleShape soapSelection(sf::Vector2f(100, 100));
	soapSelection.setTexture(&soapSelectTexture);
	sf::RectangleShape sanitizerSelection(sf::Vector2f(100, 100));
	sanitizerSelection.setTexture(&sanitizerSelectTexture);

	//loading textures for bullets
	sf::Texture squirtTexture;
	squirtTexture.loadFromFile("sanitizer_squirt_rect.png"); //sanitizer_squirt_rect (maybe sanitizer_squirt_transparent) or soap_bubbles_transparent
	sf::Texture bubbleTexture;
	bubbleTexture.loadFromFile("soap_bubbles_transparent.png");
	
	//loading sounds
	sf::SoundBuffer squirtBuffer;
	squirtBuffer.loadFromFile("squirt_sound.wav");
	sf::Sound squirtSound;
	squirtSound.setBuffer(squirtBuffer);

	//animation
	Animation soapAnimation(&soapTexture, sf::Vector2u(3, 1), 0.3f);
	Animation sanitizerAnimation(&sanitizerTexture, sf::Vector2u(3, 1), 0.3f);
	//player.setAnimation(soapAnimation);
	float deltaTime = 0.0f;
	sf::Clock clock;

	//coordinates for mouse click
	int mouseX = 0;
	int mouseY = 0;

	//main loop
	while (window.isOpen())
	{
		tower.getRect().setTexture(&soapTexture);

		//deltaTime used to make animation consistent for different OS's
		deltaTime = clock.restart().asSeconds();

		sf::Event Event;

		while (window.pollEvent(Event))
		{
			switch (Event.type)
			{
			case sf::Event::Closed:
				window.close();
			}

			int moveSpeed = 6;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				tower.move(sf::Vector2f(0, -moveSpeed));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				tower.move(sf::Vector2f(-moveSpeed, 0));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				tower.move(sf::Vector2f(0, moveSpeed));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				tower.move(sf::Vector2f(moveSpeed, 0));
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				mouseX = sf::Mouse::getPosition(window).x;
				mouseY = sf::Mouse::getPosition(window).y;
				//std::cout << mouseX << "   " << mouseY << endl;
			}

		}

		Tower player1(sf::Vector2f(150, 150), "sanitizer");
		player1.move(sf::Vector2f(100, 300));
		player1.getRect().setTexture(&sanitizerTexture);

		//create gap between bullets
		if (enemy.getX() > tower.getX() && bulletGeneration % 200 == 0)
		{
			isFiring = true;
		}
		bulletGeneration++;

		//update animations
		soapAnimation.Update(0, deltaTime);
		sanitizerAnimation.Update(0, deltaTime);

		//(for animation) set texture rectangle for all towers
		tower.getRect().setTextureRect(soapAnimation.uvRect); //step through the array/vector and use the same element # for the corresponding player and animation objects
		player1.getRect().setTextureRect(sanitizerAnimation.uvRect);

		window.clear();		
		
		//squirtSound.setLoop(false);
		
		//make sure you only play the sound once (turn a bool variable on if a type is shooting and play the sound only once at the end of the for loop)
		//generate bullets (squirt dimensions: 17x4, bubble dimensions: 75x33)
		if (isFiring == true)
		{
			Bullet newBullet(sf::Vector2f(75, 33), tower.getX(), tower.getY(), enemy.getX(), enemy.getY()); //shoot at first enemy, pass in tower for starting position
			newBullet.getBullet().setTexture(&bubbleTexture);
			bulletVec.push_back(newBullet);
			isFiring = false;
			squirtSound.play();
			//squirtSound.stop();
		}

		
		//draw bullets to window
		for (int i = 0; i < bulletVec.size(); i++)
		{
			bulletVec[i].draw(window);
			bulletVec[i].fire();
		}


		//draw all objects to window
		tower.draw(window);
		player1.draw(window);
		enemy.draw(window);
		window.draw(sanitizerSelection);
		window.draw(soapSelection);
		window.display();

	}
}


//maybe for the different waves, once the enemy queue reaches 0, we enter a while loop and until the player presses start, after the player presses start
//we load the enemies again and continue (keep track of the waves)



//ask the group how to make the firing of the bullets more consistent