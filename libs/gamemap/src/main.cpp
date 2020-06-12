#include "Map.h"
#include "Cell.h"
#include "CellEnum.h"
#include "Path.h"
#include "Ground.h"
#include "buildMapFromFile.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

sf::Vector2f getCellPositionFromCoordinates(const Coordinates& coords, const float& side_flt);
sf::Vector2f normalize(const sf::Vector2f& vec);

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

    Map* game_map = gamemap::buildMapFromFile(std::string("map.txt"));

    // get the 2D Cell* array
    Cell*** map_cells = game_map->getCells();

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
            if (map_cells[i][j]->getCellType() == CellEnum::GROUND) {
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

    Path* path_ptr = dynamic_cast<Path*>(map_cells[circle_coords.row][circle_coords.col]);

    const float circle_speed = 0.05f;

    sf::Vector2f circle_direction;
    sf::Vector2f circle_destination = getCellPositionFromCoordinates(path_ptr->getNextCoords(), side_flt);
    sf::Vector2f distance_to_destination(std::abs(circle_destination.x - circle.getPosition().x), std::abs(circle_destination.y - circle.getPosition().y));

    bool draw_circle = true;
    //circle.setPosition(getCellPositionFromCoordinates(path_ptr->getNextCoords(), side_flt));


    // Render loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (draw_circle) {
            // Check if the circle has made it to next path
            distance_to_destination = sf::Vector2f(abs(circle_destination.x - circle.getPosition().x), abs(circle_destination.y - circle.getPosition().y));
            if (std::abs(distance_to_destination.x) < 1.f && std::abs(distance_to_destination.y) < 1.f) {
                circle_coords = path_ptr->getNextCoords();
                path_ptr = dynamic_cast<Path*>(map_cells[circle_coords.row][circle_coords.col]);
                circle_destination = getCellPositionFromCoordinates(path_ptr->getNextCoords(), side_flt);
                // Check if circle has made it to the end
                if (path_ptr->getNextCoords() == game_map->getExitCoords()) {
                    draw_circle = false;
                }
            }


            // Move the circle
            circle_direction = normalize(circle_destination - circle.getPosition());
            circle.move(circle_speed * circle_direction);
        }



        // Draw
        window.clear();
        for (unsigned i = 0; i < side_len; ++i) {
            for (unsigned j = 0; j < side_len; ++j) {
                window.draw(cell_shape_array[i][j]);
            }
        }
        if (draw_circle) {
            window.draw(circle);
        }
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

sf::Vector2f getCellPositionFromCoordinates(const Coordinates& coords, const float& side_flt) {
    return sf::Vector2f(static_cast<float>(coords.col) * side_flt, static_cast<float>(coords.row) * side_flt);
}

sf::Vector2f normalize(const sf::Vector2f& vec) {
    float magnitude = sqrt((vec.x * vec.x) + (vec.y * vec.y));
    if (magnitude == 0.f) {
        return vec;
    }
    return sf::Vector2f(vec.x / magnitude, vec.y / magnitude);

}
