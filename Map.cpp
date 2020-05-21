#include "Map.hpp"
#include "Ground.hpp"

Map::Map(unsigned size) : side_length(size), start_coords(0,0), exit_coords(size, size) {
    cells = new Cell**[side_length];
    for (unsigned i = 0; i < side_length; ++i) {
        cells[i] = new Cell*[side_length];
        for (unsigned j = 0; j < side_length; ++j) {
            // fill it with Ground
            cells[i][j] = new Ground(i,j);
        }
    }
    //exit_coords = { side_length, side_length }; // invalid sentinel value, will need to be set
}

Map::~Map() {
    for (unsigned i = 0; i < side_length; ++i) {
        for (unsigned j = 0; j < side_length; ++j) {
            delete cells[i][j];
        }
        delete[] cells[i];
    }
    delete[] cells;
}
