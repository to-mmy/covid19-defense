//
// Created by Kuo Fu on 2020-06-14.
//

#ifndef FINALPROJECT_HIGHSCORESCREEN_H
#define FINALPROJECT_HIGHSCORESCREEN_H
#include "SFML/Graphics.hpp"
#include<string>

const string ResourcePaths = "/Users/fkk/Desktop/resources/";

const int numScores = 10;
class HighscoreScreen {
private:
    sf::Font hsFont;
    sf::Text hsName;
    sf::Text exitButton;
    sf::Text scores[numScores];
public:
    HighscoreScreen(sf::RenderWindow &window);
    ~HighscoreScreen(){};
    void draw(sf::RenderWindow& window);
};
HighscoreScreen::HighscoreScreen(sf::RenderWindow &window) {
    if (!hsFont.loadFromFile(ResourcePath + "Jingle Bells.ttf"))
    {
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
    exitButton.setPosition(((window.getSize().x - hsName.getGlobalBounds().width) / 2.0), (window.getSize().y - 100));
}
void HighscoreScreen::draw(sf::RenderWindow & window)
{

    std::ifstream fin(ResourcePath + "highscore.txt");
    std::string line;
    if(!fin){
        cerr << "Error opening the fin highscore.txt" << endl;
        exit(1);
    }

    for (int i = 0; i < numScores; i++)
    {
        std::getline(fin, line);

        scores[i].setString(line);
        scores[i].setFont(hsFont);
        scores[i].setCharacterSize(30);
        scores[i].setFillColor(sf::Color::Red);
        scores[i].setPosition(((window.getSize().x) / 4.0), (100.0 + (i*30.0)));
    }
    fin.close();


    bool exitFlag = false;
    sf::Vector2f mouse;
    sf::Event event;
    while (!exitFlag)
    {
        while (window.pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:
                    exitFlag = true;
                    break;
                default: break;
            }
        }
        mouse = sf::Vector2f(sf::Mouse::getPosition(window));
        if (exitButton.getGlobalBounds().contains(mouse))
        {
            exitButton.setFillColor(sf::Color::Red);
            if ((sf::Mouse::isButtonPressed(sf::Mouse::Left)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                exitFlag = true;
            }
        }else{
            exitButton.setFillColor(sf::Color::White);
        }

        window.clear();

        for (int i = 0; i < 10; i++)
        {
            window.draw(scores[i]);
        }
        window.draw(hsName);
        window.draw(exitButton);
        window.display();
    }
}



#endif //FINALPROJECT_HIGHSCORESCREEN_H