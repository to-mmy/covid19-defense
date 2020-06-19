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

namespace States {
    class GameOverScreen {
        sf::Text backButton;
        sf::Text title;
        sf::Font font;
        int selectedItemIndex;

    public:
        GameOverScreen(sf::RenderWindow &window);
        ~GameOverScreen();
        void draw(sf::RenderWindow &window);
        void displayEnd(sf::RenderWindow &window);
    };
}
GameOverScreen::GameOverScreen(sf::RenderWindow &window)
{
    // loading music



    if (!font.loadFromFile(menu::RESOURCE_PATH + "Jingle Bells.ttf"))
    {
        std::cout << "Can't find font" << std::endl;
    }

    title.setFont(font);
    title.setString("Game Over");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(((window.getSize().x - title.getGlobalBounds().width) / 2), window.getSize().y / 4);

    backButton.setFont(font);
    backButton.setCharacterSize(40);
    backButton.setString("Back");
    backButton.setFillColor(sf::Color::Red);
    backButton.setPosition(((window.getSize().x - backButton.getGlobalBounds().width) / 2.0), (window.getSize().y - 100));

}

GameOverScreen::~GameOverScreen()
{
}

void GameOverScreen::draw(sf::RenderWindow & window)
{
    window.draw(backButton);
    window.draw(title);
}

void GameOverScreen::displayEnd(sf::RenderWindow &window) {
    bool exitFlag = false;
    sf::Vector2f mouse;
    sf::Event event;
    while (!exitFlag)
    {
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    exitFlag = true;
                    selectedItemIndex = 2;
                    break;
                default:
                    break;
            }
        }
        mouse = sf::Vector2f(sf::Mouse::getPosition(window));
        if (backButton.getGlobalBounds().contains(mouse))
        {
            backButton.setFillColor(sf::Color::Red);
            if ((sf::Mouse::isButtonPressed(sf::Mouse::Left)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                exitFlag = true;
                selectedItemIndex = 2; // exit
            }
        }else{
            backButton.setFillColor(sf::Color::White);
        }

        // below is for testing the player score
        // need to pass in the playerscore object

//        sf::Text playername;
//        sf::Text playerScore;
//
//        PlayerData playerData;
//        playerData.setName("tommy");
//        playerData.setScore(18389);
//
//        playername.setFont(font);
//        playername.setString(playerData.getName());
//        playername.setCharacterSize(40);
//        playername.setFillColor(sf::Color::Blue);
//        playername.setPosition(((window.getSize().x )/ 3), window.getSize().y / 4 + 100);
//
//        int score = playerData.getScore();
//        playerScore.setFont(font);
//        playerScore.setCharacterSize(40);
//        playerScore.setString(to_string(score));
//        playerScore.setFillColor(sf::Color::Blue);
//        playerScore.setPosition(((window.getSize().x) / 3.0 + 200), window.getSize().y / 4 + 100);
//

        window.clear();
//        window.draw(playername);
//        window.draw(playerScore);
        draw(window);
        window.display();
    }
}

#endif //FINALPROJECT_GAMEOVERSCREEN_H
