#include "GameBotHard.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

HardMode::HardMode(sf::RenderWindow& window)
    : window(window),
      player("assets/images/player_1.png", "player_1", sf::Vector2f(100, 400)),
      bot("assets/images/hardbot.png", "hardbot", sf::Vector2f(500, 320)) 
{
    window.setFramerateLimit(60);

    if (!backgroundTexture.loadFromFile("assets/images/hardbg.jpg")) {
        background.setSize(sf::Vector2f(800, 600));
        background.setFillColor(sf::Color(50, 50, 50));
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

    // Thêm phần này để hiện kết quả thắng/thua
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(250, 300);

    playerHealthBar.setSize(sf::Vector2f(1000, 20));
    playerHealthBar.setFillColor(sf::Color::Red);
    playerHealthBar.setPosition(50, 50);

    botHealthBar.setSize(sf::Vector2f(200, 20));
    botHealthBar.setFillColor(sf::Color::Red);
    botHealthBar.setPosition(550, 50);

    gameEnded = false;
}


void HardMode::runGameWithBot_Hard() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
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
                bot.takeDamage(12);
                bot.setHit();
                player.getAttackClock().restart();
            }

            if (isColliding && bot.isCurrentlyAttacking() &&
                bot.getAttackClock().getElapsedTime().asSeconds() > 0.3f) {
                player.takeDamage(12);
                player.setHit();
                bot.getAttackClock().restart();
            }

            playerHealthBar.setSize(sf::Vector2f(player.getHealth(), 20));
            botHealthBar.setSize(sf::Vector2f(bot.getHealth(), 20));

            if (player.isDead()) {
                gameOverText.setString("Monster Wins");
                gameEnded = true;
            } else if (bot.isDead()) {
                gameOverText.setString("Player Wins");
                gameEnded = true;
            }
        }

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

sf::Vector2f HardMode::getOverlap(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
    float overlapX = std::min(rect1.left + rect1.width, rect2.left + rect2.width) - 
                     std::max(rect1.left, rect2.left);
    float overlapY = std::min(rect1.top + rect1.height, rect2.top + rect2.height) - 
                     std::max(rect1.top, rect2.top);
    return sf::Vector2f(overlapX, overlapY);
}

void HardMode::aiControlBot(float deltaTime) {
    sf::FloatRect playerBounds = player.getBounds();
    sf::FloatRect botBounds = bot.getBounds();
    float distanceX = playerBounds.left - botBounds.left;
    bool botOnGround = bot.getVelocity().y == 0 &&
        bot.getSprite().getPosition().y + bot.getSprite().getGlobalBounds().height >= 600;
    // 👉 Thêm vào đây
    float botSpeedFactor = 0.0375f; // 0.5 nghĩa là chạy chậm còn một nửa
    int actionChance = rand() % 100;

    // Nếu Player tấn công, bot sẽ nhảy hoặc lùi
    if (player.isCurrentlyAttacking() && std::abs(distanceX) < 100) {
        if (botOnGround) {
            if (actionChance < 50) bot.setAction(Player::ActionType::Jump);
            else bot.setAction(distanceX > 0 ? Player::ActionType::RunLeft : Player::ActionType::RunRight);
        }
        return;
    }

    // Áp sát nhanh hơn Medium
    // Áp sát nhưng chậm hơn và không phải lúc nào cũng chạy
    // Áp sát nhanh hơn Medium
    if (abs(distanceX) > 120) {
        // bot.setAction(distanceX > 0 ? Player::ActionType::RunRight : Player::ActionType::RunLeft);
        if (distanceX > 0) {
            bot.getSprite().move(150 * botSpeedFactor * deltaTime, 0); // qua phải
        } else {
            bot.getSprite().move(-150 * botSpeedFactor * deltaTime, 0); // qua trái
        }
        bot.setAction(distanceX > 0 ? Player::ActionType::RunRight : Player::ActionType::RunLeft);
    }


    
    // Trong tầm tấn công
    else if (abs(distanceX) > 40) {
        if (actionChance < 40 && bot.getAttackClock().getElapsedTime().asSeconds() > 0.9f) {
            bot.setAction(rand() % 2 == 0 ? Player::ActionType::Punch : Player::ActionType::Kick);
        } else if (actionChance < 60 && botOnGround) {
            bot.setAction(Player::ActionType::Jump);
        } else {
            bot.setAction(distanceX > 0 ? Player::ActionType::RunRight : Player::ActionType::RunLeft);
        }
    }
    // Rất gần => tấn công liên tục
    else {
        if (bot.getAttackClock().getElapsedTime().asSeconds() > 0.9f) {
            bot.setAction(rand() % 2 == 0 ? Player::ActionType::Punch : Player::ActionType::Kick);
        } else {
            bot.setAction(Player::ActionType::Idle);
        }
    }
}
