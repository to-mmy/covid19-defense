#include "GameMap.h"
#include "Cell.h"
#include "CellEnum.h"
#include "Path.h"
#include "Ground.h"
#include "Animation.h"
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include "PlaceTower.h"
#include "MouseDown.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

sf::Vector2f normalize(const sf::Vector2f& vec);
sf::Vector2f checkTowerPlacement(const sf::Vector2f&, GameMap* const, std::vector<Tower>&);

int main() {
    //MAP MAIN STUFF**********************************************************
    const sf::Vector2f GAME_MAP_ORIGIN = sf::Vector2f(0.f, 0.f);

    GameMap gameMap(std::string("map_1_data"), GAME_MAP_ORIGIN);

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
            } else {
                throw 1;
            }
        } catch (int e) {
            switch(e) {
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
    //MAP MAIN STUFF**********************************************************

    bool paused = true;

    bool waveGoing =
        0; //0 if the wave has finished/hasn't started yet, 1 if the wave of enemies is still going
    int waveNumber = 0; //0 through 5
    int bulletGeneration = 0; //used to create gaps between the generation of each bullet

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
    Enemy enemy(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX));
    std::vector<Bullet> bulletVec;
    std::vector<Tower> towerVec;
    std::vector<Tower> shootingTowers;
    bool isFiring = false;
    //bool ground = false;

    //loading textures for animated towers     //textures and animations have to be loaded outside of the main while loop
    sf::Texture soapTexture;
    soapTexture.loadFromFile(game_map::RESOURCE_PATH +
                             "bar_soap_transparent.png");    //bar_soap_transparent or sanitizer_bottle_transparent
    sf::Texture sanitizerTexture;
    sanitizerTexture.loadFromFile(game_map::RESOURCE_PATH + "sanitizer_bottle_transparent.png");

    //loading textures for tower selection
    sf::Texture pauseTexture;
    pauseTexture.loadFromFile(game_map::RESOURCE_PATH + "pause_button.png");
    sf::Texture playTexture;
    playTexture.loadFromFile(game_map::RESOURCE_PATH + "play_button.png");
    sf::Texture soapSelectTexture;
    soapSelectTexture.loadFromFile(game_map::RESOURCE_PATH + "bar_soap_selection.png");
    sf::Texture sanitizerSelectTexture;
    sanitizerSelectTexture.loadFromFile(game_map::RESOURCE_PATH + "sanitizer_bottle_selection.png");

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

    //loading textures for bullets
    sf::Texture squirtTexture;
    squirtTexture.loadFromFile(game_map::RESOURCE_PATH +
                               "sanitizer_squirt_rect.png"); //sanitizer_squirt_rect (maybe sanitizer_squirt_transparent) or soap_bubbles_transparent
    sf::Texture bubbleTexture;
    bubbleTexture.loadFromFile(game_map::RESOURCE_PATH + "soap_bubbles_transparent.png");

    //loading sounds
    sf::SoundBuffer squirtBuffer;
    squirtBuffer.loadFromFile(game_map::RESOURCE_PATH + "squirt_sound.wav");
    sf::Sound squirtSound;
    squirtSound.setBuffer(squirtBuffer);

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
            else if (soapSelectionRect.contains(mouseFloat)) {
                playPauseButton.setFillColor(sf::Color::White);
                playPauseClickable = false;
                soapSelection.setFillColor(sf::Color::Yellow);
                soapSelectionClickable = true;
                sanitizerSelection.setFillColor(sf::Color::White);
                sanitizerSelectionClickable = false;
            }
            else if (sanitizerSelectionRect.contains(mouseFloat)) {
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
                towerIndicatorPtr = new Tower(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), mouseFloat, "sanitizer");
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
                    Tower t(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), place, "sanitizer");
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
                towerIndicatorPtr = new Tower(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), mouseFloat, "soap");
                // Center on mouse
                towerIndicatorPtr->getRect().setOrigin(sf::Vector2f(game_map::SIDE_FLT / 2.f, game_map::SIDE_FLT / 2.f));
                towerIndicatorPtr->getRect().setPosition(mouseFloat);
                // Set textures, color
                towerIndicatorPtr->getRect().setTexture(&soapTexture);
                towerIndicatorPtr->getRect().setTextureRect(soapAnimation.uvRect);
                towerIndicatorPtr->getRect().setFillColor(sf::Color::Green);

                sf::Vector2f place = checkTowerPlacement(mouseFloat, &gameMap, towerVec);
                if (placeTowerConfirmed) {
                    // Add the tower to the towerVec
                    Tower t(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), place, "soap");
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

            //create gap between bullets
            if (bulletGeneration % 200 ==
                    0) { //adjust the mod # higher to slow production of bullets, lower the mod # to increase bullet production
                isFiring = true;
            }
            ++bulletGeneration;

            //squirtSound.setLoop(false);

            //make sure you only play the sound once (turn a bool variable on if a type is shooting and play the sound only once at the end of the for loop)
            //generate bullets (squirt dimensions: 17x4, bubble dimensions: 75x33)
            if (isFiring == true) {
                //Bullet newBullet(sf::Vector2f(75, 33), tower.getX(), tower.getY(), enemy.getX(), enemy.getY()); //shoot at first enemy, pass in tower for starting position
                //newBullet.getBullet().setTexture(&bubbleTexture);
                //bulletVec.push_back(newBullet);
                //isFiring = false;
                //squirtSound.play();
                //squirtSound.stop();
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
        if (drawCovid) {
            window.draw(covid);
        }

        //draw bullets to window
        for (unsigned i = 0; i < bulletVec.size(); i++) {
            bulletVec[i].draw(window);
            bulletVec[i].fire();
        }


        //draw all objects to window
        for (unsigned i = 0; i < towerVec.size(); i++) {
            towerVec[i].draw(window);
        }
        if (towerIndicatorPtr) {
            towerIndicatorPtr->draw(window);
        }
        window.draw(playPauseButton);
        window.draw(sanitizerSelection);
        window.draw(soapSelection);
        window.display();

    }
    // Cleaning up allocated memory
    if (towerIndicatorPtr) {
        delete towerIndicatorPtr;
        towerIndicatorPtr = nullptr;
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



//maybe for the different waves, once the enemy queue reaches 0, we enter a while loop and until the player presses start, after the player presses start
//we load the enemies again and continue (keep track of the waves)



//ask the group how to make the firing of the bullets more consistent
