# CIS29Project
## CIS29 Project: COVID-19 Defense Game
Members:
- Co-Group Lead: Thomas Zhang (to-mmy)
- Co-Group Lead: Jonathan Ly (jtlystudent0)
- Rohan Shroti (suburbanSadness)
- kuo fu (kkfkiera)

A group project to create a game using the SFML libraries over the course of a quarter.

## Build Instructions
Tested on and can be built on:
  Windows: CodeBlocks GCC compiler, Microsoft Visual Studio
  Mac: Clang compiler
1. In src/main.cpp and libs/gamemap/src/GameMap.cpp, change the constants `menu::RESOURCE_PATH` and `game_map::RESOURCE_PATH` to suit your file directory type, i.e. either `"resources\\"` or `"resources/"`
2. Build the library gamemap (in libs/gamemap) to lib/
3. Include the following directories: libs/gamemap/include/, include/, SFML/include/
4. Link the following directories: lib/, SFML/lib/
5. Link the following libraries: gamemap, sfml-graphics, sfml-window, sfml-audio, sfml-system
   - Follow directions for your compiler here: https://www.sfml-dev.org/tutorials/2.5/#getting-started
