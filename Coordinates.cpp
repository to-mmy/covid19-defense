#include "Coordinates.hpp"
#include <string>
#include <iostream>

void Coordinates::setCoords(const std::string& ordered_pair) {
    // ordered_pair of the form "<row#>,<col#>"
    std::string coord1(ordered_pair, 0, ordered_pair.find_first_not_of("1234567890"));
    std::string coord2(ordered_pair, ordered_pair.find_first_not_of("1234567890") + 1);
    //std::cout << coord1 << ' ' << coord2 << std::endl;
    row = std::stoi(coord1);
    col = std::stoi(coord2);
}
