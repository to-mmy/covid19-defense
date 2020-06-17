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
sf::Vector2f towerPlacement(int, int, GameMap&, std::vector<Tower>&); //checks if the tower can be placed where the player clicks
bool buttonPressed(int mouseX, int mouseY, sf::RectangleShape& button);


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


	bool waveGoing = false; //0 if the wave has finished/hasn't started yet, 1 if the wave of enemies is still going
	int waveNumber = 0; //0 through 5

	//opening a window
	//sf::RenderWindow window;
	//sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2) - 755, (sf::VideoMode::getDesktopMode().height / 2) - 390);
	/*window.create(sf::VideoMode(1500, 700), "SFML Testing", sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(centerWindow);
	window.setKeyRepeatEnabled(true);*/


	//Defining objects
	//Tower tower(sf::Vector2f(50, 50), sf::Vector2f(0, 0), "sanitizer");
	//tower.getRect().move(sf::Vector2f(100, 100));
	//Enemy enemy(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX));
	std::vector<Bullet> bulletVec;
	std::vector<Tower> towerVec;
	std::vector<Tower> shootingTowers;
	bool sanitizerSelected = false;
	bool soapSelected = false;
	bool sanitizerFiring = false;
	bool soapFiring = false;
	int squirtGeneration = 0;
	int bubbleGeneration = 0; //used to create gaps between the generation of each bullet

	//loading textures for animated towers     //textures and animations have to be loaded outside of the main while loop
	sf::Texture soapTexture; 
	soapTexture.loadFromFile("bar_soap_transparent.png");    //bar_soap_transparent or sanitizer_bottle_transparent
	sf::Texture sanitizerTexture;
	sanitizerTexture.loadFromFile("sanitizer_bottle_transparent.png");

	//loading textures for tower selection
	sf::Texture soapButtonTexture;
	soapButtonTexture.loadFromFile("bar_soap_selection.png");
	sf::Texture sanitizerButtonTexture;
	sanitizerButtonTexture.loadFromFile("sanitizer_bottle_selection.png");

	//creating tower selection pictures
	sf::RectangleShape soapButton(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX));
	soapButton.setTexture(&soapButtonTexture);
	soapButton.move(sf::Vector2f((8 * game_map::SIDE_PIX) + 10, 200)); //temporary location for testing purposes
	sf::RectangleShape sanitizerButton(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX));
	sanitizerButton.setTexture(&sanitizerButtonTexture);
	sanitizerButton.move(sf::Vector2f((8 * game_map::SIDE_PIX) + 10, 100)); //temporary location for testing purposes

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


	//main loop
	while (window.isOpen())
	{
		//tower.getRect().setTexture(&sanitizerTexture);

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

			/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				tower.getRect().move(sf::Vector2f(0, -5));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				tower.getRect().move(sf::Vector2f(-5, 0));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				tower.getRect().move(sf::Vector2f(0, 5));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				tower.getRect().move(sf::Vector2f(5, 0));
			}*/

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				mouseX = sf::Mouse::getPosition(window).x;
				mouseY = sf::Mouse::getPosition(window).y;
				//cout << mouseX << mouseY << endl;
			}

		}

		if (buttonPressed(mouseX, mouseY, sanitizerButton) == true && waveGoing == false)
			sanitizerSelected = true;

		if (buttonPressed(mouseX, mouseY, soapButton) == true && waveGoing == false)
			soapSelected = true;
		

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


		//8 * game_map::SIDE_PIX) + 10, 100
		//8 * game_map::SIDE_PIX) + 10, 200



		try //try placing a tower 
		{
			sf::Vector2f place = towerPlacement(mouseX, mouseY, gameMap, towerVec);

			if (sanitizerSelected)
			{
				//cout << "entered sanitizer" << endl;
				Tower t(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX), place, "sanitizer");
				t.getRect().setTexture(&sanitizerTexture);
				towerVec.push_back(t);
				sanitizerSelected = false;
			}
			else if (soapSelected)
			{
				//cout << "entered soap" << endl;
				Tower t(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX), place, "soap");
				t.getRect().setTexture(&soapTexture);
				towerVec.push_back(t);
				soapSelected = false;
			}

			mouseX = mouseY = -1; //set to -1 so that it doesn't keep trying to put the tower in the last selected place
		}
		catch (int a)
		{
			cerr << "There's already a tower there" << endl;
			mouseX = mouseY = -1; 
			//break;
		}
		catch (double b)
		{
			cerr << "You cannot place a tower down during a wave" << endl;
			mouseX = mouseY = -1;
		}
		catch (...)
		{}

		//create gap between bullets
		if (squirtGeneration % 100 == 0) //sanitizer bottle shoots less frequently
		{
			sanitizerFiring = true;
		}
		squirtGeneration++;

		if (bubbleGeneration % 200 == 0) //adjust the mod # higher to slow production of bullets, lower the mod # to increase bullet production
		{
			soapFiring = true;
		}
		bubbleGeneration++;

		//update animations
		soapAnimation.Update(0, deltaTime);
		sanitizerAnimation.Update(0, deltaTime);

		//(for animation) set texture rectangle for all towers
		//tower.getRect().setTextureRect(sanitizerAnimation.uvRect); //step through the array/vector and use the same element # for the corresponding player and animation objects
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
		
		for (int i = 0; i < towerVec.size(); i++) //pass in enemies that are in the tower's range
		{
			int xStart = 0; //these coordinates act as the range for a tower (xStart and yStart form the top left corner of the tower's range)
			int xEnd = 0; //the tower's range is the surrounding cells (the 3x3 section with the tower at its center)
			int yStart = 0;
			int yEnd = 0;

			if (towerVec[i].getX() - game_map::SIDE_PIX < 0)
			{
				xStart = towerVec[i].getX();
				xEnd = xStart + (2 * game_map::SIDE_PIX);
			}
			else if (towerVec[i].getX() + (2 * game_map::SIDE_PIX) > (8 * game_map::SIDE_PIX))
			{
				xStart = towerVec[i].getX() - game_map::SIDE_PIX;
				xEnd = xStart + (2 * game_map::SIDE_PIX);
			}
			else
			{
				xStart = towerVec[i].getX() - game_map::SIDE_PIX;
				xEnd = xStart + (3 * game_map::SIDE_PIX);
			}

			if (towerVec[i].getY() - game_map::SIDE_PIX < 0)
			{
				yStart = towerVec[i].getY();
				yEnd = yStart + (2 * game_map::SIDE_PIX);
			}
			else if (towerVec[i].getY() + (2 * game_map::SIDE_PIX) > (8 * game_map::SIDE_PIX))
			{
				yStart = towerVec[i].getY() - game_map::SIDE_PIX;
				yEnd = yStart + (2 * game_map::SIDE_PIX);
			}
			else
			{
				yStart = towerVec[i].getY() - game_map::SIDE_PIX;
				yEnd = yStart + (3 * game_map::SIDE_PIX);
			}

			//have an enemy for loop here that checks when the enemies are in the range of the tower (go down the enemy queue from first to last)
		}

		//make sure you only play the sound once (turn a bool variable on if a type is shooting and play the sound only once at the end of the for loop)
		//generate bullets (squirt dimensions: 17x4, bubble dimensions: 75x33)
		//if (sanitizerFiring == true)
		//{
		//	Bullet newBullet(sf::Vector2f(17, 4), tower.getX() + tower.getOffsetX()
		//		, tower.getY() + tower.getOffsetY(), 200, 200); //shoot at first enemy, pass in tower for starting position
		//	newBullet.getBullet().setTexture(&squirtTexture);
		//	bulletVec.push_back(newBullet);
		//	sanitizerFiring = false;
		//	squirtSound.play();
		//	squirtSound.stop();
		//}

		//draw all objects to window
		//tower.draw(window);
		for (int i = 0; i < towerVec.size(); i++)
		{
			towerVec[i].draw(window);
		}


		//draw bullets to window
		for (auto i = 0; i < bulletVec.size(); i++)
		{
			bulletVec[i].draw(window);
			bulletVec[i].fire();
			
			if (bulletVec[i].getBottom() < -50 || bulletVec[i].getBottom() > ((8 * game_map::SIDE_PIX) + 50))
			{
				bulletVec.erase(bulletVec.begin() + i);
			} //remove the bullet from vector if it goes off screen
		}

		//tower.draw(window);
		//enemy.draw(window);
		window.draw(sanitizerButton);
		window.draw(soapButton);
		window.display();
		//cout << bulletVec.size() << endl;
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

sf::Vector2f towerPlacement(int x, int y, GameMap& gm, std::vector<Tower>& towerVec) //USES POLYMORPHISM
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

bool buttonPressed(int mouseX, int mouseY, sf::RectangleShape& sanButton)
{
	//std::cout << "entered" << std::endl;
	int startX = sanButton.getPosition().x;
	//cout << startX << "mouseX " << mouseX << endl;
	int endX = startX + game_map::SIDE_PIX;
	int startY = sanButton.getPosition().y;
	//cout << startY << "mouseY " << mouseY << endl;
	int endY = startY + game_map::SIDE_PIX;

	if (mouseX >= startX && mouseX <= endX && mouseY >= startY && mouseY <= endY)
		return true;
	return false;
}

//maybe for the different waves, once the enemy queue reaches 0, we enter a while loop and until the player presses start, after the player presses start
//we load the enemies again and continue (keep track of the waves)



//ask the group how to make the firing of the bullets more consistent