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

sf::Vector2f getCellPositionFromCoordinates(const sf::Vector2u& coords, const float& sideFlt);
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

    GameMap gameMap(std::string("map_1_data"));

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
    //covid.setPosition(getCellPositionFromCoordinates(pathPtr->getNextCoords(), SIDE_FLT));


    // Render loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

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



        // Draw
        window.clear();
        gameMap.draw(window);
        if (drawCovid) {
            window.draw(covid);
        }
        window.display();
    }

    return 0;
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
