#include "GameMap.h"
#include "Ground.h"
#include "Path.h"
#include <iostream>
#include <fstream>

namespace game_map {
const unsigned SIDE_PIX = 96;
const float SIDE_FLT = static_cast<float>(SIDE_PIX);
const sf::Vector2f SPRITE_SCALE(SIDE_FLT / 50.f, SIDE_FLT / 50.f); // 50 is size of
                                                                        // texture file
const std::string RESOURCE_PATH = "resources\\";
}

// Constructor for GameMap, default for size is 8 as stated in GameMap.h
// parameter fileName specifies a text file of the following format:
// For a GameMap with a path as follows
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
// text file for the above looks like this (each pair is (row,col), i.e. (y,x)):
// First two lines are texture files
// 3rd and 4th lines self-explanatory
/*
city_block_1.jpg
city_street_1.jpg
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
// Builds a dynamically-allocated 2-D array of Cell pointers. Each cell holds its location in the
// array as its coordinates. Cells can be either Ground or Path, and polymorphism is enabled
// exitCoords is set to be (sideLength, sideLength) as these coordinates cannot be the coordinates
// of any Cell.
// Default argument for drOrigin is (0.f, 0.f). drawOrigin is used to specify where to start
// drawing the map from in the RenderWindow
GameMap::GameMap(const std::string& mapFileName, const sf::Vector2f& drOrigin) : drawOrigin(drOrigin) {
    // Open the file
    bool validFile = false;
    std::string input(game_map::RESOURCE_PATH + mapFileName);
    std::ifstream fin;

    #ifdef DEBUG
    std::cout << "Opening file " << input << " ...\n";
    #endif // DEBUG

    while (!validFile) {
        try {
            fin.open(input);
            if (!fin) {
                throw 1;
            }
            else {
                validFile = true;
            }
        }
        catch (int e) {
            switch(e) {
            case 1:
                std::cout << "Error opening file " << input << std::endl;
            default:
                std::cout << "Enter the full file path for the map file: ";
                std::getline(std::cin, input);
            }
            fin.clear();
        }
    }

    // First load the textures

    // First line is ground texture file
    std::getline(fin, input);
    bool textureLoadSuccess = false;
    std::string groundTextureFile(game_map::RESOURCE_PATH + input);

    #ifdef DEBUG
    std::cout << "Opening file " << groundTextureFile << " ...\n";
    #endif // DEBUG

    while (!textureLoadSuccess) {
        try {
            if (groundTexture.loadFromFile(groundTextureFile,
                                           sf::IntRect(0, 0,
                                                       game_map::SIDE_PIX,
                                                       game_map::SIDE_PIX)
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
                std::cout << "Error opening file " << groundTextureFile << std::endl;
            default:
                std::cout << "Enter the full file path for the ground texture file: ";
                std::getline(std::cin, groundTextureFile);
            }
        }
    }

    // Second line is path texture file
    std::getline(fin, input);
    textureLoadSuccess = false;
    std::string pathTextureFile(game_map::RESOURCE_PATH + input);

    #ifdef DEBUG
    std::cout << "Opening file " << pathTextureFile << " ...\n";
    #endif // DEBUG

    while (!textureLoadSuccess) {
        try {
            if (pathTexture.loadFromFile(pathTextureFile,
                                         sf::IntRect(0, 0, game_map::SIDE_PIX, game_map::SIDE_PIX)
                                         )) {
                textureLoadSuccess = true;
            }
            else {
                throw 1;
            }
        }
        catch (int e) {
            switch(e) {
            case 1:
                std::cout << "Error opening file " << pathTextureFile << std::endl;
            default:
                std::cout << "Enter the full file path for the ground texture file: ";
                std::getline(std::cin, pathTextureFile);
            }
        }
    }

    // Create the GameMap object
    unsigned temp = 0;
    std::getline(fin, input);
    input.resize(input.find_first_not_of("1234567890"));

    #ifdef DEBUG
    std::cout << input << std::endl;
    #endif // DEBUG

    temp = std::stoi(input);
    sideLength = temp;
    exitCoords = sf::Vector2u(sideLength, sideLength);

    // build 2-D array of Cell pointers
    cells = new Cell**[sideLength];
    sf::Vector2f tempPos;
    for (unsigned i = 0; i < sideLength; ++i) {
        cells[i] = new Cell*[sideLength];
        for (unsigned j = 0; j < sideLength; ++j) {
            // fill it with Ground
            cells[i][j] = new Ground(i,j);
            // set Sprite values
            cells[i][j]->setSpriteTexture(groundTexture);
            tempPos = drawOrigin + cells[i][j]->getPosition();
            cells[i][j]->setSpritePosition(tempPos);
            cells[i][j]->setSpriteScale(game_map::SPRITE_SCALE);
        }
    }

    // Fill Path cells
    std::getline(fin, input);
    input.resize(input.find_first_not_of("1234567890"));

    #ifdef DEBUG
    std::cout << input << std::endl;
    #endif // DEBUG

    temp = std::stoi(input);

    #ifdef DEBUG
    std::cout << temp << '\n';
    #endif // DEBUG

    std::getline(fin, input);
    sf::Vector2u pathCoords;

    // Parse input line
    std::string coord1(input, 0, input.find_first_not_of("1234567890"));
    std::string coord2(input, input.find_first_not_of("1234567890") + 1);

    pathCoords = sf::Vector2u(std::stoi(coord1), std::stoi(coord2));
    startCoords = pathCoords; // start coordinates are first coordinates in file
    sf::Vector2u nextPathCoords;
    for (unsigned i = 0; i < temp - 1; ++i) {
        //std::cout << i << '\n';
        std::getline(fin, input);
        coord1 = input.substr(0, input.find_first_not_of("1234567890"));
        coord2 = input.substr(input.find_first_not_of("1234567890") + 1);
        nextPathCoords = sf::Vector2u(std::stoi(coord1), std::stoi(coord2));
        // We initially filled cells with Ground so we need to delete that Cell
        delete cells[pathCoords.x][pathCoords.y];
        cells[pathCoords.x][pathCoords.y] = new Path(pathCoords, nextPathCoords);
        // Re-set Sprite values
        cells[pathCoords.x][pathCoords.y]->setSpriteTexture(pathTexture);
        tempPos = drawOrigin + cells[pathCoords.x][pathCoords.y]->getPosition();
        cells[pathCoords.x][pathCoords.y]->setSpritePosition(tempPos);
        cells[pathCoords.x][pathCoords.y]->setSpriteScale(game_map::SPRITE_SCALE);
        pathCoords = nextPathCoords;
    }
    // The last Path cell is set outside the loop because we set its next to exitCoords
    // Again, delete the existing Ground cell.
    delete cells[pathCoords.x][pathCoords.y];
    cells[pathCoords.x][pathCoords.y] = new Path(pathCoords, exitCoords);
    // And re-set the Sprite values
    cells[pathCoords.x][pathCoords.y]->setSpriteTexture(pathTexture);
    tempPos = drawOrigin + cells[pathCoords.x][pathCoords.y]->getPosition();
    cells[pathCoords.x][pathCoords.y]->setSpritePosition(tempPos);
    cells[pathCoords.x][pathCoords.y]->setSpriteScale(game_map::SPRITE_SCALE);

    // Text output (gcc -DDEBUG)
    #ifdef DEBUG
    for (unsigned i = 0; i < sideLength; ++i) {
        for (unsigned j = 0; j < sideLength; ++j) {
            #ifndef DEBUG_GRAPHIC
            std::cout << "In GameMap at " << i << ',' << j << '\n';
            std::cout << "Cell Type is "
                      << static_cast<int>(cells[i][j]->getCellType()) << '\n';
            #else // DEBUG_GRAPHIC defined
            // getCells()[0][0] is special because it's not empty but it also may not be a Path
            if (cells[i][j]->getCellType() == CellEnum::GROUND) {
                std::cout << " o";
            }
            else {
                std::cout << " x";
            }
            if (j == sideLength - 1) {
                std::cout << '\n';
            }
            #endif // DEBUG_GRAPHIC
        }
    }
    #endif // DEBUG
    fin.close();
}

GameMap::~GameMap() {
    for (unsigned i = 0; i < sideLength; ++i) {
        for (unsigned j = 0; j < sideLength; ++j) {
            delete cells[i][j];
            cells[i][j] = nullptr;
        }
        delete[] cells[i];
        cells[i] = nullptr;
    }
    delete[] cells;
    cells = nullptr;
}

// Draw to a RenderWindow
void GameMap::draw(sf::RenderWindow& window) const {
    for (unsigned i = 0; i < sideLength; ++i) {
        for (unsigned j = 0; j < sideLength; ++j) {
            window.draw(cells[i][j]->getSprite());
        }
    }
}
