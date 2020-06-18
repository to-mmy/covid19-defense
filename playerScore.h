//
// Created by Kuo Fu on 2020-06-17.
//

#ifndef FINALPROJECT_PLAYERSCORE_H
#define FINALPROJECT_PLAYERSCORE_H
//#include "PlayerData.h"
//#include "GameMap.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

namespace player_data {
    extern const int STARTING_MONEY;
    extern const int STARTING_LIVES;
    extern const std::string FONT_FILE;
}

class PlayerData {
private:
    int money;
    int score;
    int lives;
    std::string name;
    sf::Vector2f drawOrigin;
    sf::Font font;
    sf::Text text;
public:
    // Constructor
    PlayerData(const std::string& playerName = "Player 1",
               const sf::Vector2f& drOrigin = sf::Vector2f(0.f, 0.f));

    // Setters
    void setMoney(int m) { money = m; }
    void setScore(int s) { score = s; }
    void setLives(int l) { lives = l; }
    void setName(const std::string& n) { name = n; }
    void setDrawOrigin(const sf::Vector2f& d) { drawOrigin = d; }

    // Getters
    int getMoney() const { return money; }
    int getScore() const { return score; }
    int getLives() const { return lives; }
    std::string getName() const { return name; }
    sf::Vector2f getDrawOrigin() const { return drawOrigin; }

    void draw(sf::RenderWindow& window);

    // Overload insertion operator, for recording score
    friend std::ostream& operator<<(std::ostream& out, const PlayerData& obj);
    // add the compare operator
    bool operator<(const PlayerData& obj) const;

};
bool PlayerData::operator<(const PlayerData& obj) const
{
    return score > obj.score; // reverse order
}

std::ostream& operator<<(std::ostream& out, const PlayerData& obj);

namespace draw {
    extern const float SPACER;
}

namespace player_data {
    const int STARTING_MONEY = 500;
    const int STARTING_LIVES = 100;
    const std::string FONT_FILE = ResourcePath + "Jingle Bells.ttf";
}

PlayerData::PlayerData(const std::string& playerName, const sf::Vector2f& drOrigin)
        : money(player_data::STARTING_MONEY), score(0), lives(player_data::STARTING_LIVES),
          name(playerName), drawOrigin(drOrigin) {
    // load the font file
    bool validFile = false;
    std::string input = player_data::FONT_FILE;
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
    text.setFont(font);
}

void PlayerData::draw(sf::RenderWindow& window) {
    std::ostringstream outText;
    outText << "Money: " << money << "\nScore: " << score << "\nLives left: " << lives;
    text.setString(outText.str());
//    text.setCharacterSize(game_map::SIDE_PIX / 4);
    text.setFillColor(sf::Color::White);
    text.setPosition(drawOrigin);
    window.draw(text);
}

std::ostream& operator<<(std::ostream& out, const PlayerData& obj) {
    // Save the flags from the output stream
    std::ios outFlags(nullptr);
    outFlags.copyfmt(out);

    // Output looks like the following for a player with name Tommy and score 9000
    // "Tommy         9000"
    // 12 spaces for name, 6 spaces for score
    out << std::left << std::setw(12) << std::setfill(' ') << obj.name;
    out << std::right << std::setw(6) << obj.score;

    // Return out flags to what they were
    out.copyfmt(outFlags);
    return out;
}



#endif //FINALPROJECT_PLAYERSCORE_H
