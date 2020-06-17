//
// Created by Kuo Fu.
//

#ifndef FINALPROJECT_MAINMENUSCREEN_H
#define FINALPROJECT_MAINMENUSCREEN_H
const int  NUM_OF_MENU_CHOICES = 3;
const string ResourcePath = "/Users/fkk/Desktop/resources/";

#include "HighscoreScreen.h"

namespace States {
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

        void displayWelcome(sf::RenderWindow &window);

        int displayMenu(sf::RenderWindow &window);
    };
}

using namespace States;
MainMenuScreen::MainMenuScreen(float width, float height){
    // load music




    // font
    if (!font.loadFromFile(ResourcePath + "MYBLS___.TTF")){
        std::cout << "Can't find font" << std::endl;
    }
    // display the menu options
    playButton.setFont(font);
    playButton.setFillColor(sf::Color::Red);
    playButton.setString("Play");
    playButton.setStyle(sf::Text::Bold);
    playButton.setPosition(sf::Vector2f(width / 10, 100 + height / (NUM_OF_MENU_CHOICES + 10) * 1));

    scoreButton.setFont(font);
    scoreButton.setFillColor(sf::Color::Black);
    scoreButton.setString("High Score");
    scoreButton.setStyle(sf::Text::Bold);
    scoreButton.setPosition(sf::Vector2f(width / 10, 100+ height / (NUM_OF_MENU_CHOICES + 10) * 2));

    exitButton.setFont(font);
    exitButton.setFillColor(sf::Color::Black);
    exitButton.setString("Exit");
    exitButton.setStyle(sf::Text::Bold);
    exitButton.setPosition(sf::Vector2f(width / 10, 100 + height / (NUM_OF_MENU_CHOICES + 10) * 3));

    selectedItemIndex = 0;
}


MainMenuScreen::~MainMenuScreen(){
}

void MainMenuScreen::draw(sf::RenderWindow &window){
    window.draw(playButton);
    window.draw(scoreButton);
    window.draw(exitButton);
}

void MainMenuScreen::displayWelcome(sf::RenderWindow &window) {
    string textFromFile, buffer;
    ifstream fin (ResourcePath + "welcome.txt");
    if (!fin)
    {
        std::cout << "Cannot open welcome file." << std::endl;
        return;
    }
    while (getline(fin,buffer))
        textFromFile += buffer += '\n';
    fin.close();

    // Text
    sf::Font font;
    if (!font.loadFromFile(ResourcePath + "Jingle Bells.ttf"))
    {
        std::cout << "Can't find font" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    text.setString(textFromFile.c_str());
    text.setCharacterSize(28);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(50.0f,50.0f);

    fin.close();

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    {
                        window.clear();
                        // start the game
                        return;
                    }
                default:
                    break;
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }
}

int MainMenuScreen::displayMenu(sf::RenderWindow &window) {
    sf::Vector2f BackgroundSize;
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::Texture backgroundTexture;
    BackgroundSize = background.getSize();
    backgroundTexture.loadFromFile(ResourcePath + "background3.jpg");
    background.setTexture(&backgroundTexture);

    selectedItemIndex = -1;
    sf::Vector2f mouse(sf::Mouse::getPosition(window));
    if (playButton.getGlobalBounds().contains(mouse)){
        playButton.setFillColor(sf::Color::Red);
        if(sf::Mouse::isButtonPressed((sf::Mouse::Left))){
            cout << "\tyeah, left clicked" << endl;
            selectedItemIndex = 0;
            displayWelcome(window);
        }
    }else
        playButton.setFillColor(sf::Color::Black);

    if (scoreButton.getGlobalBounds().contains(mouse)){
        scoreButton.setFillColor(sf::Color::Red);
        if(sf::Mouse::isButtonPressed((sf::Mouse::Left))){
            selectedItemIndex = 1;
        }
    }else
        scoreButton.setFillColor(sf::Color::Black);

    if (exitButton.getGlobalBounds().contains(mouse)){
        exitButton.setFillColor(sf::Color::Red);
        if(sf::Mouse::isButtonPressed((sf::Mouse::Left))){
            selectedItemIndex = 2;
            window.close();
        }
    }else
        exitButton.setFillColor(sf::Color::Black);


    window.clear();
    window.draw(background);
    draw(window);
    window.display();


    return selectedItemIndex;
}
#endif //FINALPROJECT_MAINMENUSCREEN_H
