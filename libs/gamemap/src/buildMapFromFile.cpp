#include "buildMapFromFile.h"
#include "Cell.h"
#include "Path.h"
#include "CellEnum.h"
#include <iostream>
#include <fstream>

// gamemap::buildMapFromFile
// params:
//   file_name - the file name
// returns:
//   a pointer to the dynamically allocated Map
Map* gamemap::buildMapFromFile(const std::string& file_name) {
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
