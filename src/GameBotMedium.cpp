#include "Player.h"
#include "GameBotMedium.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>

MediumMode::MediumMode(sf::RenderWindow& window) : window(window), player("assets/images/player_1.png", "player1", sf::Vector2f(100, 400)), bot("assets/images/mediumbot2.png", "mediumbot2", sf::Vector2f(500, 320)) {
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
    if (!font.loadFromFile("assets/fonts/newfont.ttf")) {
        font.loadFromFile("assets/fonts/arial.ttf"); // Font dự phòng
    }
    vsText.setFont(font);
    vsText.setString("VS");
    vsText.setCharacterSize(40);
    vsText.setFillColor(sf::Color::White);
    vsText.setPosition(375, 30);
}

void MediumMode::runGameWithBot_Medium() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            player.handleInput(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, sf::Keyboard::J, sf::Keyboard::K, true);
        }
        player.update(1.0f / 60.0f);  // Cập nhật trạng thái và chuyển động của player


        // AI cho bot ở chế độ medium
        aiControlBot();

        // Va chạm và sát thương
        if (player.getBounds().intersects(bot.getBounds())) {
            player.takeDamage(10);
            bot.takeDamage(10);
            player.setHit();
            bot.setHit();
        }

        // Cập nhật thanh máu (giả định hp giảm tuyến tính, không truy cập hp trực tiếp)
        window.clear();
        window.draw(background);
        window.draw(playerHealthBar);
        window.draw(botHealthBar);
        window.draw(vsText);
        player.render(window);
        bot.render(window);
        window.display();
    }
}

void MediumMode::aiControlBot() {
    sf::FloatRect playerBounds = player.getBounds();
    sf::FloatRect botBounds = bot.getBounds();
    float distance = botBounds.left - playerBounds.left; // Dùng left để ước lượng vị trí x

    if (distance > 50) {
        bot.setAction(Player::ActionType::RunLeft); // Dòng 76
    } else if (distance < -50) {
        bot.setAction(Player::ActionType::RunRight); // Dòng 79
    } else {
        bot.setAction(Player::ActionType::Idle); // Dòng 81
        if (rand() % 100 < 5) { // 5% cơ hội tấn công
            if (rand() % 2 == 0) {
                bot.setAction(Player::ActionType::Punch); // Dòng 84
            } else {
                bot.setAction(Player::ActionType::Kick); // Dòng 86
            }
        }
    }
    bot.update(1.0f / 60.0f); // Cập nhật bot với deltaTime cố định
}