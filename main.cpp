#include "Map.hpp"
#include "Cell.hpp"
#include "CellEnum.hpp"
#include "Path.hpp"
#include "Ground.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

Map* buildMapFromFile(const std::string& file_name);
sf::Vector2f getCellPositionFromCoordinates(const Coordinates& coords, const float& side_flt);

int main()
{
    // sample main for using Map, Cell, Path, Ground, CellEnum, Coordinates

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

    Map* game_map = buildMapFromFile(std::string("map.txt"));

    // Make our SFML window
    const unsigned side_pix = 50; // the length of a side of a cell of the map in pixels
    const float side_flt = 50.f;
    // Reduce function calls
    unsigned side_len = game_map->getSideLength();
    // The following creates a window just big enough to hold the map, not the menu
    sf::RenderWindow window(sf::VideoMode(side_len * side_pix, side_len * side_pix), "Example map");

    // 2D array of RectangleShapes to display cells.
    sf::RectangleShape** cell_shape_array = new sf::RectangleShape*[side_len];
    for (unsigned i = 0; i < side_len; ++i) {
        cell_shape_array[i] = new sf::RectangleShape[side_len];
    }

    // Set values for these Rectangles
    for (unsigned i = 0; i < side_len; ++i) {
        for (unsigned j = 0; j < side_len; ++j) {
            cell_shape_array[i][j].setSize(sf::Vector2f(side_flt, side_flt));
            cell_shape_array[i][j].setPosition(static_cast<float>(j) * side_flt, static_cast<float>(i) * side_flt);
            if (game_map->getCells()[i][j]->getCellType() == CellEnum::GROUND) {
                cell_shape_array[i][j].setFillColor(sf::Color(0, 255, 0));
            }
            else {
                cell_shape_array[i][j].setFillColor(sf::Color(255, 0, 0));
            }
        }
    }

    // Enemy sprite
    sf::CircleShape circle(side_flt / 2.f);
    Coordinates circle_coords = game_map->getStartCoords();
    circle.setPosition(getCellPositionFromCoordinates(circle_coords, side_flt));

    // Render loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for (unsigned i = 0; i < side_len; ++i) {
            for (unsigned j = 0; j < side_len; ++j) {
                window.draw(cell_shape_array[i][j]);
            }
        }
        window.draw(circle);
        window.display();
    }

    // Cleanup
    for (unsigned i = 0; i < side_len; ++i) {
        delete cell_shape_array[i];
    }
    delete [] cell_shape_array;
    cell_shape_array = nullptr;
    delete game_map;
    game_map = nullptr;


    return 0;
}

// buildMapFromFile
// params:
//   file_name - the file name
// returns:
//   a pointer to the dynamically allocated Map
Map* buildMapFromFile(const std::string& file_name) {
    // Open the file
    bool valid_file = false;
    std::string input = file_name;
    std::ifstream fin;
    while (!valid_file) {
        try {
            fin.open(input);
            if (!fin) {
                throw 1;
            }
            else {
                valid_file = true;
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

    // Create the Map object
    unsigned temp = 0;
    std::getline(fin, input);
    input.resize(input.find_first_not_of("1234567890"));
    //std::cout << input << std::endl;
    temp = std::stoi(input);
    Map* new_map = new Map(temp);

    // Fill Path cells
    std::getline(fin, input);
    input.resize(input.find_first_not_of("1234567890"));
    //std::cout << input << std::endl;
    temp = std::stoi(input);
    //std::cout << temp << '\n';
    std::getline(fin, input);
    Coordinates path_coords;
    path_coords.setCoords(input);
    new_map->setStartCoords(path_coords);
    Coordinates next_path_coords;
    for (unsigned i = 0; i < temp - 1; ++i) {
        //std::cout << i << '\n';
        std::getline(fin, input);
        next_path_coords.setCoords(input);
        // Map constructor fills it with Ground so we need to delete that Cell
        delete new_map->getCells()[path_coords.row][path_coords.col];
        new_map->getCells()[path_coords.row][path_coords.col] = new Path(path_coords, next_path_coords);
        path_coords = next_path_coords;
    }
    new_map->getCells()[path_coords.row][path_coords.col] = new Path(path_coords, new_map->getExitCoords());

    // Text output (gcc -DDEBUG)
    #ifdef DEBUG
    for (unsigned i = 0; i < new_map->getSideLength(); ++i) {
        for (unsigned j = 0; j < new_map->getSideLength(); ++j) {
            //std::cout << "In Map at " << i << ',' << j << '\n';
            //std::cout << "Cell Type is " << static_cast<int>(new_map->getCells()[i][j]->getCellType()) << '\n';
            // getCells()[0][0] is special because it's not empty but it also may not be a Path
            if (new_map->getCells()[i][j]->getCellType() == CellEnum::GROUND) {
                std::cout << " o";
            }
            else {
                std::cout << " x";
            }
            if (j == new_map->getSideLength() - 1) {
                std::cout << '\n';
            }
        }
    }
    #endif
    fin.close();
    return new_map;

}

sf::Vector2f getCellPositionFromCoordinates(const Coordinates& coords, const float& side_flt) {
    return sf::Vector2f(static_cast<float>(coords.col) * side_flt, static_cast<float>(coords.row) * side_flt);
}
