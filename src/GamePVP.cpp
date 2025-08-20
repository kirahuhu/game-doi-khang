#include "GamePVP.h"
#include "GameMode.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>


PVPMode::PVPMode(sf::RenderWindow& win)
    : BaseGameMode(win,
                   "assets/images/player_2.png", // Không cần bot texture
                   "assets/images/background.jpg",
                   "assets/sounds/pvp.ogg") 
{
    // Thay bot bằng player2
    bot = Player("assets/images/player_2.png", "player_2", sf::Vector2f(500.f, 400.f));


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
                return; // thoát về menu
        }

        if (!gameEnded) {
            // Player1
            player.handleInput(sf::Keyboard::A, sf::Keyboard::D,
                               sf::Keyboard::W, sf::Keyboard::Q,
                               sf::Keyboard::E, true);
            // Player2
            bot.handleInput(sf::Keyboard::Left, sf::Keyboard::Right,
                            sf::Keyboard::Up, sf::Keyboard::RControl,
                            sf::Keyboard::RShift, false);

            player.update(deltaTime);
            bot.update(deltaTime);

            bool isColliding = player.getBounds().intersects(bot.getBounds());

            // Player1 attacks
            if (isColliding && (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::E))) {
                if (attackClock1.getElapsedTime().asSeconds() > 0.5f) {
                    bot.takeDamage(10);
                    bot.setHit();
                    attackClock1.restart();
                }
            }

            // Player2 attacks
            if (isColliding && (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))) {
                if (attackClock2.getElapsedTime().asSeconds() > 0.5f) {
                    player.takeDamage(10);
                    player.setHit();
                    attackClock2.restart();
                }
            }

            playerHealthBar.setSize(sf::Vector2f(player.getHealth(), 20));
            botHealthBar.setSize(sf::Vector2f(bot.getHealth(), 20));

            if (player.isDead()) {
                gameOverText.setString("Player 2 Wins");
                gameEnded = true;
            } else if (bot.isDead()) {
                gameOverText.setString("Player 1 Wins");
                gameEnded = true;
            }
        }

        window.clear();
        window.draw(background);
        player.render(window);
        bot.render(window);
        window.draw(playerHealthBar);
        window.draw(botHealthBar);
        window.draw(vsText);
        window.draw(exitText);

        if (gameEnded) window.draw(gameOverText);
        window.display();
    }
}

void PVPMode::update(float dt) {
    // Cập nhật các đối tượng (player và bot)
    player.update(dt); // Cập nhật player 1
    bot.update(dt);    // Cập nhật player 2

    bool isColliding = player.getBounds().intersects(bot.getBounds());

    if (isColliding && player.isCurrentlyAttacking() && player.getAttackClock().getElapsedTime().asSeconds() > 0.3f) {
        bot.takeDamage(10); // Giảm máu bot nếu player tấn công
        bot.setHit();       // Đánh dấu bot bị trúng đòn
        player.getAttackClock().restart(); // Khởi động lại cooldown của player
    }

    if (isColliding && bot.isCurrentlyAttacking() && bot.getAttackClock().getElapsedTime().asSeconds() > 0.3f) {
        player.takeDamage(10); // Giảm máu player nếu bot tấn công
        player.setHit();       // Đánh dấu player bị trúng đòn
        bot.getAttackClock().restart(); // Khởi động lại cooldown của bot
    }

    // Cập nhật thanh máu của player và bot
    playerHealthBar.setSize(sf::Vector2f(player.getHealth(), 20));
    botHealthBar.setSize(sf::Vector2f(bot.getHealth(), 20));

    // Kiểm tra nếu một trong các player chết
    if (player.isDead()) {
        gameOverText.setString("Player 2 Wins");
        gameEnded = true;
    } else if (bot.isDead()) {
        gameOverText.setString("Player 1 Wins");
        gameEnded = true;
    }
}

void PVPMode::render() {
    window.clear(); // Xóa màn hình

    // Vẽ các đối tượng lên cửa sổ
    window.draw(background); // Vẽ nền
    player.render(window);   // Vẽ player 1
    bot.render(window);      // Vẽ player 2
    window.draw(playerHealthBar); // Vẽ thanh máu player 1
    window.draw(botHealthBar);    // Vẽ thanh máu player 2

    // Vẽ các thông báo
    window.draw(vsText);       // Vẽ chữ "VS"
    window.draw(exitText);     // Vẽ thông báo thoát game
    if (gameEnded) window.draw(gameOverText); // Vẽ thông báo kết thúc game

    window.display(); // Cập nhật cửa sổ
}

