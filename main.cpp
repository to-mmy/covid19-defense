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

            //test for the player score
            HighScoreList highScoreList;
            PlayerData p1, p2;
            p1.setScore(12345);
            p2.setScore(10000);
            p2.setName("Player 2");

            highScoreList.insertNode(p1); // insert as the descending order
            highScoreList.insertNode(p2);
            highScoreList.writeToFile(ResourcePath +"highscore.txt");

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
            choice = hSScreen.draw(window);
        } else if (choice == 2) { // exit
            window.close();
        }
    }

    return 0;
}




