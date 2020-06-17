#ifndef PLAYERDATA_H
#define PLAYERDATA_H

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
};

std::ostream& operator<<(std::ostream& out, const PlayerData& obj);

#endif // PLAYERDATA_H
