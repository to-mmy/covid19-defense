// make menu class with 3 buttons

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;

// header files
#include "Gamev2.h"

// add resouce path
string ResourcePath = "/Users/fkk/Desktop/resources/";


const int  NUM_OF_MENU_CHOICES = 3;

//////////////menu class /////////////
class Menu
{

private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text menu[NUM_OF_MENU_CHOICES];

public:
    Menu(float width, float height);
    ~Menu();

    void draw(sf::RenderWindow &window);
    void MoveUp();
    void MoveDown();
    int GetPressedItem() { return selectedItemIndex; }
    void displayWelcome(sf::RenderWindow& window);
};

Menu::Menu(float width, float height){
    // load music




    // font
    if (!font.loadFromFile(ResourcePath + "MYBLS___.TTF")){
        std::cout << "Can't find font" << std::endl;
    }

    menu[0].setFont(font);
    menu[0].setFillColor(sf::Color::Red);
    menu[0].setString("Play");
    menu[0].setStyle(sf::Text::Bold);
    menu[0].setPosition(sf::Vector2f(width / 10, 100 + height / (NUM_OF_MENU_CHOICES + 10) * 1));

    menu[1].setFont(font);
    menu[1].setFillColor(sf::Color::Black);
    menu[1].setString("Options");
    menu[1].setStyle(sf::Text::Bold);
    menu[1].setPosition(sf::Vector2f(width / 10, 100+ height / (NUM_OF_MENU_CHOICES + 10) * 2));

    menu[2].setFont(font);
    menu[2].setFillColor(sf::Color::Black);
    menu[2].setString("Exit");
    menu[2].setStyle(sf::Text::Bold);
    menu[2].setPosition(sf::Vector2f(width / 10, 100 + height / (NUM_OF_MENU_CHOICES + 10) * 3));

    selectedItemIndex = 0;
}


Menu::~Menu(){
}

void Menu::draw(sf::RenderWindow &window){
    for (int i = 0; i < NUM_OF_MENU_CHOICES; i++){
        window.draw(menu[i]);
    }
}

void Menu::MoveUp(){
    if (selectedItemIndex - 1 >= 0){
        menu[selectedItemIndex].setFillColor(sf::Color::Black);
        selectedItemIndex--;
        menu[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::MoveDown(){
    if (selectedItemIndex + 1 < NUM_OF_MENU_CHOICES){
        menu[selectedItemIndex].setFillColor(sf::Color::Black);
        selectedItemIndex++;
        menu[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::displayWelcome(sf::RenderWindow &window) {
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



///////// main /////////////
int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "*****TEST VERSION ****");

    // background
    sf::Vector2f BackgroundSize;
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::Texture backgroundTexture;
    BackgroundSize = background.getSize();
    backgroundTexture.loadFromFile(ResourcePath + "background3.jpg");
    background.setTexture(&backgroundTexture);


    // menu
    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen()){
        sf::Event event;

        while (window.pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed){
                    // key gets pressed
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                            menu.MoveUp();
                            break;

                        case sf::Keyboard::Down:
                            menu.MoveDown();
                            break;

                        case sf::Keyboard::Return:
                            switch (menu.GetPressedItem()) {
                                case 0:
                                    //for test
                                    std::cout << "Play button has been pressed" << std::endl;

                                    // display the introduction
                                    menu.displayWelcome(window);

                                    // start  game
                                    //



                                    break;
                                case 1:
                                    // for test
                                    std::cout << "Option button has been pressed" << std::endl;

                                    // others....  highScores list



                                    break;
                                case 2:
                                    window.close();
                                    break;
                            }
                        default:
                            break;
                    }
                }
        }

        window.clear();
        window.draw(background);
        menu.draw(window);
        window.display();
    }
    return 0;
}




