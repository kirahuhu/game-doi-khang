#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>

void runGamePVP() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "2 Player Fighting Game");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Không thể tải font!\n";
    }   

    sf::Clock attackClock1;
    sf::Clock attackClock2;

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(200.f, 200.f);
    bool gameEnded = false;

    // Tải background
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("assets/images/background.jpg")) {
    std::cerr << "Không thể load background.jpg\n";
    }
    sf::Sprite bgSprite(bgTexture);

    // Scale ảnh nền cho vừa cửa sổ 800x600
    sf::Vector2u bgSize = bgTexture.getSize();
    bgSprite.setScale(
    800.f / bgSize.x,
    600.f / bgSize.y
    );

    // Tạo 2 người chơi
    Player player1("assets/images/player_1.png", sf::Vector2f(200.f, 400.f));
    Player player2("assets/images/player_2.png", sf::Vector2f(500.f, 400.f));

    // Đồng hồ thời gian
    sf::Clock clock;

   while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    if (!gameEnded) {
        player1.handleInput(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W);
        player2.handleInput(sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up);

        player1.update(deltaTime);
        player2.update(deltaTime);

        // Kiểm tra va chạm đơn giản
        bool isColliding = player1.getBounds().intersects(player2.getBounds());

        if (isColliding && sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
    if (attackClock1.getElapsedTime().asSeconds() > 0.5f) {
        player1.setAction(Player::ActionType::Kick);  // Đổi hình đá
        player2.takeDamage(20); // Đá gây 20 damage
        player2.setHit();       // Player 2 bị đánh
        attackClock1.restart();
    }
}


if (isColliding && sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
    if (attackClock2.getElapsedTime().asSeconds() > 0.5f) {
        player1.takeDamage(10);
        player1.setHit(); // Gọi hiệu ứng bị đánh
        attackClock2.restart();
    }
}


        // Kiểm tra kết thúc game
        if (player1.isDead()) {
            gameOverText.setString("Player 2 Win");
            gameEnded = true;
        } else if (player2.isDead()) {
            gameOverText.setString("Player 1 WIN");
            gameEnded = true;
        }
    }

    // VẼ MÀN HÌNH
    window.clear();
    window.draw(bgSprite);
    player1.render(window);
    player2.render(window);
    if (gameEnded)
        window.draw(gameOverText);
    window.display();
}
}
