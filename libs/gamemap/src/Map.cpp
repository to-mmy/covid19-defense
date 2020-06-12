#include "Map.h"
#include "Ground.h"

// Constructor for Map, default for size is 8 as stated in Map.h
// Builds a dynamically-allocated 2-D array of Cell pointers, all pointing to
// Ground objects. Each cell holds its location in the array as its coordinates
Map::Map(unsigned size) : side_length(size), start_coords(0,0), exit_coords(size, size) {
    cells = new Cell**[side_length];
    for (unsigned i = 0; i < side_length; ++i) {
        cells[i] = new Cell*[side_length];
        for (unsigned j = 0; j < side_length; ++j) {
            // fill it with Ground
            cells[i][j] = new Ground(i,j);
        }
    }
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
