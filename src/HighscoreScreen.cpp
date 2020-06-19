//
// Created by Kuo Fu on 2020-06-14.
//

#include "HighscoreScreen.h"
#include <iostream>
#include <fstream>

HighscoreScreen::HighscoreScreen(sf::RenderWindow& window) {
    if (!hsFont.loadFromFile(menu::RESOURCE_PATH + "Jingle Bells.ttf")) {

        std::cout << "Can't find font" << std::endl;
    }

    hsName.setFont(hsFont);
    hsName.setString("High Score");
    hsName.setCharacterSize(50);
    hsName.setFillColor(sf::Color::Yellow);
    hsName.setPosition(((window.getSize().x - hsName.getGlobalBounds().width) / 2), 10);
    exitButton.setFont(hsFont);
    exitButton.setString("Exit");
    exitButton.setCharacterSize(40);
    exitButton.setFillColor(sf::Color::Red);
    exitButton.setPosition(((window.getSize().x - hsName.getGlobalBounds().width) / 2.0),
                           (window.getSize().y - 100));
}
int HighscoreScreen::draw(sf::RenderWindow& window) {
    selectedItemIndex = -1;
    std::ifstream fin(menu::RESOURCE_PATH + "highscore.txt");
    std::string line;
    if(!fin) {
        std::cerr << "Error opening the fin highscore.txt" << std::endl;
        exit(1);
    }

    for (int i = 0; i < scores::NUM_SCORES; i++) {
        std::getline(fin, line);

        scores[i].setString(line);
        scores[i].setFont(hsFont);
        scores[i].setCharacterSize(30);
        scores[i].setFillColor(sf::Color::Red);
        scores[i].setPosition(((window.getSize().x) / 4.0), (100.0 + (i * 30.0)));
    }
    fin.close();


    bool exitFlag = false;

    sf::Vector2f mouse;
    sf::Event event;
    while (!exitFlag) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                exitFlag = true;
                selectedItemIndex = 2; // in main
                break;
            default:
                break;
            }
        }
        mouse = sf::Vector2f(sf::Mouse::getPosition(window));
        if (exitButton.getGlobalBounds().contains(mouse)) {
            exitButton.setFillColor(sf::Color::Red);
            if ((sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                exitFlag = true;
            }
        } else {
            exitButton.setFillColor(sf::Color::White);
        }

        window.clear();

        for (int i = 0; i < 10; i++) {
            window.draw(scores[i]);
        }
        window.draw(hsName);
        window.draw(exitButton);
        window.display();
    }
    return selectedItemIndex;
}
