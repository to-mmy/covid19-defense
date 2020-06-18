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
#include "mainMenuScreen.h"
#include "HighscoreScreen.h"
#include "GameOverScreen.h"
#include "HighScores.h"
// add resouce path
//string ResourcePath = "/Users/fkk/Desktop/resources/";



///////// main /////////////
int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "*****TEST VERSION ****");

    // menu
    MainMenuScreen menu(window.getSize().x, window.getSize().y);
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                default: break;
            }
        }

        int choice = menu.displayMenu(window);
        if(choice == 0){ // play
            // start game
            // call game
                // during game, if die,
                // 1. save the score into the highscore list
                // 2. call gameoverscreen, show the player score
                        // press back to main menu
                GameOverScreen gameOverScreen(window);
                gameOverScreen.displayEnd(window);

        } else if (choice == 1){ // high score
            HighscoreScreen hSScreen(window);
            hSScreen.draw(window);
        } else if (choice == 2) { // exit
            window.close();
        }
    }

    return 0;
}




