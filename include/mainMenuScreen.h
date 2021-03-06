//
// Created by Kuo Fu.
//

#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include "HighscoreScreen.h"

namespace menu {
    extern const int NUM_OF_MENU_CHOICES ;
    extern const std::string RESOURCE_PATH;
}

class MainMenuScreen {
private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text playButton;
    sf::Text scoreButton;
    sf::Text exitButton;

public:
    MainMenuScreen(float width, float height);

    ~MainMenuScreen();

    void draw(sf::RenderWindow &window);

    int GetPressedItem() { return selectedItemIndex; }

    int displayMenu(sf::RenderWindow &window, std::string &playerName);
    std::string displayWelcome(sf::RenderWindow& window);

};



#endif // MAINMENUSCREEN_H
