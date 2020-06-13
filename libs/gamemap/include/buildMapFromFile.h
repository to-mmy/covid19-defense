#ifndef BUILDMAPFROMFILE_H
#define BUILDMAPFROMFILE_H

#include "Map.h"
#include <string>

// function gamemap::buildMapFromFile
// params:
//   file_name - the file name
// returns:
//   a pointer to the dynamically allocated Map
namespace gamemap{
Map* buildMapFromFile(const std::string& file_name);
}

#endif // BUILDMAPFROMFILE_H
