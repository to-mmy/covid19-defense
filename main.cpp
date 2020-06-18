#include "GameMap.h"
#include "Cell.h"
#include "CellEnum.h"
#include "Path.h"
#include "Ground.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

# include "corona.h"

sf::Vector2f normalize(const sf::Vector2f& vec);

int main()
{
    // sample main for using Map, Cell, Path, Ground, CellEnum

    // First we build a Map with a path as follows
    /*
     01234567
    0oooooooo
    1xxoxxxoo
    2oxoxoxxo
    3oxoxxoxx
    4oxxoxxox
    5ooxooxox
    6ooxxxxox
    7ooooooox
    */
    // text file for the above looks like this (each pair is row,col):
    /*
    8 size
    28 path length
    1,0
    1,1
    2,1
    3,1
    4,1
    4,2
    5,2
    6,2
    6,3
    6,4
    6,5
    5,5
    4,5
    4,4
    3,4
    3,3
    2,3
    1,3
    1,4
    1,5
    2,5
    2,6
    3,6
    3,7
    4,7
    5,7
    6,7
    7,7
    */

    const sf::Vector2f GAME_MAP_ORIGIN = sf::Vector2f(0.f, 0.f);

    GameMap gameMap(std::string("map_1_data"), GAME_MAP_ORIGIN);

    // Reduce function calls
    unsigned sideLen = gameMap.getSideLength();
    // The following creates a window just big enough to hold the map, not the menu
    sf::RenderWindow window(sf::VideoMode(sideLen * game_map::SIDE_PIX,
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
            switch(e) {
            case 1:
                std::cout << "Error opening file " << covidTextureFile << std::endl;
            default:
                std::cout << "Enter the full file path for the ground texture file: ";
                std::getline(std::cin, covidTextureFile);
            }
        }
    }

/*
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
    //covid.setPosition(getCellPositionFromCoordinates(pathPtr->getNextCoords(), SIDE_FLT));

*/

/**/
    // Create coronavirus object
    sf::Vector2u startCoords = gameMap.getStartCoords();
    Path* pathPtr = dynamic_cast<Path*>(gameMap.getCells()[startCoords.x][startCoords.y]);

    Corona coronavirus(covidTexture, &gameMap, GAME_MAP_ORIGIN, pathPtr);

    // Render loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (coronavirus.getDrawCovid()) {
            // Check if the covid has made it to next path
            coronavirus.setDistanceToDestination( sf::Vector2f(abs(coronavirus.getCovidDestination().x - coronavirus.getSprite().getPosition().x),
                                                 abs(coronavirus.getCovidDestination().y - coronavirus.getSprite().getPosition().y)
                                                 ));
            if (std::abs(coronavirus.getDistanceToDestination().x) < 1.f
                && std::abs(coronavirus.getDistanceToDestination().y) < 1.f) {
                coronavirus.setCovidCoords(pathPtr->getNextCoords());
                pathPtr = dynamic_cast<Path*>(gameMap.getCells()[coronavirus.getCovidCoords().x][coronavirus.getCovidCoords().y]);
                coronavirus.setCovidDestination(pathPtr->getNextPosition());
                // Check if covid has made it to the end
                if (pathPtr->getNextCoords() == gameMap.getExitCoords()) {
                    coronavirus.setDrawCovid( false);
                }
            }


            // Move the covid
            coronavirus.setCovidDirection(normalize(coronavirus.getCovidDestination() - coronavirus.getSprite().getPosition()));
            coronavirus.getSprite().move(coronavirus.getMoveSpeed() * coronavirus.getCovidDirection());
        }



        // Draw
        window.clear();
        gameMap.draw(window);
        if (coronavirus.getDrawCovid()) {
            window.draw(coronavirus.getSprite());
        }
        window.display();
    }

    return 0;
}

sf::Vector2f normalize(const sf::Vector2f& vec) {
    float magnitude = sqrt((vec.x * vec.x) + (vec.y * vec.y));
    if (magnitude == 0.f) {
        return vec;
    }
    return sf::Vector2f(vec.x / magnitude, vec.y / magnitude);

}
