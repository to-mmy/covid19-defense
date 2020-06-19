//
// Created by Kuo Fu on 2020-06-15.
//

#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include "PlayerData.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>


namespace menu {
extern const std::string RESOURCE_PATH;
}

class GameOverScreen {
private:
    sf::Text backButton;
    sf::Text title;
    sf::Font font;
    int selectedItemIndex;
public:
    GameOverScreen(sf::RenderWindow& window);
    ~GameOverScreen();
    void draw(sf::RenderWindow& window);
    void displayEnd(sf::RenderWindow& window, const PlayerData& playerData);
};



#endif // GAMEOVERSCREEN_H
