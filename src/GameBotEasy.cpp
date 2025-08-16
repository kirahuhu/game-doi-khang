#include <iostream>
#include <cstdlib>
#include <cmath>
#include "GameBotEasy.h"


EasyMode::EasyMode(sf::RenderWindow& win)
    : gameWindow(win),
      player("assets/images/player_1.png", "player_1", sf::Vector2f(100, 400)),
      bot("assets/images/easybot.png", "easybot", sf::Vector2f(500, 320)) 
{
    gameWindow.setFramerateLimit(60);
    
    if (!bgMusic.openFromFile("assets/sounds/easymode.ogg")) {
        std::cerr << "Không thể tải nhạc nền easymode.ogg\n";
    } else {
        bgMusic.setLoop(true);
        bgMusic.setVolume(50);
        bgMusic.play();
    }
    
    if (!backgroundTexture.loadFromFile("assets/images/easybg.jpg")) {
        background.setSize(sf::Vector2f(800, 600));
        background.setFillColor(sf::Color(80, 80, 80));
    } else {
        background.setTexture(&backgroundTexture);
        background.setSize(sf::Vector2f(800, 600));
    }

    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Không thể load font arial.ttf\n";
    }

    vsText.setFont(font);
    vsText.setString("VS");
    vsText.setCharacterSize(40);
    vsText.setFillColor(sf::Color::White);
    vsText.setPosition(375, 30);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(250, 300);

    playerHealthBar.setSize(sf::Vector2f(200, 20));
    playerHealthBar.setFillColor(sf::Color::Red);
    playerHealthBar.setPosition(50, 50);

    botHealthBar.setSize(sf::Vector2f(200, 20));
    botHealthBar.setFillColor(sf::Color::Red);
    botHealthBar.setPosition(550, 50);

    gameEnded = false;
}

void EasyMode::runGameWithBot_Easy() {
     while (gameWindow.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                bgMusic.stop(); // dừng nhạc khi thoát
                gameWindow.close();
            }

            // ✨ Thoát về menu khi nhấn O
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O) {
                bgMusic.stop(); // dừng nhạc khi thoát về menu
                return; // quay về menu
            }

            if (!gameEnded) {
                player.handleInput(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W,
                                   sf::Keyboard::J, sf::Keyboard::K, true);
            }
        }


        if (!gameEnded) {
            player.update(deltaTime);
            aiControlBot(deltaTime);
            bot.update(deltaTime);

            bool isColliding = player.getBounds().intersects(bot.getBounds());

            if (isColliding && player.isCurrentlyAttacking() &&
                player.getAttackClock().getElapsedTime().asSeconds() > 0.3f) {
                bot.takeDamage(8);
                bot.setHit();
                player.getAttackClock().restart();
            }

            if (isColliding && bot.isCurrentlyAttacking() &&
                bot.getAttackClock().getElapsedTime().asSeconds() > 0.3f) {
                player.takeDamage(8);
                player.setHit();
                bot.getAttackClock().restart();
            }

            playerHealthBar.setSize(sf::Vector2f(player.getHealth(), 20));
            botHealthBar.setSize(sf::Vector2f(bot.getHealth(), 20));

            if (player.isDead()) {
                gameOverText.setString("EasyBot Wins");
                gameEnded = true;
            } else if (bot.isDead()) {
                gameOverText.setString("Player Wins");
                gameEnded = true;
            }
        }
        exitText.setFont(font);
		exitText.setString("EXIT: O");
		exitText.setCharacterSize(20);
		exitText.setFillColor(sf::Color::White);
		exitText.setPosition(10, 570); // góc dưới bên trái


        gameWindow.clear();
        gameWindow.draw(background);
        gameWindow.draw(playerHealthBar);
        gameWindow.draw(botHealthBar);
        gameWindow.draw(vsText);
        player.render(gameWindow);
        bot.render(gameWindow);
        if (gameEnded) gameWindow.draw(gameOverText);
        gameWindow.draw(exitText); 
        gameWindow.display();
    }
}

void EasyMode::aiControlBot(float deltaTime) {
    sf::FloatRect playerBounds = player.getBounds();
    sf::FloatRect botBounds = bot.getBounds();
    float distanceX = playerBounds.left - botBounds.left;
    bool botOnGround = bot.getVelocity().y == 0 &&
        bot.getSprite().getPosition().y + bot.getSprite().getGlobalBounds().height >= 600;

    int actionChance = rand() % 100;

    if (std::abs(distanceX) > 120) {
        bot.setAction(distanceX > 0 ? Player::ActionType::RunRight : Player::ActionType::RunLeft);
    }
    else if (std::abs(distanceX) > 40) {
        if (actionChance < 20 && bot.getAttackClock().getElapsedTime().asSeconds() > 2.5f) {
            bot.setAction(rand() % 2 == 0 ? Player::ActionType::Punch : Player::ActionType::Kick);
        } else if (actionChance < 30 && botOnGround) {
            bot.setAction(Player::ActionType::Jump);
        } else {
            bot.setAction(Player::ActionType::Idle);
        }
    }
    else {
        if (bot.getAttackClock().getElapsedTime().asSeconds() > 2.5f) {
            bot.setAction(rand() % 2 == 0 ? Player::ActionType::Punch : Player::ActionType::Kick);
        } else {
            bot.setAction(Player::ActionType::Idle);
        }
    }
}
