#ifndef GAMEPLAYSCREEN_H
#define GAMEPLAYSCREEN_H

#include "GameMap.h"
#include "Cell.h"
#include "CellEnum.h"
#include "Path.h"
#include "Ground.h"
#include "Animation.h"
#include "Tower.h"
#include "Bullet.h"
#include "PlaceTower.h"
#include "ButtonDown.h"
#include "PlayerData.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>

namespace draw {
extern const sf::Vector2f PLAYER_DRAW_ORIGIN;
extern const sf::Vector2f GAME_MAP_ORIGIN;
extern const float SPACER;
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
}

namespace prices {
extern const int SANITIZER_PRICE;
extern const int SOAP_PRICE;
}

namespace messages {
extern const std::string ROUND_MESSAGES[5];
}

namespace input {
extern const float CLICK_DELAY;
}

class GamePlayScreen {
private:
    // Checks if tower placement is valid, returns position
    sf::Vector2f checkTowerPlacement(const sf::Vector2f&, GameMap* const, std::vector<Tower>&);
    // Gets a pointer to the ground cell that the tower will be placed on
    Cell* getLocation(sf::Vector2f towerCoords, GameMap& map);

    PlayerData player; // player data
    bool gameOver; // if the player lost
    bool gameVictory; // if the player won
    bool windowClosed; // If the window was closed
public:
    GamePlayScreen() = default;
    ~GamePlayScreen() = default;
    // Draw to a window (takes control of window until gameplay is over)
    void draw(sf::RenderWindow& window);

    // Setters
    void setPlayer(const PlayerData& p) { player = p; }

    // Getters
    PlayerData getPlayer() const { return player; }
    bool getWindowClosed() const { return windowClosed; }
    bool getGameOver() const { return gameOver; }
    bool getGameVictory() const { return gameVictory; }

    friend sf::Vector2f normalize(const sf::Vector2f& vec);
    friend void loadTexture(sf::Texture&, const std::string&);
    friend void loadSound(sf::SoundBuffer&, const std::string&);
    friend void loadFont(sf::Font&, const std::string&);
};

sf::Vector2f normalize(const sf::Vector2f& vec);
void loadTexture(sf::Texture&, const std::string&);
void loadSound(sf::SoundBuffer&, const std::string&);
void loadFont(sf::Font&, const std::string&);

#endif // GAMEPLAYSCREEN_H
