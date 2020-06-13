#include "Coordinates.h"
#include <string>

void Coordinates::setCoords(const std::string& ordered_pair) {
    // ordered_pair of the form "<row#>,<col#>"
    std::string coord1(ordered_pair, 0, ordered_pair.find_first_not_of("1234567890"));
    std::string coord2(ordered_pair, ordered_pair.find_first_not_of("1234567890") + 1);
    row = std::stoi(coord1);
    col = std::stoi(coord2);
}
