#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include <iostream>

void runGamePVP(sf::RenderWindow& window) {
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Không thể tải font!\n";
    }

    sf::RectangleShape player1HealthBar;
    sf::RectangleShape player2HealthBar;

    player1HealthBar.setSize(sf::Vector2f(1000, 20));
    player1HealthBar.setFillColor(sf::Color::Red);
    player1HealthBar.setPosition(50, 50);

    player2HealthBar.setSize(sf::Vector2f(200, 20));
    player2HealthBar.setFillColor(sf::Color::Red);
    player2HealthBar.setPosition(550, 50);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(200.f, 200.f);

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setCharacterSize(20);
    exitText.setFillColor(sf::Color::Yellow);
    exitText.setString("EXIT: O");
    exitText.setPosition(10.f, 10.f);

    sf::Music bgMusic;
    if (!bgMusic.openFromFile("assets/sounds/pvp.ogg")) {
        std::cerr << "Không thể tải nhạc nền pvp.ogg\n";
    } else {
        bgMusic.setLoop(true);  // lặp vô hạn
        bgMusic.setVolume(50);  // âm lượng 0-100
        bgMusic.play();
    }

    bool gameEnded = false;

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("assets/images/background.jpg")) {
        std::cerr << "Không thể load background.jpg\n";
    }
    sf::Sprite bgSprite(bgTexture);
    sf::Vector2u bgSize = bgTexture.getSize();
    bgSprite.setScale(800.f / bgSize.x, 600.f / bgSize.y);

    Player player1("assets/images/player_1.png", "player_1", sf::Vector2f(200.f, 400.f));
    Player player2("assets/images/player_2.png", "player_2", sf::Vector2f(500.f, 400.f));

    sf::Clock clock, attackClock1, attackClock2;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::O) {
                    return; // Chỉ thoát về menu, không đóng cửa sổ
                }
            }
        }

        if (!gameEnded) {
            player1.handleInput(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W,
                                sf::Keyboard::Q, sf::Keyboard::E, true);

            player2.handleInput(sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up,
                                sf::Keyboard::RControl, sf::Keyboard::RShift, false);

            player1.update(deltaTime);
            player2.update(deltaTime);

            bool isColliding = player1.getBounds().intersects(player2.getBounds());

            if (isColliding && (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::E))) {
                if (attackClock1.getElapsedTime().asSeconds() > 0.5f) {
                    player2.takeDamage(10);
                    player2.setHit();
                    attackClock1.restart();
                }
            }

            if (isColliding && (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))) {
                if (attackClock2.getElapsedTime().asSeconds() > 0.5f) {
                    player1.takeDamage(10);
                    player1.setHit();
                    attackClock2.restart();
                }
            }

            player1HealthBar.setSize(sf::Vector2f(player1.getHealth(), 20));
            player2HealthBar.setSize(sf::Vector2f(player2.getHealth(), 20));

            if (player1.isDead()) {
                gameOverText.setString("Player 2 Wins");
                gameEnded = true;
            } else if (player2.isDead()) {
                gameOverText.setString("Player 1 Wins");
                gameEnded = true;
            }
        }

        window.clear();
        window.draw(bgSprite);
        player1.render(window);
        player2.render(window);
        window.draw(player1HealthBar);
        window.draw(player2HealthBar);
        window.draw(exitText);
        if (gameEnded) window.draw(gameOverText);
        window.display();
    }
}
