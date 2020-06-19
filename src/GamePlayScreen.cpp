#include "GamePlayScreen.h"

#include "Collision.h"
#include "enemyBase.h"
#include "corona.h"

#include <deque>
#include <cstdlib>

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
const std::string BACKDOOR_INSTRUCTIONS {
        "=====BACKDOOR ENABLED: YOU'RE IN=====\n"
        "l - enable unlimited lives\n"
        "m - enable unlimited money\n"
        "1-5 - set wave number 1-5\n"
    };
}

namespace input {
const float CLICK_DELAY = 0.2f;
}

void GamePlayScreen::draw(sf::RenderWindow& window) {
    player.setDrawOrigin(draw::PLAYER_DRAW_ORIGIN);

    bool backDoorEnabled = (player.getName() == std::string("backdoor") ? true : false);
    bool unlimitedLives = false;
    bool unlimitedMoney = false;


    //MAP MAIN STUFF**********************************************************

    GameMap gameMap(std::string("map_1_data"), draw::GAME_MAP_ORIGIN);

    // Enemy sprite texture
    sf::Texture covidTexture;
    std::string covidTextureFile(game_map::RESOURCE_PATH + "covid_1.png");
    loadTexture(covidTexture, covidTextureFile);

    sf::Vector2u covidCoords = gameMap.getStartCoords();

    Path* pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);

    Corona tempEnemy(&covidTexture, &gameMap, pathPtr);

    sf::Vector2f covidDirection;
    sf::Vector2f covidDestination = draw::GAME_MAP_ORIGIN + pathPtr->getNextPosition();
    sf::Vector2f distanceToDestination(std::abs(covidDestination.x - tempEnemy.getSprite().getPosition().x),
                                       std::abs(covidDestination.y - tempEnemy.getSprite().getPosition().y));

    bool drawCovid = true;
    //MAP MAIN STUFF**********************************************************

    bool paused = true;

    bool waveGoing = false; // if the wave of enemies is still going
    int waveNumber = 0; //0 through 5
    int enemiesPushed = 0;

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
    std::vector<Bullet> bulletVec;
    std::vector<Tower> towerVec;
    std::vector<Tower> shootingTowers;
    std::deque<Corona> enemyQueue;
    enemyQueue.push_back(tempEnemy);
    bool drawBullets = false;
    bool sanitizerShooting = false;
    bool soapShooting = false;
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
    if (backDoorEnabled) {
        messageText.setString(messages::BACKDOOR_INSTRUCTIONS);
    }
    else {
        messageText.setString(messages::ROUND_MESSAGES[0]);
    }
    messageText.setFillColor(sf::Color::White);
    bool drawMessageText = true;

    // A temporary tower to show placement
    Tower* towerIndicatorPtr = nullptr;

    //loading textures for bullets
    sf::Texture squirtTexture;
    loadTexture(squirtTexture, std::string(game_map::RESOURCE_PATH + "sanitizer_squirt_rect.png"));
    sf::Texture bubbleTexture;
    loadTexture(bubbleTexture, std::string(game_map::RESOURCE_PATH
                                           + "soap_bubbles.png"));

    //loading sounds
    sf::SoundBuffer squirtBuffer;
    loadSound(squirtBuffer, std::string(game_map::RESOURCE_PATH + "squirt_sound.wav"));
    sf::Sound squirtSound;
    squirtSound.setBuffer(squirtBuffer);
    squirtSound.setVolume(10.f);
    sf::SoundBuffer bubbleBuffer;
	bubbleBuffer.loadFromFile(game_map::RESOURCE_PATH + "bubble_sound.wav");
	sf::Sound bubbleSound;
	bubbleSound.setBuffer(bubbleBuffer);
	bubbleSound.setVolume(10.f);

    sf::SoundBuffer mouseClickBuffer;
    loadSound(mouseClickBuffer, std::string(game_map::RESOURCE_PATH + "mouse_click.wav"));
    sf::Sound mouseClickSound;
    mouseClickSound.setBuffer(mouseClickBuffer);
    mouseClickSound.setVolume(1.f);

    sf::SoundBuffer wrongClickBuffer;
    loadSound(wrongClickBuffer, std::string(game_map::RESOURCE_PATH + "wrong.wav"));
    sf::Sound wrongClickSound;
    wrongClickSound.setBuffer(wrongClickBuffer);
    wrongClickSound.setVolume(10.f);

    sf::SoundBuffer cashRegisterBuffer;
    loadSound(cashRegisterBuffer, std::string(game_map::RESOURCE_PATH + "cash_register.wav"));
    sf::Sound cashRegisterSound;
    cashRegisterSound.setBuffer(cashRegisterBuffer);
    cashRegisterSound.setVolume(1.f);

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
    sf::Clock lKeyClock;
    sf::Clock mKeyClock;
    sf::Clock num1KeyClock;
    sf::Clock num2KeyClock;
    sf::Clock num3KeyClock;
    sf::Clock num4KeyClock;
    sf::Clock num5KeyClock;
    sf::Clock spaceKeyClock;
    sf::Clock leftMouseClock;
    sf::Clock rightMouseClock;
    sf::Clock playPauseClock;
    sf::Clock soapSelectionClock;
    sf::Clock sanitizerSelectionClock;

    // What kind of tower, if any, to place
    PlaceTower placeWhat = PlaceTower::NONE;
    bool placeTowerConfirmed = false;

    // For input events
    sf::Event event;
    ButtonDown lKeyDown = ButtonDown::RELEASE;
    bool lKeyPressable = false;
    ButtonDown mKeyDown = ButtonDown::RELEASE;
    bool mKeyPressable = false;
    ButtonDown num1KeyDown = ButtonDown::RELEASE;
    bool num1KeyPressable = false;
    ButtonDown num2KeyDown = ButtonDown::RELEASE;
    bool num2KeyPressable = false;
    ButtonDown num3KeyDown = ButtonDown::RELEASE;
    bool num3KeyPressable = false;
    ButtonDown num4KeyDown = ButtonDown::RELEASE;
    bool num4KeyPressable = false;
    ButtonDown num5KeyDown = ButtonDown::RELEASE;
    bool num5KeyPressable = false;
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
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::L) {
                    lKeyClock.restart();
                    lKeyDown = ButtonDown::PRESS;
                }
                else if (event.key.code == sf::Keyboard::M) {
                    mKeyClock.restart();
                    mKeyDown = ButtonDown::PRESS;
                }
                else if (event.key.code == sf::Keyboard::Num1) {
                    num1KeyClock.restart();
                    num1KeyDown = ButtonDown::PRESS;
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    num2KeyClock.restart();
                    num2KeyDown = ButtonDown::PRESS;
                }
                else if (event.key.code == sf::Keyboard::Num3) {
                    num3KeyClock.restart();
                    num3KeyDown = ButtonDown::PRESS;
                }
                else if (event.key.code == sf::Keyboard::Num4) {
                    num4KeyClock.restart();
                    num4KeyDown = ButtonDown::PRESS;
                }
                else if (event.key.code == sf::Keyboard::Num5) {
                    num5KeyClock.restart();
                    num5KeyDown = ButtonDown::PRESS;
                }
                else if (event.key.code == sf::Keyboard::Space) {
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

            // Backdoor menu items
            if (backDoorEnabled) {

                if (lKeyDown == ButtonDown::PRESS && lKeyPressable) {
                    unlimitedLives = unlimitedLives ? false : true;
                }

                if (mKeyDown == ButtonDown::PRESS && mKeyPressable) {
                    unlimitedMoney = unlimitedMoney ? false : true;
                }

                if (num1KeyDown == ButtonDown::PRESS && num1KeyPressable) {
                    waveNumber = 0;
                    paused = true;
                }

                if (num2KeyDown == ButtonDown::PRESS && num2KeyPressable) {
                    waveNumber = 1;
                    paused = true;
                }

                if (num3KeyDown == ButtonDown::PRESS && num3KeyPressable) {
                    waveNumber = 2;
                    paused = true;
                }

                if (num4KeyDown == ButtonDown::PRESS && num4KeyPressable) {
                    waveNumber = 3;
                    paused = true;
                }
                if (num5KeyDown == ButtonDown::PRESS && num5KeyPressable) {
                    waveNumber = 4;
                    paused = true;
                }

                if (lKeyDown == ButtonDown::RELEASE
                    && lKeyClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    lKeyPressable = true;
                }
                else {
                    lKeyPressable = false;
                }

                if (mKeyDown == ButtonDown::RELEASE
                    && mKeyClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    mKeyPressable = true;
                }
                else {
                    mKeyPressable = false;
                }

                if (num1KeyDown == ButtonDown::RELEASE
                    && num1KeyClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    num1KeyPressable = true;
                }
                else {
                    num1KeyPressable = false;
                }

                if (num2KeyDown == ButtonDown::RELEASE
                    && num2KeyClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    num2KeyPressable = true;
                }
                else {
                    num2KeyPressable = false;
                }

                if (num3KeyDown == ButtonDown::RELEASE
                    && num3KeyClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    num3KeyPressable = true;
                }
                else {
                    num3KeyPressable = false;
                }

                if (num4KeyDown == ButtonDown::RELEASE
                    && num4KeyClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    num4KeyPressable = true;
                }
                else {
                    num4KeyPressable = false;
                }

                if (num5KeyDown == ButtonDown::RELEASE
                    && num5KeyClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    num5KeyPressable = true;
                }
                else {
                    num5KeyPressable = false;
                }
            }

            // space key pauses game
            if (spaceKeyDown == ButtonDown::PRESS && spaceKeyPressable
                && playPauseClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                //Play the click sound
                mouseClickSound.play();

                playPauseClock.restart();
                // Round messages will go away when player clicks play
                if (!backDoorEnabled) {
                    drawMessageText = false;
                }
                if (paused) {
                    paused = false;
                    playPauseButton.setFillColor(sf::Color::Red);
                    playPauseButton.setTexture(&pauseTexture);
                    if (!waveGoing) {
                        waveGoing = true;
                    }
                }
                else {
                    paused = true;
                    playPauseButton.setFillColor(sf::Color::Red);
                    playPauseButton.setTexture(&playTexture);
                }
            }
            if (spaceKeyDown == ButtonDown::RELEASE
                && spaceKeyClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                spaceKeyPressable = true;
            }
            else {
                spaceKeyPressable = false;
            }

            // We can click buttons and place towers while paused
            // Determine if something was clicked
            if (leftMouseDown == ButtonDown::PRESS && leftMousePressable) {
                // Clicked the play/pause button
                if (playPauseClickable && playPauseClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    // Restart the clickable timer
                    playPauseClock.restart();

                    //Play the click sound
                    mouseClickSound.play();

                    // Round messages will go away when player clicks play
                    if (!backDoorEnabled) {
                        drawMessageText = false;
                    }
                    if (paused) {
                        paused = false;
                        playPauseButton.setFillColor(sf::Color::Red);
                        playPauseButton.setTexture(&pauseTexture);
                        if (!waveGoing) {
                            waveGoing = true;
                            ++waveNumber;
                        }
                    }
                    else {
                        paused = true;
                        playPauseButton.setFillColor(sf::Color::Red);
                        playPauseButton.setTexture(&playTexture);
                    }
                }
                // Clicked the sanitizer tower selection button
                else if (sanitizerSelectionClickable
                         && sanitizerSelectionClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    // Restart the clickable timer
                    sanitizerSelectionClock.restart();

                    if (player.getMoney() >= prices::SANITIZER_PRICE || unlimitedMoney) {
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
                         && soapSelectionClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
                    // Restart the clickable timer
                    soapSelectionClock.restart();

                    if (player.getMoney() >= prices::SOAP_PRICE || unlimitedMoney) {
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
                && leftMouseClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
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
                && rightMouseClock.getElapsedTime().asSeconds() > input::CLICK_DELAY) {
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
                                                   mouseFloat, "sanitizer", gameMap.getCells()[0][0]);
                    towerIndicatorPtr->setPlacedDown(false);
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
                                place, "sanitizer", getLocation(place, gameMap));
                        t.setPlacedDown(true);
                        t.getRect().setTexture(&sanitizerTexture);
                        towerVec.push_back(t);

                        // Pay for the tower
                        if (!unlimitedMoney) {
                            player.loseMoney(prices::SANITIZER_PRICE);
                        }
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
                                                  mouseFloat, "soap", gameMap.getCells()[0][0]);
                    towerIndicatorPtr->setPlacedDown(false);
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
                        Tower t(sf::Vector2f(game_map::SIDE_FLT, game_map::SIDE_FLT), place, "soap", getLocation(place, gameMap));
                        t.setPlacedDown(true);
                        t.getRect().setTexture(&soapTexture);
                        towerVec.push_back(t);

                        // Pay for the tower
                        if (!unlimitedMoney) {
                            player.loseMoney(prices::SOAP_PRICE);
                        }
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
                // Add enemies to the queue
                switch (waveNumber) {
                case 1:
                    if (enemiesPushed < 20) {
                        if (!(std::rand() % 2500)) {
                            // add enemy
                            covidCoords = gameMap.getStartCoords();
                            pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
                            Corona tempEnemy(&covidTexture, &gameMap, pathPtr);
                            enemyQueue.push_back(tempEnemy);
                            ++enemiesPushed;
                        }
                    }
                    else if (enemyQueue.empty()) {
                        player.addScore(1000);
                        player.addMoney(500);
                        waveGoing = false;
                        paused = true;
                        playPauseButton.setTexture(&playTexture);
                        enemiesPushed = 0;
                        messageText.setString(messages::ROUND_MESSAGES[waveNumber]);
                        drawMessageText = true;
                    }
                    break;
                case 2:
                    if (enemiesPushed < 40) {
                        if (!(std::rand() % 2100)) {
                            // add enemy
                            covidCoords = gameMap.getStartCoords();
                            pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
                            Corona tempEnemy(&covidTexture, &gameMap, pathPtr);
                            enemyQueue.push_back(tempEnemy);
                            ++enemiesPushed;
                        }
                    }
                    else if (enemyQueue.empty()) {
                        player.addScore(2000);
                        player.addMoney(500);
                        waveGoing = false;
                        paused = true;
                        playPauseButton.setTexture(&playTexture);
                        enemiesPushed = 0;
                        messageText.setString(messages::ROUND_MESSAGES[waveNumber]);
                        drawMessageText = true;
                    }
                    break;
                case 3:
                    if (enemiesPushed < 70) {
                        if (!(std::rand() % 1700)) {
                            // add enemy
                            covidCoords = gameMap.getStartCoords();
                            pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
                            Corona tempEnemy(&covidTexture, &gameMap, pathPtr);
                            enemyQueue.push_back(tempEnemy);
                            ++enemiesPushed;
                        }
                    }
                    else if (enemyQueue.empty()) {
                        player.addScore(3000);
                        player.addMoney(500);
                        waveGoing = false;
                        paused = true;
                        playPauseButton.setTexture(&playTexture);
                        enemiesPushed = 0;
                        messageText.setString(messages::ROUND_MESSAGES[waveNumber]);
                        drawMessageText = true;
                    }
                    break;
                case 4:
                    if (enemiesPushed < 100) {
                        if (!(std::rand() % 1400)) {
                            // add enemy
                            covidCoords = gameMap.getStartCoords();
                            pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
                            Corona tempEnemy(&covidTexture, &gameMap, pathPtr);
                            enemyQueue.push_back(tempEnemy);
                            ++enemiesPushed;
                        }
                    }
                    else if (enemyQueue.empty()) {
                        player.addScore(4000);
                        player.addMoney(500);
                        waveGoing = false;
                        paused = true;
                        playPauseButton.setTexture(&playTexture);
                        enemiesPushed = 0;
                        messageText.setString(messages::ROUND_MESSAGES[waveNumber]);
                        drawMessageText = true;
                    }
                    break;
                case 5:
                    if (enemiesPushed < 150) {
                        if (!(std::rand() % 1100)) {
                            // add enemy
                            covidCoords = gameMap.getStartCoords();
                            pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x][covidCoords.y]);
                            Corona tempEnemy(&covidTexture, &gameMap, pathPtr);
                            enemyQueue.push_back(tempEnemy);
                            ++enemiesPushed;
                        }
                    }
                    else if (enemyQueue.empty()) {
                        player.addScore(5000);
                        waveGoing = false;
                        paused = true;
                        playPauseButton.setTexture(&playTexture);
                        enemiesPushed = 0;
                        gameVictory = true;
                        gameOver = true;
                    }
                    break;

                default: waveNumber = 0;
                }
                for (auto& i : enemyQueue) {
                    if (i.getDrawCovid()) {
                        // Get values from the object
                        distanceToDestination = i.getDistanceToDestination();
                        covidDestination = i.getCovidDestination();
                        covidDirection = i.getCovidDirection();
                        covidCoords = i.getCovidCoords();
                        // Update and modify these values
                        // Check if the covid has made it to next path
                        distanceToDestination = sf::Vector2f(abs(covidDestination.x - i.getSprite().getPosition().x),
                                                             abs(covidDestination.y - i.getSprite().getPosition().y));
                        if (std::abs(distanceToDestination.x) < 1.f
                            && std::abs(distanceToDestination.y) < 1.f) {
                            pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x]
                                                                            [covidCoords.y]);
                            covidCoords = pathPtr->getNextCoords();
                            pathPtr = dynamic_cast<Path*>(gameMap.getCells()[covidCoords.x]
                                                                            [covidCoords.y]);
                            covidDestination = draw::GAME_MAP_ORIGIN + pathPtr->getNextPosition();
                            // Check if covid has made it to the end
                            if (pathPtr->getNextCoords() == gameMap.getExitCoords()) {
                                i.setDrawCovid(false);
                                player.loseScore(50);
                                if (!unlimitedLives) {
                                    player.loseLives(1);
                                }
                                if (player.getLives() == 0) {
                                    gameOver = true;
                                }
                            }
                        }
                        covidDirection = normalize(covidDestination - i.getSprite().getPosition());
                        i.getSprite().move(i.getMoveSpeed() * covidDirection);
                        // Update the enemy object
                        i.setDistanceToDestination(distanceToDestination);
                        i.setCovidCoords(covidCoords);
                        i.setCovidDestination(covidDestination);
                        i.setCovidDirection(covidDirection);
                    }
                }

                // update shooting
                if (!enemyQueue.empty()) {

                    sf::Sprite covid = enemyQueue.front().getSprite();

                    for (unsigned i = 0; i < towerVec.size(); ++i) //pass in enemies that are in the tower's range
                    {


                        unsigned startRow = 0; //these coordinates indicate a tower's range (which cells it can attack bullets in)
                        unsigned endRow = 0; //the tower's range is the surrounding cells (the 3x3 section with the tower at its center)
                        unsigned startCol = 0;
                        unsigned endCol = 0;

                        if (towerVec[i].getGroundLocation()->getCoords().x == 0)
                        {
                            startRow = 0;
                        }
                        else startRow = towerVec[i].getGroundLocation()->getCoords().x - 1;

                        if (towerVec[i].getGroundLocation()->getCoords().x == 7)
                        {
                            endRow = 7;
                        }
                        else endRow = towerVec[i].getGroundLocation()->getCoords().x + 1;

                        if (towerVec[i].getGroundLocation()->getCoords().y == 0)
                        {
                            startCol = 0;
                        }
                        else startCol = towerVec[i].getGroundLocation()->getCoords().y - 1;

                        if (towerVec[i].getGroundLocation()->getCoords().y == 7)
                        {
                            endCol = 7;
                        }
                        else endCol = towerVec[i].getGroundLocation()->getCoords().y + 1;

                        for (int r = startRow; r <= endRow; r++)
                        {
                            for (int c = startCol; c <= endCol; c++)
                            {

                                if (towerVec[i].shouldShoot() == true && towerVec[i].getHaveTarget() == false &&
                                    Collision::PixelPerfectTest(covid, gameMap.getCells()[r][c]->getSprite())
                                    && enemyQueue.front().getDrawCovid())
                                    //check if enemy is in range, if there has been a good enough gap between the last bullet shot, and if the tower already has a target or not
                                {
                                    //generate bullets (squirt dimensions: 17x4, bubble dimensions: 75x33)
                                    if (towerVec[i].getType() == "sanitizer")
                                    {
                                        Bullet newBullet("squirt", towerVec[i].getX() + towerVec[i].getOffsetX(),
                                            towerVec[i].getY() + towerVec[i].getOffsetY(), (covid.getPosition().x + (game_map::SIDE_PIX / 2))
                                            , (covid.getPosition().y + (game_map::SIDE_PIX / 2))); //shoot at first enemy in range, pass in tower for starting location of bullet
                                        newBullet.getSprite().setTexture(squirtTexture);
                                        bulletVec.push_back(newBullet);
                                        sanitizerShooting = true;
                                    }
                                    else //soap
                                    {
                                        Bullet newBullet("bubble", towerVec[i].getX() + towerVec[i].getOffsetX(),
                                            towerVec[i].getY() + towerVec[i].getOffsetY(), (covid.getPosition().x + (game_map::SIDE_PIX / 2))
                                            , (covid.getPosition().y + (game_map::SIDE_PIX / 2))); //shoot at first enemy, pass in tower for starting position
                                        newBullet.getSprite().setTexture(bubbleTexture); //add game_map::SIDE_PIX / 2 to the enemy coords so the bullet shoots at enemy's center
                                        bulletVec.push_back(newBullet);
                                        soapShooting = true;
                                    }
                                    towerVec[i].setHaveTarget(true);
                                }
                            }
                        }
                    }
                    // update bullets
                    for (unsigned i = 0; i < bulletVec.size(); i++) {
                        bulletVec[i].fire();

                        if (Collision::PixelPerfectTest(bulletVec[i].getSprite(), covid))
                        {
                            enemyQueue.front().lowerHealth();
                            if (!enemyQueue.front().getIsAlive()) {
                                enemyQueue.pop_front();
                                player.addScore(100);
                                player.addMoney(25);
                            }
                            bulletVec.erase(bulletVec.begin() + i);
                        }

                        if ((bulletVec[i].getLeft() + bulletVec[i].getSprite().getScale().x < -50 ||
                            (bulletVec[i].getTop() + bulletVec[i].getSprite().getScale().y > ((8 * game_map::SIDE_PIX) + 50))))
                            {
                                bulletVec.erase(bulletVec.begin() + i);
                            } //remove the bullet from vector if it goes off screen
                    }
                }

                for (auto& i : towerVec) {
                    i.setHaveTarget(false); // reset shooting for towers
                    i.incrementShootingGap();
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
            for (auto i = enemyQueue.begin(); i != enemyQueue.end(); ++i) {
                if (i->getDrawCovid()) {
                    window.draw(i->getSprite());
                }
            }


            //draw all towers to window
            for (unsigned i = 0; i < towerVec.size(); ++i) {
                towerVec[i].draw(window);
            }

            //draw bullets to window
            for (unsigned i = 0; i < bulletVec.size(); ++i) {
                bulletVec[i].draw(window);
            }

            if (sanitizerShooting && !paused) //only play the sound for sanitizer towers firing bullets once per loop
            {
                squirtSound.play();
                //squirtSound.stop();
                sanitizerShooting = false;
            }
            if (soapShooting && !paused) //only play the sound for soap towers firing bullets once per loop
            {
                bubbleSound.play();
                //bubbleSound.stop();
                soapShooting = false;
            }


            player.draw(window);


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
}

Cell* GamePlayScreen::getLocation(sf::Vector2f towerCoords, GameMap& map) {
	int row = towerCoords.y / game_map::SIDE_PIX;
	int col = towerCoords.x / game_map::SIDE_PIX;
	//std::cout << "here1" << std::endl;
	return map.getCells()[row][col];
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
