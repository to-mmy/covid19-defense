#include "GameMap.h"
#include "Cell.h"
#include "CellEnum.h"
#include "Path.h"
#include "Ground.h"
#include "Animation.h"
//#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include "PlaceTower.h"
#include "MouseDown.h"
//#include "corona.h"
#include "Collision.h"
#include "BulletSprite.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <queue>

sf::Vector2f normalize(const sf::Vector2f& vec);
sf::Vector2f checkTowerPlacement(const sf::Vector2f&, GameMap* const, std::vector<Tower>&);
Cell* getLocation(sf::Vector2f towerCoords, GameMap& map); //gets a pointer to the ground cell that the tower will be placed on

int main() {
	//MAP MAIN STUFF**********************************************************
	const sf::Vector2f GAME_MAP_ORIGIN = sf::Vector2f(0.f, 0.f);

	GameMap gameMap(std::string("map_1_data"), GAME_MAP_ORIGIN);

	//// Enemy sprite texture
	sf::Texture covidTexture;
	bool textureLoadSuccess = false;
	std::string covidTextureFile(/*game_map::RESOURCE_PATH + */"covid_1.png");
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

	Path* pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
	covid.setPosition(GAME_MAP_ORIGIN + pathPtr->getPosition());

	covid.setScale(game_map::SPRITE_SCALE);

	const float COVID_SPEED = 0.05f;

	sf::Vector2f covidDirection;
	sf::Vector2f covidDestination = GAME_MAP_ORIGIN + pathPtr->getNextPosition();
	sf::Vector2f distanceToDestination(std::abs(covidDestination.x - covid.getPosition().x),
		std::abs(covidDestination.y - covid.getPosition().y));

	bool drawCovid = true;
	bool deadCovid = false;
	//MAP MAIN STUFF**********************************************************

	bool paused = true;

	bool waveGoing = false;
		0; //0 if the wave has finished/hasn't started yet, 1 if the wave of enemies is still going
	int waveNumber = 0; //0 through 5
	//int bulletGeneration = 0; //used to create gaps between the generation of each bullet

	//opening a window
	sf::RenderWindow window;
	sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2) - 540,
		(sf::VideoMode::getDesktopMode().height / 2) - 360);
	window.create(sf::VideoMode(1080, 720), "Tower Defense Game",
		sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(centerWindow);
	window.setKeyRepeatEnabled(true);

	//Defining objects
	//Tower tower(sf::Vector2f(150, 150), sf::Vector2f(0, 0), "soap");
	//Enemy enemy(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX));
	//TESTING WITH SPRITES***************************************
	std::vector<BulletSprite> bSpriteVec;
	std::vector<Bullet> bulletVec;
	std::vector<Tower> towerVec;
	std::vector<Tower> shootingTowers;
	std::queue<sf::Sprite> enemyQueue;
	std::queue<sf::Vector2f> distanceToDestinationQueue;
	std::queue<sf::Vector2f> destination;
	bool sanitizerShooting = false;
	bool soapShooting = false;
	//sf::Sprite* enemyPtr;
	/*std::queue<int> tester;
	tester.push(1);
	tester.push(2);
	tester.push(3);
	int* testerPtr = &tester.front() + 2;
	std::cout << *testerPtr << std::endl;*/
	bool isFiring = false;
	//bool ground = false;


	//sf::Vector2u covidCoords;
	////filling enemy queue
	//while (enemyQueue.size() < 20)
	//{
	//	sf::Sprite covid;
	//	covid.setTexture(covidTexture);
	//	covidCoords = gameMap.getStartCoords();

	//	Path* pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
	//	covid.setPosition(GAME_MAP_ORIGIN + pathPtr->getPosition());

	//	covid.setScale(game_map::SPRITE_SCALE);

	//	const float COVID_SPEED = 0.05f;

	//	sf::Vector2f covidDirection;
	//	sf::Vector2f covidDestination = GAME_MAP_ORIGIN + pathPtr->getNextPosition();
	//	destination.push(covidDestination);
	//	sf::Vector2f distToDest(std::abs(covidDestination.x - covid.getPosition().x),
	//		std::abs(covidDestination.y - covid.getPosition().y));
	//	distanceToDestinationQueue.push(distToDest);
	//	enemyQueue.push(covid);
	//}

	// Create coronavirus object
	//corona coronavirus(covidTexture, gameMap);

	//loading textures for animated towers     //textures and animations have to be loaded outside of the main while loop
	sf::Texture soapTexture;
	soapTexture.loadFromFile(/*game_map::RESOURCE_PATH +*/
		"bar_soap_transparent.png");    //bar_soap_transparent or sanitizer_bottle_transparent
	sf::Texture sanitizerTexture;
	sanitizerTexture.loadFromFile(/*game_map::RESOURCE_PATH + */"sanitizer_bottle_transparent.png");

	//loading textures for tower selection
	sf::Texture pauseTexture;
	pauseTexture.loadFromFile(/*game_map::RESOURCE_PATH + */"pause_button.png");
	sf::Texture playTexture;
	playTexture.loadFromFile(/*game_map::RESOURCE_PATH + */"play_button.png");
	sf::Texture soapSelectTexture;
	soapSelectTexture.loadFromFile(/*game_map::RESOURCE_PATH + */"bar_soap_selection.png");
	sf::Texture sanitizerSelectTexture;
	sanitizerSelectTexture.loadFromFile(/*game_map::RESOURCE_PATH + */"sanitizer_bottle_selection.png");

	// creating and positioning selection buttons
	const float SPACER = 10.f;
	sf::RectangleShape playPauseButton(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT));
	playPauseButton.setTexture(&playTexture);
	playPauseButton.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
		+ SPACER,
		(game_map::SIDE_FLT + SPACER) * 0.f)
	); // 0th down

	sf::RectangleShape soapSelection(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT));
	soapSelection.setTexture(&soapSelectTexture);
	soapSelection.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
		+ SPACER,
		(game_map::SIDE_FLT + SPACER) * 1.f)
	); // 1st down
	sf::RectangleShape sanitizerSelection(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT));
	sanitizerSelection.setTexture(&sanitizerSelectTexture);
	sanitizerSelection.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
		+ SPACER,
		(game_map::SIDE_FLT + SPACER) * 2.f)
	); // 2nd down

// Rects holding bounds of the buttons, for checking mouse input
	sf::FloatRect playPauseRect = playPauseButton.getGlobalBounds();
	sf::FloatRect soapSelectionRect = soapSelection.getGlobalBounds();
	sf::FloatRect sanitizerSelectionRect = sanitizerSelection.getGlobalBounds();

	// bools for determining if the buttons are clickable
	bool playPauseClickable = false;
	bool soapSelectionClickable = false;
	bool sanitizerSelectionClickable = false;

	// A temporary tower to show placement
	Tower* towerIndicatorPtr = nullptr;
	//towerIndicatorPtr.setPlacedDown(false);

	//loading textures for bullets
	sf::Texture squirtTexture;
	squirtTexture.loadFromFile(/*game_map::RESOURCE_PATH +*/
		"sanitizer_squirt_rect.png"); //sanitizer_squirt_rect (maybe sanitizer_squirt_transparent) or soap_bubbles_transparent
	sf::Texture bubbleTexture;
	bubbleTexture.loadFromFile(/*game_map::RESOURCE_PATH + */"soap_bubbles.png");

	//loading sounds
	sf::SoundBuffer squirtBuffer;
	squirtBuffer.loadFromFile(/*game_map::RESOURCE_PATH + */"squirt_sound.wav");
	sf::Sound squirtSound;
	squirtSound.setBuffer(squirtBuffer);
	sf::SoundBuffer bubbleBuffer;
	bubbleBuffer.loadFromFile(/*game_map::RESOURCE_PATH + */"bubble_sound.wav");
	sf::Sound bubbleSound;
	bubbleSound.setBuffer(bubbleBuffer);

	//animation
	Animation soapAnimation(&soapTexture, sf::Vector2u(3, 1), 0.3f);
	Animation sanitizerAnimation(&sanitizerTexture, sf::Vector2u(3, 1), 0.3f);
	//player.setAnimation(soapAnimation);
	float animationDeltaTime = 0.0f;
	sf::Clock animationClock;

	//coordinates for mouse click

	sf::Vector2i mousePos(-1, -1);
	sf::Vector2f mouseFloat(mousePos);

	// delay timer for mouse click
	sf::Clock clickClock;
	sf::Clock playPauseClock;
	const float CLICK_DELAY = 0.2f; // delay in seconds -> 0.2 seconds (100 ms == instant to humans)

	// What kind of tower, if any, to place
	PlaceTower placeWhat = PlaceTower::NONE;
	bool placeTowerConfirmed = false;


	sf::Event event;
	MouseDown leftMouseDown = MouseDown::RELEASE;
	MouseDown rightMouseDown = MouseDown::RELEASE;

	//main loop
	while (window.isOpen()) {
		//tower.getRect().setTexture(&soapTexture);

		//animationDeltaTime used to make animation consistent for different OS's
		animationDeltaTime = animationClock.restart().asSeconds();

		// Input polling
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseMoved:
				mousePos = sf::Mouse::getPosition(window);
				break;
			case sf::Event::MouseButtonPressed:
				// left click
				if (event.mouseButton.button == sf::Mouse::Left) {
					clickClock.restart();
					leftMouseDown = MouseDown::PRESS;
				}
				// right click -> cancel placement
				else if (event.mouseButton.button == sf::Mouse::Right
					&& rightMouseDown == MouseDown::RELEASE) {
					placeWhat = PlaceTower::NONE;
					rightMouseDown = MouseDown::PRESS;
				}
				mousePos = sf::Mouse::getPosition(window);
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left) {
					leftMouseDown = MouseDown::RELEASE;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					rightMouseDown = MouseDown::RELEASE;
				}
			default:
				mousePos = sf::Mouse::getPosition(window);
				if (leftMouseDown == MouseDown::PRESS) {
					leftMouseDown = MouseDown::HOLD;
				}
			}

		}

		//if (coronavirus.getDrawCovid()) {
		//	// Check if the covid has made it to next path
		//	corona.setDistanceToDestination = sf::Vector2f(abs(covidDestination.x - covid.getPosition().x),
		//		abs(covidDestination.y - covid.getPosition().y)
		//	);
		//	if (std::abs(corona.getDistanceToDestination.x) < 1.f
		//		&& std::abs(corona.getDistanceToDestination.y) < 1.f) {
		//		corona.setCovidCoords = corona.getPathPtr->getNextCoords();
		//		corona.setPathPtr(dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]));
		//		corona.setCovidDestination(getCellPositionFromCoordinates(pathPtr->getNextCoords(),
		//			game_map::SIDE_FLT));
		//		// Check if covid has made it to the end
		//		if (corona.getPathPtr->getNextCoords() == gameMap.getExitCoords()) {
		//			corona.setDrawCovid(false);
		//		}
		//	}


		//	// Move the covid
		//	corona.setCovidDirection(normalize(covidDestination - covid.getPosition());
		//	corona.move(corona.getCovidSpeed() * corona.getCovidDirection());
		//}

		mouseFloat = sf::Vector2f(mousePos);
		if (clickClock.getElapsedTime().asSeconds() > CLICK_DELAY && leftMouseDown == MouseDown::RELEASE) {
			// For each button, highlight it if it is clickable (mouse within borders)
			clickClock.restart();
			if (playPauseRect.contains(mouseFloat)) {
				playPauseButton.setFillColor(sf::Color::Yellow);
				playPauseClickable = true;
				soapSelection.setFillColor(sf::Color::White);
				soapSelectionClickable = false;
				sanitizerSelection.setFillColor(sf::Color::White);
				sanitizerSelectionClickable = false;
			}
			else if (soapSelectionRect.contains(mouseFloat) && waveGoing == false) { //can only place down towers if the wave isn't going on
				playPauseButton.setFillColor(sf::Color::White);
				playPauseClickable = false;
				soapSelection.setFillColor(sf::Color::Yellow);
				soapSelectionClickable = true;
				sanitizerSelection.setFillColor(sf::Color::White);
				sanitizerSelectionClickable = false;
			}
			else if (sanitizerSelectionRect.contains(mouseFloat) && waveGoing == false) {
				playPauseButton.setFillColor(sf::Color::White);
				playPauseClickable = false;
				soapSelection.setFillColor(sf::Color::White);
				soapSelectionClickable = false;
				sanitizerSelection.setFillColor(sf::Color::Yellow);
				sanitizerSelectionClickable = true;
			}
			else {
				playPauseButton.setFillColor(sf::Color::White);
				playPauseClickable = false;
				soapSelection.setFillColor(sf::Color::White);
				soapSelectionClickable = false;
				sanitizerSelection.setFillColor(sf::Color::White);
				sanitizerSelectionClickable = false;
			}
		}
		else if (clickClock.getElapsedTime().asSeconds() > CLICK_DELAY && leftMouseDown == MouseDown::PRESS) {
			playPauseButton.setFillColor(sf::Color::White);
			playPauseClickable = false;
			soapSelection.setFillColor(sf::Color::White);
			soapSelectionClickable = false;
			sanitizerSelection.setFillColor(sf::Color::White);
			sanitizerSelectionClickable = false;
		}
		// We can click buttons and place towers while paused
		// Determine if something was clicked
		if (leftMouseDown == MouseDown::PRESS) {
			// Clicked the play/pause button
			if (playPauseClickable && playPauseClock.getElapsedTime().asSeconds() > CLICK_DELAY) {
				playPauseClock.restart();
				if (paused) {
					paused = false;
					playPauseButton.setFillColor(sf::Color::Red);
					playPauseButton.setTexture(&pauseTexture);
					if (waveGoing == false) //start the wave if the play presses play while the wave is not running
					{
						waveGoing = true;
					}
				}
				else {
					paused = true;
					playPauseButton.setFillColor(sf::Color::Red);
					playPauseButton.setTexture(&playTexture);
				}
			}
			// Clicked the sanitizer tower selection button
			else if (sanitizerSelectionClickable) {
				placeWhat = PlaceTower::SANITIZER;
			}
			// Clicked the soap tower selection button
			else if (soapSelectionClickable) {
				placeWhat = PlaceTower::SOAP;
			}
			// Clicked to place a sanitizer tower
			else if (placeWhat == PlaceTower::SANITIZER) {
				placeTowerConfirmed = true;
			}
			// Clicked to place a soap tower
			else if (placeWhat == PlaceTower::SOAP) {
				placeTowerConfirmed = true;
			}
		}

		switch (placeWhat) {
		case PlaceTower::NONE:
			if (towerIndicatorPtr) {
				delete towerIndicatorPtr;
				towerIndicatorPtr = nullptr;
			}
			placeTowerConfirmed = false;
			break;
		case PlaceTower::SANITIZER:
			try { //try placing a sanitizer tower
				// show an indicator for the Tower
				// If the indicator already exists, clean it up
				if (towerIndicatorPtr) {
					delete towerIndicatorPtr;
					towerIndicatorPtr = nullptr;
				}
				// Show the indicator as green by default
				towerIndicatorPtr = new Tower(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), mouseFloat, "sanitizer", gameMap.getCells()[0][0]);
				towerIndicatorPtr->setPlacedDown(false);
				// Center on mouse
				towerIndicatorPtr->getRect().setOrigin(sf::Vector2f(game_map::SIDE_FLT / 2.f, game_map::SIDE_FLT / 2.f));
				towerIndicatorPtr->getRect().setPosition(mouseFloat);
				// Set textures, color
				towerIndicatorPtr->getRect().setTexture(&sanitizerTexture);
				towerIndicatorPtr->getRect().setTextureRect(sanitizerAnimation.uvRect);
				towerIndicatorPtr->getRect().setFillColor(sf::Color::Green);


				sf::Vector2f place = checkTowerPlacement(mouseFloat, &gameMap, towerVec);
				if (placeTowerConfirmed) {
					// Add the tower to the towerVec
					Tower t(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), place, "sanitizer", getLocation(place, gameMap));
					t.setPlacedDown(true);
					t.getRect().setTexture(&sanitizerTexture);
					towerVec.push_back(t);

					// reset values
					delete towerIndicatorPtr;
					towerIndicatorPtr = nullptr;
					placeWhat = PlaceTower::NONE;
					placeTowerConfirmed = false;
				}
			}
			catch (int a) {
				towerIndicatorPtr->getRect().setFillColor(sf::Color::Red);
				placeTowerConfirmed = false;
			}
			catch (...) {}
			break;
		case PlaceTower::SOAP:
			try { //try placing a soap tower
				// Show an indicator for the tower
				towerIndicatorPtr = new Tower(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), mouseFloat, "soap", gameMap.getCells()[0][0]);
				towerIndicatorPtr->setPlacedDown(false);
				// Center on mouse
				towerIndicatorPtr->getRect().setOrigin(sf::Vector2f(game_map::SIDE_FLT / 2.f, game_map::SIDE_FLT / 2.f));
				towerIndicatorPtr->getRect().setPosition(mouseFloat);
				// Set textures, color
				towerIndicatorPtr->getRect().setTexture(&soapTexture);
				towerIndicatorPtr->getRect().setTextureRect(soapAnimation.uvRect);
				towerIndicatorPtr->getRect().setFillColor(sf::Color::Green);

				sf::Vector2f place = checkTowerPlacement(mouseFloat, &gameMap, towerVec);
				//std::cout << "here" << std::endl;
				if (placeTowerConfirmed) {
					//std::cout << "here" << std::endl;
					// Add the tower to the towerVec
					Tower t(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), place, "soap", getLocation(place, gameMap));
					//std::cout << "here" << std::endl;
					t.setPlacedDown(true);
					t.getRect().setTexture(&soapTexture);
					towerVec.push_back(t);

					// reset values
					delete towerIndicatorPtr;
					towerIndicatorPtr = nullptr;
					placeWhat = PlaceTower::NONE;
					placeTowerConfirmed = false;
				}
			}
			catch (int a) {
				towerIndicatorPtr->getRect().setFillColor(sf::Color::Red);
				placeTowerConfirmed = false;
			}
			catch (...) {}
			break;
		default:
			placeWhat = PlaceTower::NONE;
		}

		//std::cout << "here" << std::endl;
		// Update enemies, bullets (only update while game is not paused)
		if (!paused) {
			if (drawCovid) {
				// Check if the covid has made it to next path
				distanceToDestination = sf::Vector2f(abs(covidDestination.x - covid.getPosition().x),
					abs(covidDestination.y - covid.getPosition().y)
				);
				if (std::abs(distanceToDestination.x) < 1.f
					&& std::abs(distanceToDestination.y) < 1.f) {
					covidCoords = pathPtr->getNextCoords();
					pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
					covidDestination = GAME_MAP_ORIGIN + pathPtr->getNextPosition();
					// Check if covid has made it to the end
					if (pathPtr->getNextCoords() == gameMap.getExitCoords()) {
						drawCovid = false;
					}
				}


				// Move the covid
				covidDirection = normalize(covidDestination - covid.getPosition());
				covid.move(COVID_SPEED * covidDirection);
			}
		}

		//update animations
		soapAnimation.Update(0, animationDeltaTime);
		sanitizerAnimation.Update(0, animationDeltaTime);

		//(for animation) set texture rectangle for all towers
		//tower.getRect().setTextureRect(soapAnimation.uvRect); //step through the array/vector and use the same element # for the corresponding player and animation objects
		for (unsigned i = 0; i < towerVec.size(); i++) {
			towerVec[i].getRect().setTextureRect(sanitizerAnimation.uvRect);
		}
		//player1.getRect().setTextureRect(sanitizerAnimation.uvRect);

		window.clear();


		// Drawing
		gameMap.draw(window);
		/*if (drawCovid) {
			window.draw(covid);
		}*/
		//std::cout << "here" << std::endl;
		//outside of this for loop should be the enemy queue loop
		for (int i = 0; i < towerVec.size(); i++) //pass in enemies that are in the tower's range
		{
			if (waveGoing == false) //don't produce bullets if the wave isn't going
				break;

			int startRow = 0; //these coordinates indicate a tower's range (which cells it can attack bullets in)
			int endRow = 0; //the tower's range is the surrounding cells (the 3x3 section with the tower at its center)
			int startCol = 0;
			int endCol = 0;

			if (towerVec[i].getGroundLocation()->getCoords().x == 0)
			{
				startRow = 0;
			}
			else startRow = towerVec[i].getGroundLocation()->getCoords().x - 1;

			if (towerVec[i].getGroundLocation()->getCoords().x == 7)
			{
				endRow = 7;
			}
			else endRow = towerVec[i].getGroundLocation()->getCoords().x + 1;

			if (towerVec[i].getGroundLocation()->getCoords().y == 0)
			{
				startCol = 0;
			}
			else startCol = towerVec[i].getGroundLocation()->getCoords().y - 1;

			if (towerVec[i].getGroundLocation()->getCoords().y == 7)
			{
				endCol = 7;
			}
			else endCol = towerVec[i].getGroundLocation()->getCoords().y + 1;
			
			for (int r = startRow; r <= endRow; r++)
			{
				for (int c = startCol; c <= endCol; c++)
				{
					if (towerVec[i].shouldShoot() == true && towerVec[i].getHaveTarget() == false &&
						Collision::PixelPerfectTest(covid, gameMap.getCells()[r][c]->getSprite()))
						//check if enemy is in range, if there has been a good enough gap between the last bullet shot, and if the tower already has a target or not
					{
						//generate bullets (squirt dimensions: 17x4, bubble dimensions: 75x33)
						if (towerVec[i].getType() == "sanitizer")
						{
							Bullet newBullet("squirt", towerVec[i].getX() + towerVec[i].getOffsetX(),
								towerVec[i].getY() + towerVec[i].getOffsetY(), (covid.getPosition().x + (game_map::SIDE_PIX / 2))
								, (covid.getPosition().y + (game_map::SIDE_PIX / 2))); //shoot at first enemy in range, pass in tower for starting location of bullet
							newBullet.getSprite().setTexture(squirtTexture);
							bulletVec.push_back(newBullet);
							sanitizerShooting = true;
						}
						else //soap 
						{
							Bullet newBullet("bubble", towerVec[i].getX() + towerVec[i].getOffsetX(),
								towerVec[i].getY() + towerVec[i].getOffsetY(), (covid.getPosition().x + (game_map::SIDE_PIX / 2))
								, (covid.getPosition().y + (game_map::SIDE_PIX / 2))); //shoot at first enemy, pass in tower for starting position
							newBullet.getSprite().setTexture(bubbleTexture); //add game_map::SIDE_PIX / 2 to the enemy coords so the bullet shoots at enemy's center
							bulletVec.push_back(newBullet);
							soapShooting = true;
						}
						towerVec[i].setHaveTarget(true);
					}
				}
			}
		}

		for (auto& i : towerVec) //range-based for loop
		{
			i.setHaveTarget(false); //reset all of the tower's shooting ability so they can choose a new target in the next iteration of the game loop
			i.incrementShootingGap();
		}

		//draw all objects to window
		for (unsigned i = 0; i < towerVec.size(); i++) {
			towerVec[i].draw(window);
		}


		//draw bullets to window
		for (unsigned i = 0; i < bulletVec.size(); i++) {
			bulletVec[i].draw(window);
			bulletVec[i].fire();

			if ((bulletVec[i].getLeft() + bulletVec[i].getSprite().getScale().x < -50 || 
				(bulletVec[i].getTop() + bulletVec[i].getSprite().getScale().y > ((8 * game_map::SIDE_PIX) + 50))))
				{
					bulletVec.erase(bulletVec.begin() + i);
				} //remove the bullet from vector if it goes off screen
		}

		if (sanitizerShooting) //only play the sound for sanitizer towers firing bullets once per loop
		{
			squirtSound.play();
			//squirtSound.stop();
			sanitizerShooting = false;
		}
		if (soapShooting) //only play the sound for soap towers firing bullets once per loop
		{
			bubbleSound.play();
			//bubbleSound.stop();
			soapShooting = false;
		}

		//step through enemy queue here using &queueName.front() + i to check collisions with bullets
		for (int i = 0; i < bulletVec.size(); i++)
		{
			if (Collision::PixelPerfectTest(bulletVec[i].getSprite(), covid))
			{
				deadCovid = true;
			}
		}
		if (!deadCovid)
		{
			window.draw(covid);
		}

		if (towerIndicatorPtr) {
			towerIndicatorPtr->draw(window);
		}
		/*if (corona.getDrawCovid()) {
			window.draw(covid);
		}*/
		window.draw(playPauseButton);
		window.draw(sanitizerSelection);
		window.draw(soapSelection);
		window.display();

		// Cleaning up allocated memory
		if (towerIndicatorPtr) {
			delete towerIndicatorPtr;
			towerIndicatorPtr = nullptr;
		}
	}
}

sf::Vector2f normalize(const sf::Vector2f& vec) {
	float magnitude = sqrt((vec.x * vec.x) + (vec.y * vec.y));
	if (magnitude == 0.f) {
		return vec;
	}
	return sf::Vector2f(vec.x / magnitude, vec.y / magnitude);

}

sf::Vector2f checkTowerPlacement(const sf::Vector2f& mouseFloat, GameMap* const gameMapPtr,
	std::vector<Tower>& towerVec) { //USES POLYMORPHISM


	if (!gameMapPtr->getGlobalBounds().contains(mouseFloat)) { // outside boundaries of game map
		throw 1;
	}
	Cell* selected = gameMapPtr->getCells()[static_cast<int>(mouseFloat.y / game_map::SIDE_FLT)]
		[static_cast<int>(mouseFloat.x / game_map::SIDE_FLT)];
	if (selected->getCellType() == CellEnum::GROUND) {
		// Check if tower already in towerVec
		for (unsigned i = 0; i < towerVec.size(); i++) {
			if (towerVec[i].getPosition() == selected->getPosition()) {
				throw 1;
			}
		}

		return selected->getPosition();
	}
	else {
		// tower placement is on a Path
		throw 1;
	}
}

Cell* getLocation(sf::Vector2f towerCoords, GameMap& map)
{
	int row = towerCoords.y / game_map::SIDE_PIX;
	int col = towerCoords.x / game_map::SIDE_PIX;
	//std::cout << "here1" << std::endl;
	return map.getCells()[row][col];
}

//maybe for the different waves, once the enemy queue reaches 0, we enter a while loop and until the player presses start, after the player presses start
//we load the enemies again and continue (keep track of the waves)