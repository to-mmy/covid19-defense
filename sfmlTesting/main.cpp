#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Animation.h"
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include <vector>
#include "GameMap.h"
#include "Cell.h"
#include "CellEnum.h"
#include "Path.h"
#include "Ground.h"
#include <fstream>
#include <string>
#include <cmath>

using namespace std;


sf::Vector2f getCellPositionFromCoordinates(const sf::Vector2u& coords, const float& sideFlt);
sf::Vector2f normalize(const sf::Vector2f& vec);
sf::Vector2f checkTowerPlacement(int, int, GameMap&, std::vector<Tower>&);


int main()
{
	//MAP MAIN STUFF**********************************************************
	GameMap gameMap(std::string("map_1_data"));

	// Reduce function calls
	unsigned sideLen = gameMap.getSideLength();
	// The following creates a window just big enough to hold the map, not the menu
	sf::RenderWindow window(sf::VideoMode((sideLen * game_map::SIDE_PIX) + 70,
		sideLen * game_map::SIDE_PIX),
		"Example map");

	// Enemy sprite texture
	sf::Texture covidTexture;
	bool textureLoadSuccess = false;
	std::string covidTextureFile(game_map::RESOURCE_PATH + "covid_1.png");
	while (!textureLoadSuccess) {
		try {
			if (covidTexture.loadFromFile(covidTextureFile,
				sf::IntRect(0, 0, game_map::SIDE_PIX, game_map::SIDE_PIX)
			)
				) {
				textureLoadSuccess = true;
			}
			else {
				throw 1;
			}
		}
		catch (int e) {
			switch (e) {
			case 1:
				std::cout << "Error opening file " << covidTextureFile << std::endl;
			default:
				std::cout << "Enter the full file path for the ground texture file: ";
				std::getline(std::cin, covidTextureFile);
			}
		}
	}


	sf::Sprite covid;
	covid.setTexture(covidTexture);
	sf::Vector2u covidCoords = gameMap.getStartCoords();
	covid.setPosition(getCellPositionFromCoordinates(covidCoords, game_map::SIDE_FLT));

	Path* pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);

	const float covidSpeed = 0.05f;

	sf::Vector2f covidDirection;
	sf::Vector2f covidDestination = getCellPositionFromCoordinates(pathPtr->getNextCoords(),
		game_map::SIDE_FLT);
	sf::Vector2f distanceToDestination(std::abs(covidDestination.x - covid.getPosition().x),
		std::abs(covidDestination.y - covid.getPosition().y));

	bool drawCovid = true;
	//MAP MAIN STUFF**********************************************************


	bool waveGoing = 0; //0 if the wave has finished/hasn't started yet, 1 if the wave of enemies is still going
	bool waveNumber = 0; //0 through 5
	int bulletGeneration = 0; //used to create gaps between the generation of each bullet

	//opening a window
	//sf::RenderWindow window;
	//sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2) - 755, (sf::VideoMode::getDesktopMode().height / 2) - 390);
	/*window.create(sf::VideoMode(1500, 700), "SFML Testing", sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(centerWindow);
	window.setKeyRepeatEnabled(true);*/


	//Defining objects
	//Tower tower(sf::Vector2f(150, 150), sf::Vector2f(0, 0), "soap");
	Enemy enemy(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX));
	std::vector<Bullet> bulletVec;
	std::vector<Tower> towerVec;
	std::vector<Tower> shootingTowers;
	bool sanitizerButton = false;
	bool soapButton = false;
	bool isFiring = false;
	//bool ground = false;

	//set position of objects for testing
	//enemy.setPos(sf::Vector2f(500, 200));
	//tower.move(sf::Vector2f(100, 50));

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
	sf::RectangleShape soapSelection(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX));
	soapSelection.setTexture(&soapSelectTexture);
	soapSelection.move(sf::Vector2f((8 * game_map::SIDE_PIX) + 10, 200)); //temporary location for testing purposes
	sf::RectangleShape sanitizerSelection(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX));
	sanitizerSelection.setTexture(&sanitizerSelectTexture);
	sanitizerSelection.move(sf::Vector2f((8 * game_map::SIDE_PIX) + 10, 100)); //temporary location for testing purposes

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
	int mouseX = -1; 
	int mouseY = -1;
	int lastX = -1;
	int lastY = -1;



	//for (int i = 0; i < 8; i++)
	//{
	//	for (int j = 0; j < 8; j++)
	//	{
	//		Cell* chosen = (gameMap.getCells())[i][j];
	//		if (chosen->getCellType() == CellEnum::PATH)
	//		{
	//			std::cout << "row" << i << " column " << j << "  PATH" << endl;
	//		}
	//		else std::cout << "row" << i << " column " << j << "  GROUND" << endl;
	//	}
	//}


	//main loop
	while (window.isOpen())
	{
		//tower.getRect().setTexture(&soapTexture);

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

			/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
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
			}*/
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				mouseX = sf::Mouse::getPosition(window).x;
				mouseY = sf::Mouse::getPosition(window).y;
				//std::cout << mouseX << "   " << mouseY << endl;
			}

		}

		

		//MAP STUFF
		if (drawCovid) {
			// Check if the covid has made it to next path
			distanceToDestination = sf::Vector2f(abs(covidDestination.x - covid.getPosition().x),
				abs(covidDestination.y - covid.getPosition().y)
			);
			if (std::abs(distanceToDestination.x) < 1.f
				&& std::abs(distanceToDestination.y) < 1.f) {
				covidCoords = pathPtr->getNextCoords();
				pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
				covidDestination = getCellPositionFromCoordinates(pathPtr->getNextCoords(),
					game_map::SIDE_FLT);
				// Check if covid has made it to the end
				if (pathPtr->getNextCoords() == gameMap.getExitCoords()) {
					drawCovid = false;
				}
			}


			// Move the covid
			covidDirection = normalize(covidDestination - covid.getPosition());
			covid.move(covidSpeed * covidDirection);
		}

		try //try placing a tower 
		{
			sf::Vector2f place = checkTowerPlacement(mouseX, mouseY, gameMap, towerVec);
			Tower t(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX), place, "sanitizer");
			t.getRect().setTexture(&sanitizerTexture);
			towerVec.push_back(t);
			mouseX = mouseY = -1; //set to -1 so that it doesn't keep trying to put the tower in the last selected place
		}
		catch (int a)
		{
			cerr << "You cannot place a tower there" << endl;
			mouseX = mouseY = -1; 
			//break;
		}
		catch (...)
		{}

		//create gap between bullets
		if (bulletGeneration % 200 == 0) //adjust the mod # higher to slow production of bullets, lower the mod # to increase bullet production
		{
			isFiring = true;
		}
		bulletGeneration++;

		//update animations
		soapAnimation.Update(0, deltaTime);
		sanitizerAnimation.Update(0, deltaTime);

		//(for animation) set texture rectangle for all towers
		//tower.getRect().setTextureRect(soapAnimation.uvRect); //step through the array/vector and use the same element # for the corresponding player and animation objects
		for (int i = 0; i < towerVec.size(); i++)
		{
			towerVec[i].getRect().setTextureRect(sanitizerAnimation.uvRect);
		}
		//player1.getRect().setTextureRect(sanitizerAnimation.uvRect);

		window.clear();	

		//MAP STUFF
		gameMap.draw(window);
		if (drawCovid) {
			window.draw(covid);
		}
		
		//squirtSound.setLoop(false);
		
		//make sure you only play the sound once (turn a bool variable on if a type is shooting and play the sound only once at the end of the for loop)
		//generate bullets (squirt dimensions: 17x4, bubble dimensions: 75x33)
		if (isFiring == true)
		{
			//Bullet newBullet(sf::Vector2f(75, 33), tower.getX(), tower.getY(), enemy.getX(), enemy.getY()); //shoot at first enemy, pass in tower for starting position
			//newBullet.getBullet().setTexture(&bubbleTexture);
			//bulletVec.push_back(newBullet);
			//isFiring = false;
			//squirtSound.play();
			//squirtSound.stop();
		}

		
		//draw bullets to window
		for (int i = 0; i < bulletVec.size(); i++)
		{
			bulletVec[i].draw(window);
			bulletVec[i].fire();
		}


		//draw all objects to window
		//tower.draw(window);
		for (int i = 0; i < towerVec.size(); i++)
		{
			towerVec[i].draw(window);
		}
		//player1.draw(window);
		enemy.draw(window);
		window.draw(sanitizerSelection);
		window.draw(soapSelection);
		window.display();

	}
}

sf::Vector2f getCellPositionFromCoordinates(const sf::Vector2u& coords, const float& sideFlt) {
	return sf::Vector2f(static_cast<float>(coords.y) * sideFlt,
		static_cast<float>(coords.x) * sideFlt);
}

sf::Vector2f normalize(const sf::Vector2f& vec) {
	float magnitude = sqrt((vec.x * vec.x) + (vec.y * vec.y));
	if (magnitude == 0.f) {
		return vec;
	}
	return sf::Vector2f(vec.x / magnitude, vec.y / magnitude);

}

sf::Vector2f checkTowerPlacement(int x, int y, GameMap& gm, std::vector<Tower>& towerVec) //USES POLYMORPHISM
{
	if (x <= 0 || x >= 8 * game_map::SIDE_PIX || y <= 0 || y >= 8 * game_map::SIDE_PIX)
	{
		throw "nope";
		//return sf::Vector2f(-1, -1);
	}
	Cell* selected = (gm.getCells())[static_cast<int>(y / game_map::SIDE_PIX)][static_cast<int>(x / game_map::SIDE_PIX)];
	if (selected->getCellType() == CellEnum::GROUND)
	{
		for (int i = 0; i < towerVec.size(); i++)
		{
			if (towerVec[i].getPosition() == sf::Vector2f(static_cast<int>(x / game_map::SIDE_PIX) * game_map::SIDE_PIX,
				static_cast<int>(y / game_map::SIDE_PIX) * game_map::SIDE_PIX))
				throw 1;
		}

		return sf::Vector2f(static_cast<int>(x / game_map::SIDE_PIX) * game_map::SIDE_PIX, static_cast<int>(y / game_map::SIDE_PIX) * game_map::SIDE_PIX);
	}
	
	throw "nope";
}

//maybe for the different waves, once the enemy queue reaches 0, we enter a while loop and until the player presses start, after the player presses start
//we load the enemies again and continue (keep track of the waves)



//ask the group how to make the firing of the bullets more consistent