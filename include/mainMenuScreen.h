//
// Created by Kuo Fu.
//

#ifndef FINALPROJECT_MAINMENUSCREEN_H
#define FINALPROJECT_MAINMENUSCREEN_H

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

//    void displayWelcome(sf::RenderWindow &window);
//    int displayMenu(sf::RenderWindow &window);
    int displayMenu(sf::RenderWindow &window, std::string &playerName);
    std::string displayWelcome(sf::RenderWindow& window);

};



#endif //FINALPROJECT_MAINMENUSCREEN_H
