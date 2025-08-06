#include "Player.h"
#include "GameBotMedium.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

MediumMode::MediumMode(sf::RenderWindow& window) : window(window), player("assets/images/player_1.png", "player_1", sf::Vector2f(100, 400)), bot("assets/images/mediumbot2.png", "mediumbot2", sf::Vector2f(500, 320)) {
   window.setFramerateLimit(60); // Giới hạn FPS để đồng bộ
    // Load background texture
    if (!backgroundTexture.loadFromFile("assets/images/mediumbg.jpg")) {
        background.setSize(sf::Vector2f(800, 600));
        background.setFillColor(sf::Color(150, 75, 0)); // Màu nâu nếu texture thất bại
    } else {
        background.setTexture(&backgroundTexture);
        background.setSize(sf::Vector2f(800, 600));
    }

    // Thiết lập thanh máu (giả định ban đầu là full)
    /*playerHealthBar.setSize(sf::Vector2f(200, 20)); // Giả định hp ban đầu là 200
    playerHealthBar.setFillColor(sf::Color::Red);
    playerHealthBar.setPosition(50, 50);

    botHealthBar.setSize(sf::Vector2f(200, 20)); // Giả định hp ban đầu là 200
    botHealthBar.setFillColor(sf::Color::Red);
    botHealthBar.setPosition(550, 50);*/

    // Load font
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    std::cout << "Không thể load font arial.ttf, kiểm tra lại đường dẫn và file.\n";
}

    vsText.setFont(font);
    vsText.setString("VS");
    vsText.setCharacterSize(40);
    vsText.setFillColor(sf::Color::White);
    vsText.setPosition(375, 30);
}

void MediumMode::runGameWithBot_Medium() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds(); // Sử dụng deltaTime động
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (!gameEnded) {
                player.handleInput(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, sf::Keyboard::J, sf::Keyboard::K, true);
            }
        }
        if (!gameEnded) {
            player.update(deltaTime);
            aiControlBot(deltaTime);
            bot.update(deltaTime);

            bool isColliding = player.getBounds().intersects(bot.getBounds());

            if (isColliding && player.isCurrentlyAttacking() && player.getAttackClock().getElapsedTime().asSeconds() > 0.5f) {
                bot.takeDamage(10);
                bot.setHit();
                player.getAttackClock().restart();
                std::cout << "Player tấn công Bot: HP Bot = " << bot.getHealth() << std::endl;
            }

            if (isColliding && bot.isCurrentlyAttacking() && bot.getAttackClock().getElapsedTime().asSeconds() > 0.5f) {
                player.takeDamage(10);
                player.setHit();
                bot.getAttackClock().restart();
                std::cout << "Bot tấn công Player: HP Player = " << player.getHealth() << std::endl;
            }

            playerHealthBar.setSize(sf::Vector2f(player.getHealth(), 20));
            botHealthBar.setSize(sf::Vector2f(bot.getHealth(), 20));

            if (player.isDead()) {
                gameOverText.setString("Bot Wins");
                gameEnded = true;
            } else if (bot.isDead()) {
                gameOverText.setString("Player Wins");
                gameEnded = true;
            }
        }


        // AI cho bot ở chế độ medium
        //aiControlBot();

        // Va chạm và sát thương
        /*if (player.getBounds().intersects(bot.getBounds())) {
            player.takeDamage(10);
            bot.takeDamage(10);
            player.setHit();
            bot.setHit();
        }*/

         /*if (player.getBounds().intersects(bot.getBounds()) && player.getDamageCooldown().getElapsedTime().asSeconds() > 0.5f) {
            player.takeDamage(5); // Giảm sát thương từ 10 xuống 5
            bot.takeDamage(5);
            player.setHit();
            bot.setHit();
            player.getDamageCooldown().restart();
            bot.getDamageCooldown().restart();
            std::cout << "Collision: Player1 HP = " << player.getHealth() << ", Bot HP = " << bot.getHealth() << std::endl;
        }*/

        /*if (player.getDamageCooldown().getElapsedTime().asSeconds() > 0.5f) {
                player.takeDamage(5);
                bot.takeDamage(5);
                player.setHit();
                bot.setHit();
                player.getDamageCooldown().restart();
                bot.getDamageCooldown().restart();
                std::cout << "Collision: Player1 HP = " << player.getHealth() << ", Bot HP = " << bot.getHealth() << std::endl;
            }*/
        window.clear();
        window.draw(background);
        window.draw(playerHealthBar);
        window.draw(botHealthBar);
        window.draw(vsText);
        player.render(window);
        bot.render(window);
        if (gameEnded) window.draw(gameOverText);
        window.display();
    }
}

sf::Vector2f MediumMode::getOverlap(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
    float overlapX = std::min(rect1.left + rect1.width, rect2.left + rect2.width) - std::max(rect1.left, rect2.left);
    float overlapY = std::min(rect1.top + rect1.height, rect2.top + rect2.height) - std::max(rect1.top, rect2.top);
    return sf::Vector2f(overlapX, overlapY);
}

void MediumMode::aiControlBot(float deltaTime) {
    sf::FloatRect playerBounds = player.getBounds();
    sf::FloatRect botBounds = bot.getBounds();
    float distanceX = playerBounds.left - botBounds.left;
    bool botOnGround = bot.getVelocity().y == 0 && bot.getSprite().getPosition().y + bot.getSprite().getGlobalBounds().height >= 600;

    int actionChance = rand() % 100;

    // Nếu khoảng cách lớn, ưu tiên di chuyển đến gần player
    if (abs(distanceX) > 150) {
        if (distanceX > 0) {
            bot.setAction(Player::ActionType::RunRight);
        } else {
            bot.setAction(Player::ActionType::RunLeft);
        }
    }
    // Nếu khoảng cách trung bình, kết hợp di chuyển và tấn công
    else if (abs(distanceX) > 50) {
        if (actionChance < 20 && bot.getAttackClock().getElapsedTime().asSeconds() > 0.5f) {
            bot.setAction(rand() % 2 == 0 ? Player::ActionType::Punch : Player::ActionType::Kick);
        } else if (actionChance < 40 && botOnGround) {
            bot.setAction(Player::ActionType::Jump);
        } else {
            if (distanceX > 0) {
                bot.setAction(Player::ActionType::RunRight);
            } else {
                bot.setAction(Player::ActionType::RunLeft);
            }
        }
    }
    // Nếu gần player, ưu tiên tấn công
    else {
        if (actionChance < 60 && bot.getAttackClock().getElapsedTime().asSeconds() > 0.5f) {
            bot.setAction(rand() % 2 == 0 ? Player::ActionType::Punch : Player::ActionType::Kick);
        } else if (actionChance < 80 && botOnGround) {
            bot.setAction(Player::ActionType::Jump);
        } else {
            bot.setAction(Player::ActionType::Idle);
        }
    }
}