#ifndef COORDINATES_H_INCLUDED
#define COORDINATES_H_INCLUDED

#include <string>

struct Coordinates {
    unsigned row;
    unsigned col;
    Coordinates() = default;
    Coordinates(const Coordinates&) = default;
    Coordinates(unsigned r, unsigned c) : row(r), col(c) {}


    void setCoords(const std::string&);

    bool operator==(const Coordinates& c) {
        return row == c.row && col == c.col;
    }
    bool operator!=(const Coordinates& c) {
        return !(*this == c);
    }
};

#endif // COORDINATES_H_INCLUDED
