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

// header files
#include "mainMenuScreen.h"
#include "HighscoreScreen.h"
#include "GameOverScreen.h"
#include "HighScores.h"
#include "GamePlayScreen.h"
// add resouce path
//string ResourcePath = "/Users/fkk/Desktop/resources/";
void readFromFile(std::string inFilename, HighScoreList &highScoreList);

///////// main /////////////
int main(){
    sf::RenderWindow window(sf::VideoMode(draw::WINDOW_WIDTH, draw::WINDOW_HEIGHT), "COVID-19 Defense");

    // menu
    MainMenuScreen menu(window.getSize().x, window.getSize().y);

    HighScoreList highScoreList;
    readFromFile(menu::RESOURCE_PATH +"highscore.txt", highScoreList);

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
            PlayerData p1, p2;
            p1.setScore(8888);
            p2.setScore(99);
            p2.setName("Player 2");

            highScoreList.insertNode(p1); // insert as the descending order
            highScoreList.insertNode(p2);
            highScoreList.writeToFile(menu::RESOURCE_PATH +"highscore.txt");


            PlayerData p3;
            p3.setName("Game Player");

            GamePlayScreen gamePlayScreen;
            gamePlayScreen.setPlayer(p3);
            gamePlayScreen.draw(window);

            p3 = gamePlayScreen.getPlayer();
            highScoreList.insertNode(p3);
            highScoreList.writeToFile(menu::RESOURCE_PATH +"highscore.txt");

            if (gamePlayScreen.getWindowClosed()) {
                window.close();
            }
            else if (gamePlayScreen.getGameOver()) {
                GameOverScreen gameOverScreen(window);
//                gameOverScreen.displayEnd(window);
                // change the displayEnd method, to display the current player score at the end
                gameOverScreen.displayEnd(window, p3); // has been tested, work

            }

        } else if (choice == 1){ // high score
            HighscoreScreen hSScreen(window);
            choice = hSScreen.draw(window);
        } else if (choice == 2) { // exit
            window.close();
        }
    }

    return 0;
}

void loadTexture(sf::Texture& texture, const std::string& fileName) {
    bool validFile = false;
    std::string input = fileName;
    while (!validFile) {
        try {
            if (!texture.loadFromFile(input)) {
                throw 1;
            }
            else {
                validFile = true;
            }
        }
        catch (int e) {
            switch(e) {
            case 1:
                std::cout << "Error opening file " << input << std::endl;
            default:
                std::cout << "Enter the full file path for the texture file: ";
                std::getline(std::cin, input);
            }
        }
    }
}

void loadSound(sf::SoundBuffer& soundBuffer, const std::string& fileName) {
    bool validFile = false;
    std::string input = fileName;
    while (!validFile) {
        try {
            if (!soundBuffer.loadFromFile(input)) {
                throw 1;
            }
            else {
                validFile = true;
            }
        }
        catch (int e) {
            switch(e) {
            case 1:
                std::cout << "Error opening file " << input << std::endl;
            default:
                std::cout << "Enter the full file path for the sound file: ";
                std::getline(std::cin, input);
            }
        }
    }
}

void loadFont(sf::Font& font, const std::string& fileName) {
    bool validFile = false;
    std::string input = fileName;
    while (!validFile) {
        try {
            if (!font.loadFromFile(input)) {
                throw 1;
            }
            else {
                validFile = true;
            }
        }
        catch (int e) {
            switch(e) {
            case 1:
                std::cout << "Error opening file " << input << std::endl;
            default:
                std::cout << "Enter the full file path for the font file: ";
                std::getline(std::cin, input);
            }
        }
    }
}
// add method 6/18
// to make sure each time when reopening the game, create/update the highscoreList
// has been tested, work
void readFromFile(std::string inFilename, HighScoreList &highScoreList) {
    std::ifstream infile;
    char name[13];
    int score;

    infile.open(inFilename);
    if(!infile){
        std::cout << "Error happened to open the input file!" << std::endl;
        exit(EXIT_FAILURE);
    }



    while(!infile.eof()){
        infile.read(name, 12);
        infile >> score;
        infile.ignore();

        // trim trailing spaces
        name[12] = 0;
        // right trim extra white space
        size_t size = strlen(name);
        for (int i = size - 1; i > 0; --i)
        {
            if (!isspace(name[i])) break;
            else name[i] = 0;
        }
        PlayerData pData;
        pData.setName(name);
        pData.setScore(score);
        highScoreList.insertNode(pData);
    }
    infile.close();
}



