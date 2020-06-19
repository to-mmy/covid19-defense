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
namespace menu {
    const int NUM_OF_MENU_CHOICES = 3;
    const std::string RESOURCE_PATH = "resources/";
}

sf::Vector2f normalize(const sf::Vector2f& vec);
void loadTexture(sf::Texture&, const std::string&);
void loadSound(sf::SoundBuffer&, const std::string&);
void loadFont(sf::Font&, const std::string&);
void readFromFile(std::string inFilename, HighScoreList &highScoreList);

///////// main /////////////
int main(){
    sf::RenderWindow window(sf::VideoMode(draw::WINDOW_WIDTH, draw::WINDOW_HEIGHT), "COVID-19 Defense");

//     load music
    sf::Music backgroundMusic;

    if (!backgroundMusic.openFromFile(menu::RESOURCE_PATH + "background-scifi.ogg"))
        std::cout << "Can't load the background music" << std::endl;

    backgroundMusic.setVolume(50);
    backgroundMusic.play();

    // menu
    MainMenuScreen menu(window.getSize().x, window.getSize().y);

    HighScoreList highScoreList;
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
        std::string playername = "play 1"; // for template
        int choice = menu.displayMenu(window, playername); ///////// get the player name during display menu
        if(choice == 0){ // play

            //test for the player score
            PlayerData p3;
            p3.setName(playername); /// set the player name as the user input

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
                gameOverScreen.displayEnd(window, p3);
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

sf::Vector2f normalize(const sf::Vector2f& vec) {
	float magnitude = sqrt((vec.x * vec.x) + (vec.y * vec.y));
	if (magnitude == 0.f) {
		return vec;
	}
	return sf::Vector2f(vec.x / magnitude, vec.y / magnitude);

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

