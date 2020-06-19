//
// Created by Kuo Fu on 2020-06-18.
//
#include "mainMenuScreen.h"
#include <SFML/Audio.hpp>
#include <sstream>
#include <iostream>
#include <fstream>


MainMenuScreen::MainMenuScreen(float width, float height) {


    // font
    if (!font.loadFromFile(menu::RESOURCE_PATH + "MYBLS___.TTF")) {
        std::cout << "Can't find font" << std::endl;
    }
    // display the menu options
    playButton.setFont(font);
    playButton.setFillColor(sf::Color::Red);
    playButton.setString("Play");
    playButton.setStyle(sf::Text::Bold);
    playButton.setPosition(sf::Vector2f(width / 10,
                                        100 + height / (menu::NUM_OF_MENU_CHOICES + 10) * 1));

    scoreButton.setFont(font);
    scoreButton.setFillColor(sf::Color::Black);
    scoreButton.setString("High Score");
    scoreButton.setStyle(sf::Text::Bold);
    scoreButton.setPosition(sf::Vector2f(width / 10,
                                         100 + height / (menu::NUM_OF_MENU_CHOICES + 10) * 2));

    exitButton.setFont(font);
    exitButton.setFillColor(sf::Color::Black);
    exitButton.setString("Exit");
    exitButton.setStyle(sf::Text::Bold);
    exitButton.setPosition(sf::Vector2f(width / 10,
                                        100 + height / (menu::NUM_OF_MENU_CHOICES + 10) * 3));

    selectedItemIndex = 0;
}


MainMenuScreen::~MainMenuScreen() {
}

void MainMenuScreen::draw(sf::RenderWindow& window) {
    window.draw(playButton);
    window.draw(scoreButton);
    window.draw(exitButton);
}

std::string MainMenuScreen::displayWelcome(sf::RenderWindow& window) {
    std::string textFromFile, buffer;
    std::ifstream fin (menu::RESOURCE_PATH + "welcome.txt");
    if (!fin) {
        std::cout << "Cannot open welcome file." << std::endl;
    }
    while (getline(fin, buffer))
        textFromFile += buffer += '\n';
    std::ostringstream sout;
    sout << std::endl;
    textFromFile += sout.str();
    textFromFile += "Please type your name and press Enter ===> ";
    fin.close();

    // Text
    sf::Font font;
    if (!font.loadFromFile(menu::RESOURCE_PATH + "Jingle Bells.ttf")) {
        std::cout << "Can't find font" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    text.setString(textFromFile.c_str());
    text.setCharacterSize(28);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(50.0f, 50.0f);

    fin.close();

    std::string name = "";
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::TextEntered:
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 13 || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) { // return key
                        window.clear();
                        return name; // finished entering name
                    } else
                        if (event.text.unicode == 8) { // backspace
                            if (name.size() > 0)
                                name.resize(name.size() - 1);
                        } else {
                            name += static_cast<char>(event.text.unicode);
//                            std::cout << "\t\teach letter:" << name << ";;\n";
                        }
                }
                break;
            case sf::Event::Closed:
                window.close();
                break;
            default:
                ;
            }
        }
        window.clear();
        text.setString((textFromFile + name).c_str());
        window.draw(text);
        window.display();
    }
    return name;
}

int MainMenuScreen::displayMenu(sf::RenderWindow& window, std::string& playerName) {
    sf::Vector2f BackgroundSize;
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::Texture backgroundTexture;
    BackgroundSize = background.getSize();
    backgroundTexture.loadFromFile(menu::RESOURCE_PATH + "background3.jpg");
    background.setTexture(&backgroundTexture);

    selectedItemIndex = -1;
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    if (playButton.getGlobalBounds().contains(mouse)) {
        playButton.setFillColor(sf::Color::Red);
        if(sf::Mouse::isButtonPressed((sf::Mouse::Left))) {
            selectedItemIndex = 0;
//            displayWelcome(window);

/////////////change to welcome method
            playerName = displayWelcome(window);
        }
    } else
        playButton.setFillColor(sf::Color::Black);

    if (scoreButton.getGlobalBounds().contains(mouse)) {
        scoreButton.setFillColor(sf::Color::Red);
        if(sf::Mouse::isButtonPressed((sf::Mouse::Left))) {
            selectedItemIndex = 1;
        }
    } else
        scoreButton.setFillColor(sf::Color::Black);

    if (exitButton.getGlobalBounds().contains(mouse)) {
        exitButton.setFillColor(sf::Color::Red);
        if(sf::Mouse::isButtonPressed((sf::Mouse::Left))) {
            selectedItemIndex = 2;
            window.close();
        }
    } else
        exitButton.setFillColor(sf::Color::Black);


    window.clear();
    window.draw(background);
    draw(window);
    window.display();

    return selectedItemIndex;
}
