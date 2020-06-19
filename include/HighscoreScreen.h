//
// Created by Kuo Fu on 2020-06-14.
//

#ifndef FINALPROJECT_HIGHSCORESCREEN_H
#define FINALPROJECT_HIGHSCORESCREEN_H
#include <SFML/Graphics.hpp>
#include<string>

namespace menu {
extern const std::string RESOURCE_PATH;
}

namespace scores {
const int NUM_SCORES = 10;
}

class HighscoreScreen {
private:
    int selectedItemIndex;
    sf::Font hsFont;
    sf::Text hsName;
    sf::Text exitButton;
    sf::Text scores[scores::NUM_SCORES];
public:
    HighscoreScreen(sf::RenderWindow &window);
    ~HighscoreScreen(){};
    int draw(sf::RenderWindow& window);
};




#endif //FINALPROJECT_HIGHSCORESCREEN_H
