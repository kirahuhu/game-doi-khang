#include "GamePVP.h"
#include "GameMode.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

PVPMode::PVPMode(sf::RenderWindow& win)
    : BaseGameMode(win,
                   "assets/images/player_2.png",
                   "assets/images/background.jpg",
                   "assets/sounds/pvp.ogg"),
      player1(player),   // alias cho player từ BaseGameMode
      player2(bot)       // alias cho bot nhưng gọi là player2
{
    // Khởi tạo player2 (thay vì bot)
    player2 = Player("assets/images/player_2.png", "player_2", sf::Vector2f(500.f, 400.f));
}

void PVPMode::aiControlBot(float dt) {
    // PVP không cần AI
}

void PVPMode::runGame() {
    sf::Clock clock, attackClock1, attackClock2;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O)
                return; // Thoát về menu
        }

        if (!gameEnded) {
            // Player1
            player1.handleInput(sf::Keyboard::A, sf::Keyboard::D,
                                sf::Keyboard::W, sf::Keyboard::Q,
                                sf::Keyboard::E, true);

            // Player2
            player2.handleInput(sf::Keyboard::Left, sf::Keyboard::Right,
                                sf::Keyboard::Up, sf::Keyboard::RControl,
                                sf::Keyboard::RShift, false);

            player1.update(deltaTime);
            player2.update(deltaTime);

            bool isColliding = player1.getBounds().intersects(player2.getBounds());

            // Player1 attacks
            if (isColliding && (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::E))) {
                if (attackClock1.getElapsedTime().asSeconds() > 0.5f) {
                    player2.takeDamage(10);
                    player2.setHit();
                    attackClock1.restart();
                }
            }

            // Player2 attacks
            if (isColliding && (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))) {
                if (attackClock2.getElapsedTime().asSeconds() > 0.5f) {
                    player1.takeDamage(10);
                    player1.setHit();
                    attackClock2.restart();
                }
            }

            playerHealthBar.setSize(sf::Vector2f(player1.getHealth(), 20));
            botHealthBar.setSize(sf::Vector2f(player2.getHealth(), 20));

            if (player1.isDead()) {
                gameOverText.setString("Player 2 Wins");
                gameEnded = true;
            } else if (player2.isDead()) {
                gameOverText.setString("Player 1 Wins");
                gameEnded = true;
            }
        }

        window.clear();
        window.draw(background);
        player1.render(window);
        player2.render(window);
        window.draw(playerHealthBar);
        window.draw(botHealthBar);
        window.draw(vsText);
        window.draw(exitText);

        if (gameEnded) window.draw(gameOverText);
        window.display();
    }
}

void PVPMode::update(float dt) {
    player1.update(dt);
    player2.update(dt);

    bool isColliding = player1.getBounds().intersects(player2.getBounds());

    if (isColliding && player1.isCurrentlyAttacking() && player1.getAttackClock().getElapsedTime().asSeconds() > 0.3f) {
        player2.takeDamage(10);
        player2.setHit();
        player1.getAttackClock().restart();
    }

    if (isColliding && player2.isCurrentlyAttacking() && player2.getAttackClock().getElapsedTime().asSeconds() > 0.3f) {
        player1.takeDamage(10);
        player1.setHit();
        player2.getAttackClock().restart();
    }

    playerHealthBar.setSize(sf::Vector2f(player1.getHealth(), 20));
    botHealthBar.setSize(sf::Vector2f(player2.getHealth(), 20));

    if (player1.isDead()) {
        gameOverText.setString("Player 2 Wins");
        gameEnded = true;
    } else if (player2.isDead()) {
        gameOverText.setString("Player 1 Wins");
        gameEnded = true;
    }
}

void PVPMode::render() {
    window.clear();

    window.draw(background);
    player1.render(window);
    player2.render(window);
    window.draw(playerHealthBar);
    window.draw(botHealthBar);

    window.draw(vsText);
    window.draw(exitText);
    if (gameEnded) window.draw(gameOverText);

    window.display();
}
