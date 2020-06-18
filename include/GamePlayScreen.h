#ifndef GAMEPLAYSCREEN_H
#define GAMEPLAYSCREEN_H

#include "GameMap.h"
#include "Cell.h"
#include "CellEnum.h"
#include "Path.h"
#include "Ground.h"
#include "Animation.h"
#include "Enemy.h"
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
public:
    GamePlayScreen() = default;
    // Construct from a player name and a map file
//    GamePlayScreen(const std::string& playerName = "Player 1",
//                   const std::string& mapFile = "map_1_data");
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

    friend void loadTexture(sf::Texture&, const std::string&);
    friend void loadSound(sf::SoundBuffer&, const std::string&);
    friend void loadFont(sf::Font&, const std::string&);

private:
    sf::Vector2f checkTowerPlacement(const sf::Vector2f&, GameMap* const, std::vector<Tower>&);
    PlayerData player; // player data
//    GameMap gameMap; // game map
    bool gameOver; // if the player lost
    bool gameVictory; // if the player won
    bool windowClosed; // If the window was closed
//    bool paused; // if game is currently paused
//    bool waveGoing; // if a wave is currently going
//    int waveNumber; // # of current wave, 0 to 4
//    int bulletGeneration; // to create gaps between generation of bullets
//    // towers
//    std::vector<Bullet> bulletVec;
//    std::vector<Tower> towerVec;
//    std::vector<Tower> shootingTowers;
//    // tower textures
//    sf::Texture soapTexture;
//    sf::Texture sanitizerTexture;
//    // button textures
//    sf::Texture pauseTexture;
//    sf::Texture playTexture;
//    sf::Texture soapSelectTexture;
//    sf::Texture sanitizerSelectTexture;
//    // buttons
//    sf::RectangleShape playPauseButton;
//    sf::RectangleShape soapSelection;
//    sf::RectangleShape sanitizerSelection;
//    // boundaries of buttons
//    sf::FloatRect playPauseRect;
//    sf::FloatRect soapSelectionRect;
//    sf::FloatRect sanitizerSelectionRect;
//    // if the buttons are currently clickable
//    bool playPauseClickable;
//    bool soapSelectionClickable;
//    bool sanitizerSelectionClickable;
//    // text font
//    sf::Font descriptionFont;
//    // hover text for buy buttons
//    sf::Text soapDescription;
//    sf::Text sanitizerDescription;
//    // if descriptions should be displayed
//    bool drawSoapDescription;
//    bool drawSanitizerDescription;
//    sf::Text messageText; // general game message text
//    bool drawMessageText; // if message text should be displayed
//    // to show where tower will be placed
//    Tower soapIndicator;
//    Tower sanitizerIndicator;
//    bool drawSoapIndicator;
//    bool drawSanitizerIndicator;
//    // bullet textures
//    sf::Texture squirtTexture;
//    sf::Texture bubbleTexture;
//    // bullet sounds
//    sf::SoundBuffer squirtBuffer;
//    sf::Sound squirtSound;
//    // interactive sounds
//    sf::SoundBuffer mouseClickBuffer;
//    sf::Sound mouseClickSound;
//    sf::SoundBuffer wrongClickBuffer;
//    sf::Sound wrongClickSound;
//    sf::SoundBuffer cashRegisterBuffer;
//    sf::Sound cashRegisterSound;
//    // tower animations
//    Animation soapAnimation;
//    Animation sanitizerAnimation;
//    // to time the tower animations
//    float animationDeltaTime;
//    sf::Clock animationClock;
//    // store mouse location
//    sf::Vector2i mousePos;
//    sf::Vector2f mouseFloat;
//    // to time inputs so they don't repeatedly trigger
//    sf::Clock spaceKeyClock;
//    sf::Clock leftMouseClock;
//    sf::Clock rightMouseClock;
//    sf::Clock playPauseClock;
//    sf::Clock soapSelectionClock;
//    sf::Clock sanitizerSelectionClock;
//    PlaceTower placeWhat; // to determine what kind of tower is being placed
//    bool placeTowerConfirmed; // if the player has clicked to place a tower
//    // input events
//    sf::Event event;
//    // hold whether a button is pressed/held/released
//    ButtonDown spaceKeyDown;
//    ButtonDown leftMouseDown;
//    ButtonDown rightMouseDown;
//    bool spaceKeyPressable;
//    bool leftMousePressable;
//    bool rightMousePressable;
};

void loadTexture(sf::Texture&, const std::string&);
void loadSound(sf::SoundBuffer&, const std::string&);
void loadFont(sf::Font&, const std::string&);

#endif // GAMEPLAYSCREEN_H
