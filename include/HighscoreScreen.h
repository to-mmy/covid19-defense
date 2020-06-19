//
// Created by Kuo Fu on 2020-06-14.
//

#ifndef HIGHSCORESCREEN_H
#define HIGHSCORESCREEN_H
#include <SFML/Graphics.hpp>
#include <string>

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
    ~HighscoreScreen() = default;
    int draw(sf::RenderWindow& window);
};




#endif // HIGHSCORESCREEN_H
