//
// Created by Kuo Fu on 2020-06-15.
//

#ifndef FINALPROJECT_GAMEOVERSCREEN_H
#define FINALPROJECT_GAMEOVERSCREEN_H
#include "SFML/Graphics.hpp"
//#include "SFML/Audio.hpp"
#include "PlayerData.h"

#include <iostream>
#include <string>


namespace menu {
extern const std::string RESOURCE_PATH;
}

class GameOverScreen {
    sf::Text backButton;
    sf::Text title;
    sf::Font font;
    int selectedItemIndex;

public:
    GameOverScreen(sf::RenderWindow &window);
    ~GameOverScreen();
    void draw(sf::RenderWindow &window);
    void displayEnd(sf::RenderWindow &window,const PlayerData &playerData);
};



#endif //FINALPROJECT_GAMEOVERSCREEN_H
