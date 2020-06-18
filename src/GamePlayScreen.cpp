#include "GamePlayScreen.h"

namespace draw {
const sf::Vector2f PLAYER_DRAW_ORIGIN = sf::Vector2f(730.f, 630.f);
const sf::Vector2f GAME_MAP_ORIGIN = sf::Vector2f(0.f, 0.f);
const float SPACER = 10.f;
const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
}

namespace prices {
const int SANITIZER_PRICE = 400;
const int SOAP_PRICE = 150;
}

namespace messages {
const std::string ROUND_MESSAGES[5] {
        "Welcome to the COVID-19 Defense Force!\n"
        "Click on a tower in the right-hand column\n"
        "to select it, and click again on a city block\n"
        "to spend money to place your tower.\n"
        "Right-click to cancel placing your tower.\n"
        "Keep an eye on your lives - if you run out,\n"
        "it's game over! Click the play button or\n"
        "press the space key when you're ready\n"
        "to begin.",
        "Congratulations, you've beaten the first wave!\n"
        "Did you know you can build towers while the\n"
        "game is paused? Prepare yourself for the\n"
        "second wave, and click the play button when\n"
        "you're ready to begin.",
        "Second wave complete! Almost halfway there,\n"
        "you can do it! Get ready for the third wave,\n"
        "and click play to begin.",
        "Third wave down! You're more than halfway\n"
        "there, keep it up!",
        "One more wave to go. Just one more and\n"
        "you're free. Don't stop now!"
    };
}

namespace input {
const float CLICK_DELAY = 0.2f;
}

using input::CLICK_DELAY;

//GamePlayScreen::GamePlayScreen(const std::string& playerName, const std::string& mapFile)
//    : player(playerName, draw::PLAYER_DRAW_ORIGIN),
//      gameMap(mapFile, draw::GAME_MAP_ORIGIN),
//      gameOver(false),
//      gameVictory(false),
//      windowClosed(false),
//      paused(true),
//      waveGoing(false),
//      waveNumber(0),
//      bulletGeneration(0),
//      bulletVec(),
//      towerVec(),
//      shootingTowers(),
//      playPauseButton(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT)),
//      soapSelection(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT)),
//      sanitizerSelection(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT)),
//      playPauseClickable(false),
//      soapSelectionClickable(false),
//      sanitizerSelectionClickable(false),
//      drawSoapDescription(false),
//      drawSanitizerDescription(false),
//      drawMessageText(true),
//      soapIndicator(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT),
//                    sf::Vector2f(0.f,0.f), "soap"),
//      sanitizerIndicator(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT),
//                         sf::Vector2f(0.f,0.f), "sanitizer"),
//      drawSoapIndicator(false),
//      drawSanitizerIndicator(false),
//      soapAnimation(&soapTexture, sf::Vector2u(3, 1), 0.3f),
//      sanitizerAnimation(&sanitizerTexture, sf::Vector2u(3, 1), 0.3f),
//      animationDeltaTime(0.f),
//      animationClock(),
//      mousePos(-1, -1),
//      mouseFloat(mousePos),
//      spaceKeyClock(),
//      leftMouseClock(),
//      rightMouseClock(),
//      playPauseClock(),
//      soapSelectionClock(),
//      sanitizerSelectionClock(),
//      placeWhat(PlaceTower::NONE),
//      placeTowerConfirmed(false),
//      event(),
//      spaceKeyDown(ButtonDown::RELEASE),
//      leftMouseDown(ButtonDown::RELEASE),
//      rightMouseDown(ButtonDown::RELEASE),
//      spaceKeyPressable(false),
//      leftMousePressable(false),
//      rightMousePressable(false)
//      {
//    // textures
//    loadTexture(soapTexture, std::string(game_map::RESOURCE_PATH
//                                         + "bar_soap_transparent.png"));
//    loadTexture(sanitizerTexture, std::string(game_map::RESOURCE_PATH
//                                              + "sanitizer_bottle_transparent.png"));
//
//    loadTexture(pauseTexture, std::string(game_map::RESOURCE_PATH + "pause_button.png"));
//    loadTexture(playTexture, std::string(game_map::RESOURCE_PATH + "play_button.png"));
//    loadTexture(soapSelectTexture, std::string(game_map::RESOURCE_PATH
//                                               + "bar_soap_selection.png"));
//    loadTexture(sanitizerSelectTexture, std::string(game_map::RESOURCE_PATH
//                                                    + "sanitizer_bottle_selection.png"));
//
//    // buttons
//    playPauseButton.setTexture(&playTexture);
//    playPauseButton.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
//                                             + draw::SPACER,
//                                             (game_map::SIDE_FLT + draw::SPACER) * 0.f)
//                                ); // 0th down
//
//    soapSelection.setTexture(&soapSelectTexture);
//    soapSelection.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
//                                           + draw::SPACER,
//                                           (game_map::SIDE_FLT + draw::SPACER) * 1.f)
//                              ); // 1st down
//
//    sanitizerSelection.setTexture(&sanitizerSelectTexture);
//    sanitizerSelection.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
//                                                + draw::SPACER,
//                                                (game_map::SIDE_FLT + draw::SPACER) * 2.f)
//                                   ); // 2nd down
//
//    // Rects holding bounds of the buttons, for checking mouse input
//    playPauseRect = playPauseButton.getGlobalBounds();
//    soapSelectionRect = soapSelection.getGlobalBounds();
//    sanitizerSelectionRect = sanitizerSelection.getGlobalBounds();
//    loadTexture(soapTexture, std::string(game_map::RESOURCE_PATH
//                                         + "bar_soap_transparent.png"));
//    loadTexture(sanitizerTexture, std::string(game_map::RESOURCE_PATH
//                                              + "sanitizer_bottle_transparent.png"));
//
//    //loading textures for tower selection
//    loadTexture(pauseTexture, std::string(game_map::RESOURCE_PATH + "pause_button.png"));
//    loadTexture(playTexture, std::string(game_map::RESOURCE_PATH + "play_button.png"));
//    loadTexture(soapSelectTexture, std::string(game_map::RESOURCE_PATH
//                                               + "bar_soap_selection.png"));
//    loadTexture(sanitizerSelectTexture, std::string(game_map::RESOURCE_PATH
//                                                    + "sanitizer_bottle_selection.png"));
//
//    // positioning selection buttons
//    playPauseButton.setTexture(&playTexture);
//    playPauseButton.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
//                                             + draw::SPACER,
//                                             (game_map::SIDE_FLT + draw::SPACER) * 0.f)
//                                ); // 0th down
//
//    soapSelection.setTexture(&soapSelectTexture);
//    soapSelection.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
//                                           + draw::SPACER,
//                                           (game_map::SIDE_FLT + draw::SPACER) * 1.f)
//                              ); // 1st down
//    sanitizerSelection.setTexture(&sanitizerSelectTexture);
//    sanitizerSelection.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
//                                                + draw::SPACER,
//                                                (game_map::SIDE_FLT + draw::SPACER) * 2.f)
//                                   ); // 2nd down
//
//    // Rects holding bounds of the buttons, for checking mouse input
//    sf::FloatRect playPauseRect = playPauseButton.getGlobalBounds();
//    sf::FloatRect soapSelectionRect = soapSelection.getGlobalBounds();
//    sf::FloatRect sanitizerSelectionRect = sanitizerSelection.getGlobalBounds();
//
//    // Descriptions for buttons
//    loadFont(descriptionFont, std::string(game_map::RESOURCE_PATH + "Jingle Bells.ttf"));
//    soapDescription.setFont(descriptionFont);
//    soapDescription.setPosition(soapSelection.getPosition()
//                                + sf::Vector2f(game_map::SIDE_FLT + draw::SPACER, 0.f));
//    soapDescription.setCharacterSize(game_map::SIDE_PIX / 4);
//    std::ostringstream soapDescriptionString;
//    soapDescriptionString << "Soap Tower\nCost: " << prices::SOAP_PRICE
//                          << "\nFires soap bubbles.";
//    soapDescription.setString(soapDescriptionString.str());
//    soapDescription.setFillColor(sf::Color::White);
//
//    sanitizerDescription.setFont(descriptionFont);
//    sanitizerDescription.setPosition(sanitizerSelection.getPosition()
//                                     + sf::Vector2f(game_map::SIDE_FLT + draw::SPACER, 0.f));
//    sanitizerDescription.setCharacterSize(game_map::SIDE_PIX / 4);
//    std::ostringstream sanitizerDescriptionString;
//    sanitizerDescriptionString << "Sanitizer Tower\nCost: " << prices::SANITIZER_PRICE
//                               << "\nFires squirts of sanitizer.";
//    sanitizerDescription.setString(sanitizerDescriptionString.str());
//    sanitizerDescription.setFillColor(sf::Color::White);
//
//    // Game messages to the player
//    messageText.setFont(descriptionFont);
//    messageText.setPosition(sanitizerSelection.getPosition()
//                            + sf::Vector2f(0.f, game_map::SIDE_FLT + draw::SPACER));
//    messageText.setCharacterSize(game_map::SIDE_PIX / 4);
//    messageText.setString(messages::ROUND_MESSAGES[0]);
//    messageText.setFillColor(sf::Color::White);
//
//    //loading textures for bullets
//    sf::Texture squirtTexture;
//    loadTexture(squirtTexture, std::string(game_map::RESOURCE_PATH + "sanitizer_squirt_rect.png"));
//    sf::Texture bubbleTexture;
//    loadTexture(bubbleTexture, std::string(game_map::RESOURCE_PATH
//                                           + "soap_bubbles_transparent.png"));
//
//    //loading sounds
//    sf::SoundBuffer squirtBuffer;
//    loadSound(squirtBuffer, std::string(game_map::RESOURCE_PATH + "squirt_sound.wav"));
//    sf::Sound squirtSound;
//    squirtSound.setBuffer(squirtBuffer);
//
//    sf::SoundBuffer mouseClickBuffer;
//    loadSound(mouseClickBuffer, std::string(game_map::RESOURCE_PATH + "mouse_click.wav"));
//    sf::Sound mouseClickSound;
//    mouseClickSound.setBuffer(mouseClickBuffer);
//
//    sf::SoundBuffer wrongClickBuffer;
//    loadSound(wrongClickBuffer, std::string(game_map::RESOURCE_PATH + "wrong.wav"));
//    sf::Sound wrongClickSound;
//    wrongClickSound.setBuffer(wrongClickBuffer);
//
//    sf::SoundBuffer cashRegisterBuffer;
//    loadSound(cashRegisterBuffer, std::string(game_map::RESOURCE_PATH + "cash_register.wav"));
//    sf::Sound cashRegisterSound;
//    cashRegisterSound.setBuffer(cashRegisterBuffer);
//
//    // indicators for tower placement
//    soapIndicator.getRect().setTexture(&soapTexture);
//    soapIndicator.getRect().setTextureRect(soapAnimation.uvRect);
//    soapIndicator.getRect().setOrigin(sf::Vector2f(game_map::SIDE_FLT / 2.f,
//                                                   game_map::SIDE_FLT / 2.f));
//    sanitizerIndicator.getRect().setTexture(&sanitizerTexture);
//    sanitizerIndicator.getRect().setTextureRect(sanitizerAnimation.uvRect);
//    sanitizerIndicator.getRect().setOrigin(sf::Vector2f(game_map::SIDE_FLT / 2.f,
//                                                        game_map::SIDE_FLT / 2.f));
//}


void GamePlayScreen::draw(sf::RenderWindow& window) {
    player.setDrawOrigin(draw::PLAYER_DRAW_ORIGIN);
    //MAP MAIN STUFF**********************************************************

    GameMap gameMap(std::string("map_1_data"), draw::GAME_MAP_ORIGIN);

    // Enemy sprite texture
    sf::Texture covidTexture;
    std::string covidTextureFile(game_map::RESOURCE_PATH + "covid_1.png");
    loadTexture(covidTexture, covidTextureFile);

//
//    sf::Sprite covid;
//    covid.setTexture(covidTexture);
//    sf::Vector2u covidCoords = gameMap.getStartCoords();
//
//    Path* pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
//    covid.setPosition(draw::GAME_MAP_ORIGIN + pathPtr->getPosition());
//
//    covid.setScale(game_map::SPRITE_SCALE);
//
//    const float COVID_SPEED = 0.05f;
//
//    sf::Vector2f covidDirection;
//    sf::Vector2f covidDestination = draw::GAME_MAP_ORIGIN + pathPtr->getNextPosition();
//    sf::Vector2f distanceToDestination(std::abs(covidDestination.x - covid.getPosition().x),
//                                       std::abs(covidDestination.y - covid.getPosition().y));
//
//    bool drawCovid = true;
    //MAP MAIN STUFF**********************************************************

    bool paused = true;

    bool waveGoing =
        0; //0 if the wave has finished/hasn't started yet, 1 if the wave of enemies is still going
    int waveNumber = 0; //0 through 5
    int bulletGeneration = 0; //used to create gaps between the generation of each bullet

    //opening a window
//    sf::RenderWindow window;
//    sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2)
//                                  - (draw::WINDOW_WIDTH / 2),
//                              (sf::VideoMode::getDesktopMode().height / 2)
//                                  - (draw::WINDOW_HEIGHT / 2));
//    window.create(sf::VideoMode(draw::WINDOW_WIDTH, draw::WINDOW_HEIGHT), "COVID-19 Defense",
//                  sf::Style::Titlebar | sf::Style::Close);
//    window.setPosition(centerWindow);

    //Defining objects
    //Tower tower(sf::Vector2f(150, 150), sf::Vector2f(0, 0), "soap");
    Enemy enemy(sf::Vector2f(game_map::SIDE_PIX, game_map::SIDE_PIX));
    std::vector<Bullet> bulletVec;
    std::vector<Tower> towerVec;
    std::vector<Tower> shootingTowers;
    bool isFiring = false;
    //bool ground = false;

    //loading textures for animated towers     //textures and animations have to be loaded outside of the main while loop
    sf::Texture soapTexture;
    loadTexture(soapTexture, std::string(game_map::RESOURCE_PATH
                                         + "bar_soap_transparent.png"));
    sf::Texture sanitizerTexture;
    loadTexture(sanitizerTexture, std::string(game_map::RESOURCE_PATH
                                              + "sanitizer_bottle_transparent.png"));

    //loading textures for tower selection
    sf::Texture pauseTexture;
    loadTexture(pauseTexture, std::string(game_map::RESOURCE_PATH + "pause_button.png"));
    sf::Texture playTexture;
    loadTexture(playTexture, std::string(game_map::RESOURCE_PATH + "play_button.png"));
    sf::Texture soapSelectTexture;
    loadTexture(soapSelectTexture, std::string(game_map::RESOURCE_PATH
                                               + "bar_soap_selection.png"));
    sf::Texture sanitizerSelectTexture;
    loadTexture(sanitizerSelectTexture, std::string(game_map::RESOURCE_PATH
                                                    + "sanitizer_bottle_selection.png"));

    // creating and positioning selection buttons

    sf::RectangleShape playPauseButton(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT));
    playPauseButton.setTexture(&playTexture);
    playPauseButton.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
                                             + draw::SPACER,
                                             (game_map::SIDE_FLT + draw::SPACER) * 0.f)
                                ); // 0th down

    sf::RectangleShape soapSelection(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT));
    soapSelection.setTexture(&soapSelectTexture);
    soapSelection.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
                                           + draw::SPACER,
                                           (game_map::SIDE_FLT + draw::SPACER) * 1.f)
                              ); // 1st down
    sf::RectangleShape sanitizerSelection(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT));
    sanitizerSelection.setTexture(&sanitizerSelectTexture);
    sanitizerSelection.setPosition(sf::Vector2f((gameMap.getSideLength() * game_map::SIDE_FLT)
                                                + draw::SPACER,
                                                (game_map::SIDE_FLT + draw::SPACER) * 2.f)
                                   ); // 2nd down

    // Rects holding bounds of the buttons, for checking mouse input
    sf::FloatRect playPauseRect = playPauseButton.getGlobalBounds();
    sf::FloatRect soapSelectionRect = soapSelection.getGlobalBounds();
    sf::FloatRect sanitizerSelectionRect = sanitizerSelection.getGlobalBounds();

    // bools for determining if the buttons are clickable
    bool playPauseClickable = false;
    bool soapSelectionClickable = false;
    bool sanitizerSelectionClickable = false;

    // Descriptions for buttons
    sf::Font descriptionFont;
    loadFont(descriptionFont, std::string(game_map::RESOURCE_PATH + "Jingle Bells.ttf"));
    sf::Text soapDescription;
    soapDescription.setFont(descriptionFont);
    soapDescription.setPosition(soapSelection.getPosition()
                                + sf::Vector2f(game_map::SIDE_FLT + draw::SPACER, 0.f));
    soapDescription.setCharacterSize(game_map::SIDE_PIX / 4);
    std::ostringstream soapDescriptionString;
    soapDescriptionString << "Soap Tower\nCost: " << prices::SOAP_PRICE
                          << "\nFires soap bubbles.";
    soapDescription.setString(soapDescriptionString.str());
    soapDescription.setFillColor(sf::Color::White);
    bool drawSoapDescription = false;

    sf::Text sanitizerDescription;
    sanitizerDescription.setFont(descriptionFont);
    sanitizerDescription.setPosition(sanitizerSelection.getPosition()
                                     + sf::Vector2f(game_map::SIDE_FLT + draw::SPACER, 0.f));
    sanitizerDescription.setCharacterSize(game_map::SIDE_PIX / 4);
    std::ostringstream sanitizerDescriptionString;
    sanitizerDescriptionString << "Sanitizer Tower\nCost: " << prices::SANITIZER_PRICE
                               << "\nFires squirts of sanitizer.";
    sanitizerDescription.setString(sanitizerDescriptionString.str());
    sanitizerDescription.setFillColor(sf::Color::White);
    bool drawSanitizerDescription = false;

    // Game messages to the player


    sf::Text messageText;
    messageText.setFont(descriptionFont);
    messageText.setPosition(sanitizerSelection.getPosition()
                            + sf::Vector2f(0.f, game_map::SIDE_FLT + draw::SPACER));
    messageText.setCharacterSize(game_map::SIDE_PIX / 4);
    messageText.setString(messages::ROUND_MESSAGES[0]);
    messageText.setFillColor(sf::Color::White);
    bool drawMessageText = true;

    // A temporary tower to show placement
    Tower* towerIndicatorPtr = nullptr;

    //loading textures for bullets
    sf::Texture squirtTexture;
    loadTexture(squirtTexture, std::string(game_map::RESOURCE_PATH + "sanitizer_squirt_rect.png"));
    sf::Texture bubbleTexture;
    loadTexture(bubbleTexture, std::string(game_map::RESOURCE_PATH
                                           + "soap_bubbles_transparent.png"));

    //loading sounds
    sf::SoundBuffer squirtBuffer;
    loadSound(squirtBuffer, std::string(game_map::RESOURCE_PATH + "squirt_sound.wav"));
    sf::Sound squirtSound;
    squirtSound.setBuffer(squirtBuffer);

    sf::SoundBuffer mouseClickBuffer;
    loadSound(mouseClickBuffer, std::string(game_map::RESOURCE_PATH + "mouse_click.wav"));
    sf::Sound mouseClickSound;
    mouseClickSound.setBuffer(mouseClickBuffer);

    sf::SoundBuffer wrongClickBuffer;
    loadSound(wrongClickBuffer, std::string(game_map::RESOURCE_PATH + "wrong.wav"));
    sf::Sound wrongClickSound;
    wrongClickSound.setBuffer(wrongClickBuffer);

    sf::SoundBuffer cashRegisterBuffer;
    loadSound(cashRegisterBuffer, std::string(game_map::RESOURCE_PATH + "cash_register.wav"));
    sf::Sound cashRegisterSound;
    cashRegisterSound.setBuffer(cashRegisterBuffer);

    //animation
    Animation soapAnimation(&soapTexture, sf::Vector2u(3, 1), 0.3f);
    Animation sanitizerAnimation(&sanitizerTexture, sf::Vector2u(3, 1), 0.3f);
    //player.setAnimation(soapAnimation);
    float animationDeltaTime = 0.0f;
    sf::Clock animationClock;

    //coordinates for mouse click

    sf::Vector2i mousePos(-1, -1);
    sf::Vector2f mouseFloat(mousePos);

    // delay timer for mouse click
    sf::Clock spaceKeyClock;
    sf::Clock leftMouseClock;
    sf::Clock rightMouseClock;
    sf::Clock playPauseClock;
    sf::Clock soapSelectionClock;
    sf::Clock sanitizerSelectionClock;
    const float CLICK_DELAY = 0.2f; // delay in seconds -> 0.2 seconds (100 ms == instant to humans)

    // What kind of tower, if any, to place
    PlaceTower placeWhat = PlaceTower::NONE;
    bool placeTowerConfirmed = false;


    sf::Event event;
    ButtonDown spaceKeyDown = ButtonDown::RELEASE;
    bool spaceKeyPressable = false;
    ButtonDown leftMouseDown = ButtonDown::RELEASE;
    bool leftMousePressable = false;
    ButtonDown rightMouseDown = ButtonDown::RELEASE;
    bool rightMousePressable = false;

    gameOver = false;
    gameVictory = false;
    windowClosed = false;

    //main loop
    while (!(windowClosed || gameOver || gameVictory)) {

        //animationDeltaTime used to make animation consistent for different OS's
        animationDeltaTime = animationClock.restart().asSeconds();

        // Input polling
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                windowClosed = true;
                break;
            case sf::Event::LostFocus:
                paused = true;
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Space) {
                    spaceKeyClock.restart();
                    spaceKeyDown = ButtonDown::PRESS;
                }
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Space) {
                    spaceKeyDown = ButtonDown::RELEASE;
                }
                break;

            case sf::Event::MouseMoved:
                mousePos = sf::Mouse::getPosition(window);
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    leftMouseClock.restart();
                    leftMouseDown = ButtonDown::PRESS;
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    rightMouseClock.restart();
                    rightMouseDown = ButtonDown::PRESS;
                }
                mousePos = sf::Mouse::getPosition(window);
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    leftMouseDown = ButtonDown::RELEASE;
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    rightMouseDown = ButtonDown::RELEASE;
                }

            default:
                mousePos = sf::Mouse::getPosition(window);
                if (leftMouseDown == ButtonDown::PRESS) {
                    leftMouseDown = ButtonDown::HOLD;
                }
                if (rightMouseDown == ButtonDown::PRESS) {
                    rightMouseDown = ButtonDown::HOLD;
                }
            }

        }

        if (!windowClosed) {

            // Updating based on inputs
            mouseFloat = sf::Vector2f(mousePos);
            // For each button, highlight it if it is clickable (mouse within borders)
            if (playPauseRect.contains(mouseFloat)) {
                playPauseButton.setFillColor(sf::Color::Yellow);
                playPauseClickable = true;
                soapSelection.setFillColor(sf::Color::White);
                soapSelectionClickable = false;
                sanitizerSelection.setFillColor(sf::Color::White);
                sanitizerSelectionClickable = false;
            }
            else if (soapSelectionRect.contains(mouseFloat)) {
                playPauseButton.setFillColor(sf::Color::White);
                playPauseClickable = false;
                soapSelection.setFillColor(sf::Color::Yellow);
                soapSelectionClickable = true;
                sanitizerSelection.setFillColor(sf::Color::White);
                sanitizerSelectionClickable = false;
            }
            else if (sanitizerSelectionRect.contains(mouseFloat)) {
                playPauseButton.setFillColor(sf::Color::White);
                playPauseClickable = false;
                soapSelection.setFillColor(sf::Color::White);
                soapSelectionClickable = false;
                sanitizerSelection.setFillColor(sf::Color::Yellow);
                sanitizerSelectionClickable = true;
            }
            else {
                playPauseButton.setFillColor(sf::Color::White);
                playPauseClickable = false;
                soapSelection.setFillColor(sf::Color::White);
                soapSelectionClickable = false;
                sanitizerSelection.setFillColor(sf::Color::White);
                sanitizerSelectionClickable = false;
            }
            drawSoapDescription = soapSelectionClickable;
            drawSanitizerDescription = sanitizerSelectionClickable;

            // space key pauses game
            if (spaceKeyDown == ButtonDown::PRESS && spaceKeyPressable
                && playPauseClock.getElapsedTime().asSeconds() > CLICK_DELAY) {
                //Play the click sound
                mouseClickSound.play();

                playPauseClock.restart();
                // Round messages will go away when player clicks play
                drawMessageText = false;
                if (paused) {
                    paused = false;
                    playPauseButton.setFillColor(sf::Color::Red);
                    playPauseButton.setTexture(&pauseTexture);
                }
                else {
                    paused = true;
                    playPauseButton.setFillColor(sf::Color::Red);
                    playPauseButton.setTexture(&playTexture);
                }
            }
            if (spaceKeyDown == ButtonDown::RELEASE
                && spaceKeyClock.getElapsedTime().asSeconds() > CLICK_DELAY) {
                spaceKeyPressable = true;
            }
            else {
                spaceKeyPressable = false;
            }

            // We can click buttons and place towers while paused
            // Determine if something was clicked
            if (leftMouseDown == ButtonDown::PRESS && leftMousePressable) {
                // Clicked the play/pause button
                if (playPauseClickable && playPauseClock.getElapsedTime().asSeconds() > CLICK_DELAY) {
                    // Restart the clickable timer
                    playPauseClock.restart();

                    //Play the click sound
                    mouseClickSound.play();

                    // Round messages will go away when player clicks play
                    drawMessageText = false;
                    if (paused) {
                        paused = false;
                        playPauseButton.setFillColor(sf::Color::Red);
                        playPauseButton.setTexture(&pauseTexture);
                    }
                    else {
                        paused = true;
                        playPauseButton.setFillColor(sf::Color::Red);
                        playPauseButton.setTexture(&playTexture);
                    }
                }
                // Clicked the sanitizer tower selection button
                else if (sanitizerSelectionClickable
                         && sanitizerSelectionClock.getElapsedTime().asSeconds() > CLICK_DELAY) {
                    // Restart the clickable timer
                    sanitizerSelectionClock.restart();

                    if (player.getMoney() >= prices::SANITIZER_PRICE) {
                        // Play the click sound
                        mouseClickSound.play();

                        placeWhat = PlaceTower::SANITIZER;
                    }
                    else {
                        // Can't click (not enough money)
                        wrongClickSound.play();

                        sanitizerSelection.setFillColor(sf::Color::Red);
                    }
                }
                // Clicked the soap tower selection button
                else if (soapSelectionClickable
                         && soapSelectionClock.getElapsedTime().asSeconds() > CLICK_DELAY) {
                    // Restart the clickable timer
                    soapSelectionClock.restart();

                    if (player.getMoney() >= prices::SOAP_PRICE) {
                        //Play the click sound
                        mouseClickSound.play();
                        placeWhat = PlaceTower::SOAP;
                    }
                    else {
                        // Can't click (not enough money)
                        wrongClickSound.play();

                        soapSelection.setFillColor(sf::Color::Red);
                    }
                }
                // Clicked to place a sanitizer tower
                else if (placeWhat == PlaceTower::SANITIZER) {
                    placeTowerConfirmed = true;
                }
                // Clicked to place a soap tower
                else if (placeWhat == PlaceTower::SOAP) {
                    placeTowerConfirmed = true;
                }
            }
            if (leftMouseDown == ButtonDown::RELEASE
                && leftMouseClock.getElapsedTime().asSeconds() > CLICK_DELAY) {
                leftMousePressable = true;
            }
            else {
                leftMousePressable = false;
            }

            if (rightMouseDown == ButtonDown::PRESS && rightMousePressable) {
                if (placeWhat != PlaceTower::NONE) {
                    mouseClickSound.play();
                    placeWhat = PlaceTower::NONE;
                }
            }
            if (rightMouseDown == ButtonDown::RELEASE
                && rightMouseClock.getElapsedTime().asSeconds() > CLICK_DELAY) {
                rightMousePressable = true;
            }
            else {
                rightMousePressable = false;
            }

            switch (placeWhat) {
            case PlaceTower::NONE:
                if (towerIndicatorPtr) {
                    delete towerIndicatorPtr;
                    towerIndicatorPtr = nullptr;
                }
                placeTowerConfirmed = false;
                break;
            case PlaceTower::SANITIZER:
                try { //try placing a sanitizer tower
                    // show an indicator for the Tower
                    // If the indicator already exists, clean it up
                    if (towerIndicatorPtr) {
                        delete towerIndicatorPtr;
                        towerIndicatorPtr = nullptr;
                    }
                    // Show the indicator as green by default
                    towerIndicatorPtr = new Tower(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT),
                                                   mouseFloat, "sanitizer");
                    // Center on mouse
                    towerIndicatorPtr->getRect().setOrigin(sf::Vector2f(game_map::SIDE_FLT / 2.f,
                                                                        game_map::SIDE_FLT / 2.f));
                    towerIndicatorPtr->getRect().setPosition(mouseFloat);
                    // Set textures, color
                    towerIndicatorPtr->getRect().setTexture(&sanitizerTexture);
                    towerIndicatorPtr->getRect().setTextureRect(sanitizerAnimation.uvRect);
                    towerIndicatorPtr->getRect().setFillColor(sf::Color::Green);


                    sf::Vector2f place = checkTowerPlacement(mouseFloat, &gameMap, towerVec);
                    if (placeTowerConfirmed) {
                        // Add the tower to the towerVec
                        Tower t(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT),
                                place, "sanitizer");
                        t.getRect().setTexture(&sanitizerTexture);
                        towerVec.push_back(t);

                        // Pay for the tower
                        player.loseMoney(prices::SANITIZER_PRICE);
                        cashRegisterSound.play();

                        // reset values
                        delete towerIndicatorPtr;
                        towerIndicatorPtr = nullptr;
                        placeWhat = PlaceTower::NONE;
                        placeTowerConfirmed = false;
                    }
                }
                catch (int a) {
                    towerIndicatorPtr->getRect().setFillColor(sf::Color::Red);
                    if (placeTowerConfirmed) {
                        // Player tried to confirm placing a tower
                        wrongClickSound.play();
                        placeTowerConfirmed = false;
                    }
                    }
                catch (...) {}
                break;
            case PlaceTower::SOAP:
                try { //try placing a soap tower
                    // Show an indicator for the tower
                    towerIndicatorPtr = new Tower(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT),
                                                  mouseFloat, "soap");
                    // Center on mouse
                    towerIndicatorPtr->getRect().setOrigin(sf::Vector2f(game_map::SIDE_FLT / 2.f,
                                                                        game_map::SIDE_FLT / 2.f));
                    towerIndicatorPtr->getRect().setPosition(mouseFloat);
                    // Set textures, color
                    towerIndicatorPtr->getRect().setTexture(&soapTexture);
                    towerIndicatorPtr->getRect().setTextureRect(soapAnimation.uvRect);
                    towerIndicatorPtr->getRect().setFillColor(sf::Color::Green);

                    sf::Vector2f place = checkTowerPlacement(mouseFloat, &gameMap, towerVec);
                    if (placeTowerConfirmed) {
                        // Add the tower to the towerVec
                        Tower t(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), place, "soap");
                        t.getRect().setTexture(&soapTexture);
                        towerVec.push_back(t);

                        // Pay for the tower
                        player.loseMoney(prices::SOAP_PRICE);
                        cashRegisterSound.play();

                        // reset values
                        delete towerIndicatorPtr;
                        towerIndicatorPtr = nullptr;
                        placeWhat = PlaceTower::NONE;
                        placeTowerConfirmed = false;
                    }
                }
                catch (int a) {
                    towerIndicatorPtr->getRect().setFillColor(sf::Color::Red);
                    if (placeTowerConfirmed) {
                        // Player tried to confirm placing a tower
                        wrongClickSound.play();
                        placeTowerConfirmed = false;
                    }
                    }
                catch (...) {}
                break;
            default:
                placeWhat = PlaceTower::NONE;
            }


            // Update enemies, bullets (only update while game is not paused)
            if (!paused) {
    //            if (drawCovid) {
    //                // Check if the covid has made it to next path
    //                distanceToDestination
    //                    = sf::Vector2f(abs(covidDestination.x - covid.getPosition().x),
    //                                   abs(covidDestination.y - covid.getPosition().y));
    //                if (std::abs(distanceToDestination.x) < 1.f
    //                        && std::abs(distanceToDestination.y) < 1.f) {
    //                    covidCoords = pathPtr->getNextCoords();
    //                    pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x]
    //                                                                    [covidCoords.y]);
    //                    covidDestination = draw::GAME_MAP_ORIGIN + pathPtr->getNextPosition();
    //                    // Check if covid has made it to the end
    //                    if (pathPtr->getNextCoords() == gameMap.getExitCoords()) {
    //                        drawCovid = false;
    //                    }
    //                }
    //
    //
    //                // Move the covid
    //                covidDirection = normalize(covidDestination - covid.getPosition());
    //                covid.move(COVID_SPEED * covidDirection);
    //            }

                //create gap between bullets
                if (bulletGeneration % 200 ==
                        0) { //adjust the mod # higher to slow production of bullets, lower the mod # to increase bullet production
                    isFiring = true;
                }
                ++bulletGeneration;

                //squirtSound.setLoop(false);

                //make sure you only play the sound once (turn a bool variable on if a type is shooting and play the sound only once at the end of the for loop)
                //generate bullets (squirt dimensions: 17x4, bubble dimensions: 75x33)
                if (isFiring == true) {
                    //Bullet newBullet(sf::Vector2f(75, 33), tower.getX(), tower.getY(), enemy.getX(), enemy.getY()); //shoot at first enemy, pass in tower for starting position
                    //newBullet.getBullet().setTexture(&bubbleTexture);
                    //bulletVec.push_back(newBullet);
                    //isFiring = false;
                    //squirtSound.play();
                    //squirtSound.stop();
                }
            }






            //update animations
            soapAnimation.Update(0, animationDeltaTime);
            sanitizerAnimation.Update(0, animationDeltaTime);

            //(for animation) set texture rectangle for all towers
            //tower.getRect().setTextureRect(soapAnimation.uvRect); //step through the array/vector and use the same element # for the corresponding player and animation objects
            for (unsigned i = 0; i < towerVec.size(); i++) {
                towerVec[i].getRect().setTextureRect(sanitizerAnimation.uvRect);
            }
            //player1.getRect().setTextureRect(sanitizerAnimation.uvRect);



            window.clear();

            // Drawing
            gameMap.draw(window);
    //        if (drawCovid) {
    //            window.draw(covid);
    //        }
            player.draw(window);

            //draw bullets to window
            for (unsigned i = 0; i < bulletVec.size(); i++) {
                bulletVec[i].draw(window);
                bulletVec[i].fire();
            }


            //draw all objects to window
            for (unsigned i = 0; i < towerVec.size(); i++) {
                towerVec[i].draw(window);
            }
            window.draw(playPauseButton);
            window.draw(sanitizerSelection);
            window.draw(soapSelection);
            if (drawSoapDescription) {
                window.draw(soapDescription);
            }
            if (drawSanitizerDescription) {
                window.draw(sanitizerDescription);
            }
            if (drawMessageText) {
                window.draw(messageText);
            }
            if (towerIndicatorPtr) {
                towerIndicatorPtr->draw(window);
            }
            window.display();
        }
    }
    // Cleaning up allocated memory
    if (towerIndicatorPtr) {
        delete towerIndicatorPtr;
        towerIndicatorPtr = nullptr;
    }
//
//    while (!(gameOver || gameVictory)) {
//
//        //animationDeltaTime used to make animation consistent for different OS's
//        animationDeltaTime = animationClock.restart().asSeconds();
//
//        // Input polling
//        while (window.pollEvent(event)) {
//            switch (event.type) {
//            case sf::Event::Closed:
//                windowClosed = true;
//                gameOver = true;
//                break;
//            case sf::Event::LostFocus:
//                paused = true;
//                break;
//
//            case sf::Event::KeyPressed:
//                if (event.key.code == sf::Keyboard::Space) {
//                    spaceKeyClock.restart();
//                    spaceKeyDown = ButtonDown::PRESS;
//                }
//                break;
//            case sf::Event::KeyReleased:
//                if (event.key.code == sf::Keyboard::Space) {
//                    spaceKeyDown = ButtonDown::RELEASE;
//                }
//                break;
//
//            case sf::Event::MouseMoved:
//                mousePos = sf::Mouse::getPosition(window);
//                break;
//            case sf::Event::MouseButtonPressed:
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    leftMouseClock.restart();
//                    leftMouseDown = ButtonDown::PRESS;
//                }
//                else if (event.mouseButton.button == sf::Mouse::Right) {
//                    rightMouseClock.restart();
//                    rightMouseDown = ButtonDown::PRESS;
//                }
//                mousePos = sf::Mouse::getPosition(window);
//                break;
//            case sf::Event::MouseButtonReleased:
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    leftMouseDown = ButtonDown::RELEASE;
//                }
//                else if (event.mouseButton.button == sf::Mouse::Right) {
//                    rightMouseDown = ButtonDown::RELEASE;
//                }
//
//            default:
//                mousePos = sf::Mouse::getPosition(window);
//                if (leftMouseDown == ButtonDown::PRESS) {
//                    leftMouseDown = ButtonDown::HOLD;
//                }
//                if (rightMouseDown == ButtonDown::PRESS) {
//                    rightMouseDown = ButtonDown::HOLD;
//                }
//            }
//
//        }
//
//        if (!windowClosed) {
//
//            // Updating based on inputs
//            mouseFloat = sf::Vector2f(mousePos);
//            // For each button, highlight it if it is clickable (mouse within borders)
//            if (playPauseRect.contains(mouseFloat)) {
//                playPauseButton.setFillColor(sf::Color::Yellow);
//                playPauseClickable = true;
//                soapSelection.setFillColor(sf::Color::White);
//                soapSelectionClickable = false;
//                sanitizerSelection.setFillColor(sf::Color::White);
//                sanitizerSelectionClickable = false;
//            }
//            else if (soapSelectionRect.contains(mouseFloat)) {
//                playPauseButton.setFillColor(sf::Color::White);
//                playPauseClickable = false;
//                soapSelection.setFillColor(sf::Color::Yellow);
//                soapSelectionClickable = true;
//                sanitizerSelection.setFillColor(sf::Color::White);
//                sanitizerSelectionClickable = false;
//            }
//            else if (sanitizerSelectionRect.contains(mouseFloat)) {
//                playPauseButton.setFillColor(sf::Color::White);
//                playPauseClickable = false;
//                soapSelection.setFillColor(sf::Color::White);
//                soapSelectionClickable = false;
//                sanitizerSelection.setFillColor(sf::Color::Yellow);
//                sanitizerSelectionClickable = true;
//            }
//            else {
//                playPauseButton.setFillColor(sf::Color::White);
//                playPauseClickable = false;
//                soapSelection.setFillColor(sf::Color::White);
//                soapSelectionClickable = false;
//                sanitizerSelection.setFillColor(sf::Color::White);
//                sanitizerSelectionClickable = false;
//            }
//            drawSoapDescription = soapSelectionClickable;
//            drawSanitizerDescription = sanitizerSelectionClickable;
//
//            // space key pauses game
//            if (spaceKeyDown == ButtonDown::PRESS && spaceKeyPressable
//                && playPauseClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
//                //Play the click sound
//                mouseClickSound.play();
//
//                playPauseClock.restart();
//                // Round messages will go away when player clicks play
//                drawMessageText = false;
//                if (paused) {
//                    paused = false;
//                    playPauseButton.setFillColor(sf::Color::Red);
//                    playPauseButton.setTexture(&pauseTexture);
//                }
//                else {
//                    paused = true;
//                    playPauseButton.setFillColor(sf::Color::Red);
//                    playPauseButton.setTexture(&playTexture);
//                }
//            }
//            if (spaceKeyDown == ButtonDown::RELEASE
//                && spaceKeyClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
//                spaceKeyPressable = true;
//            }
//            else {
//                spaceKeyPressable = false;
//            }
//
//            // We can click buttons and place towers while paused
//            // Determine if something was clicked
//            if (leftMouseDown == ButtonDown::PRESS && leftMousePressable) {
//                // Clicked the play/pause button
//                if (playPauseClickable && playPauseClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
//                    // Restart the clickable timer
//                    playPauseClock.restart();
//
//                    //Play the click sound
//                    mouseClickSound.play();
//
//                    // Round messages will go away when player clicks play
//                    drawMessageText = false;
//                    if (paused) {
//                        paused = false;
//                        playPauseButton.setFillColor(sf::Color::Red);
//                        playPauseButton.setTexture(&pauseTexture);
//                    }
//                    else {
//                        paused = true;
//                        playPauseButton.setFillColor(sf::Color::Red);
//                        playPauseButton.setTexture(&playTexture);
//                    }
//                }
//                // Clicked the sanitizer tower selection button
//                else if (sanitizerSelectionClickable
//                         && sanitizerSelectionClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
//                    // Restart the clickable timer
//                    sanitizerSelectionClock.restart();
//
//                    if (player.getMoney() >= prices::SANITIZER_PRICE) {
//                        // Play the click sound
//                        mouseClickSound.play();
//
//                        placeWhat = PlaceTower::SANITIZER;
//                    }
//                    else {
//                        // Can't click (not enough money)
//                        wrongClickSound.play();
//
//                        sanitizerSelection.setFillColor(sf::Color::Red);
//                    }
//                }
//                // Clicked the soap tower selection button
//                else if (soapSelectionClickable
//                         && soapSelectionClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
//                    // Restart the clickable timer
//                    soapSelectionClock.restart();
//
//                    if (player.getMoney() >= prices::SOAP_PRICE) {
//                        //Play the click sound
//                        mouseClickSound.play();
//                        placeWhat = PlaceTower::SOAP;
//                    }
//                    else {
//                        // Can't click (not enough money)
//                        wrongClickSound.play();
//
//                        soapSelection.setFillColor(sf::Color::Red);
//                    }
//                }
//                // Clicked to place a sanitizer tower
//                else if (placeWhat == PlaceTower::SANITIZER) {
//                    placeTowerConfirmed = true;
//                }
//                // Clicked to place a soap tower
//                else if (placeWhat == PlaceTower::SOAP) {
//                    placeTowerConfirmed = true;
//                }
//            }
//            if (leftMouseDown == ButtonDown::RELEASE
//                && leftMouseClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
//                leftMousePressable = true;
//            }
//            else {
//                leftMousePressable = false;
//            }
//
//            if (rightMouseDown == ButtonDown::PRESS && rightMousePressable) {
//                if (placeWhat != PlaceTower::NONE) {
//                    mouseClickSound.play();
//                    placeWhat = PlaceTower::NONE;
//                }
//            }
//            if (rightMouseDown == ButtonDown::RELEASE
//                && rightMouseClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
//                rightMousePressable = true;
//            }
//            else {
//                rightMousePressable = false;
//            }
//
//            switch (placeWhat) {
//            case PlaceTower::NONE:
//                drawSanitizerIndicator = false;
//                drawSoapIndicator = false;
//                placeTowerConfirmed = false;
//                break;
//            case PlaceTower::SANITIZER:
//                try { //try placing a sanitizer tower
//                    // show an indicator for the Tower
//                    drawSanitizerIndicator = true;
//
//                    // Center on mouse
//                    sanitizerIndicator.getRect().setPosition(mouseFloat);
//                    // Show the indicator as green by default
//                    sanitizerIndicator.getRect().setFillColor(sf::Color::Green);
//
//
//                    sf::Vector2f place = checkTowerPlacement(mouseFloat, &gameMap, towerVec);
//                    if (placeTowerConfirmed) {
//                        // Add the tower to the towerVec
//                        Tower t(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT),
//                                place, "sanitizer");
//                        t.getRect().setTexture(&sanitizerTexture);
//                        towerVec.push_back(t);
//
//                        // Pay for the tower
//                        //player.loseMoney(prices::SANITIZER_PRICE);
//                        cashRegisterSound.play();
//
//                        // reset values
//                        drawSanitizerIndicator = false;
//                        placeWhat = PlaceTower::NONE;
//                        placeTowerConfirmed = false;
//                    }
//                }
//                catch (int a) {
//                    sanitizerIndicator.getRect().setFillColor(sf::Color::Red);
//                    if (placeTowerConfirmed) {
//                        // Player tried to confirm placing a tower
//                        wrongClickSound.play();
//                        placeTowerConfirmed = false;
//                    }
//                }
//                catch (...) {}
//                break;
//            case PlaceTower::SOAP:
//                try { //try placing a soap tower
//                    // show an indicator for the Tower
//                    drawSoapIndicator = true;
//
//                    // Center on mouse
//                    soapIndicator.getRect().setPosition(mouseFloat);
//                    // Show the indicator as green by default
//                    soapIndicator.getRect().setFillColor(sf::Color::Green);
//
//                    sf::Vector2f place = checkTowerPlacement(mouseFloat, &gameMap, towerVec);
//                    if (placeTowerConfirmed) {
//                        // Add the tower to the towerVec
//                        Tower t(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), place, "soap");
//                        t.getRect().setTexture(&soapTexture);
//                        towerVec.push_back(t);
//
//                        // Pay for the tower
//                        //player.loseMoney(prices::SOAP_PRICE);
//                        cashRegisterSound.play();
//
//                        // reset values
//                        drawSoapIndicator = false;
//                        placeWhat = PlaceTower::NONE;
//                        placeTowerConfirmed = false;
//                    }
//                }
//                catch (int a) {
//                    soapIndicator.getRect().setFillColor(sf::Color::Red);
//                    if (placeTowerConfirmed) {
//                        // Player tried to confirm placing a tower
//                        wrongClickSound.play();
//                        placeTowerConfirmed = false;
//                    }
//                }
//                catch (...) {}
//                break;
//            default:
//                placeWhat = PlaceTower::NONE;
//            }
//
//
//            // Update enemies, bullets (only update while game is not paused)
//            if (!paused) {
//                // enemies and bullets
//            }
//
//            //update animations
//            soapAnimation.Update(0, animationDeltaTime);
//            sanitizerAnimation.Update(0, animationDeltaTime);
//
//            //(for animation) set texture rectangle for all towers
//            //tower.getRect().setTextureRect(soapAnimation.uvRect); //step through the array/vector and use the same element # for the corresponding player and animation objects
//            for (unsigned i = 0; i < towerVec.size(); i++) {
//                towerVec[i].getRect().setTextureRect(sanitizerAnimation.uvRect);
//            }
//            //player1.getRect().setTextureRect(sanitizerAnimation.uvRect);
//
//
//
//            window.clear();
//
//            // Drawing
//            gameMap.draw(window);
//    //        if (drawCovid) {
//    //            window.draw(covid);
//    //        }
//            player.draw(window);
//
//            //draw bullets to window
//            for (unsigned i = 0; i < bulletVec.size(); i++) {
//                bulletVec[i].draw(window);
//                bulletVec[i].fire();
//            }
//
//
//            //draw all objects to window
//            for (unsigned i = 0; i < towerVec.size(); i++) {
//                towerVec[i].draw(window);
//            }
//            window.draw(playPauseButton);
//            window.draw(sanitizerSelection);
//            window.draw(soapSelection);
//            if (drawSoapDescription) {
//                window.draw(soapDescription);
//            }
//            if (drawSanitizerDescription) {
//                window.draw(sanitizerDescription);
//            }
//            if (drawMessageText) {
//                window.draw(messageText);
//            }
//            if (drawSoapIndicator) {
//                soapIndicator.draw(window);
//            }
//            if (drawSanitizerIndicator) {
//                sanitizerIndicator.draw(window);
//            }
//            window.display();
//        }
//    }
}

sf::Vector2f GamePlayScreen::checkTowerPlacement(const sf::Vector2f& mouseFloat,
                                                 GameMap* const gameMapPtr,
                                                 std::vector<Tower>& towerVec) {
    if (!gameMapPtr->getGlobalBounds().contains(mouseFloat)) { // outside boundaries of game map
        throw 1;
    }
    Cell* selected = gameMapPtr->getCells()[static_cast<int>(mouseFloat.y / game_map::SIDE_FLT)]
                                           [static_cast<int>(mouseFloat.x / game_map::SIDE_FLT)];
    if (selected->getCellType() == CellEnum::GROUND) {
        // Check if tower already in towerVec
        for (unsigned i = 0; i < towerVec.size(); i++) {
            if (towerVec[i].getPosition() == selected->getPosition()) {
                throw 1;
            }
        }

        return selected->getPosition();
    }
    else {
        // tower placement is on a Path
        throw 1;
    }
}
