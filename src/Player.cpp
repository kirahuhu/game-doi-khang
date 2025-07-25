#include "Player.h"
#include <iostream>

Player::Player(const std::string& textureFile, sf::Vector2f startPos) {
    isOnGround = false;

    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Lỗi: Không thể load " << textureFile << std::endl;
    }

    sprite.setTexture(texture);

    sprite.setScale(0.2f, 0.2f); // thu nhỏ xuống 20%, tuỳ chỉnh nếu cần

    sprite.setPosition(startPos);
    speed = 200.f;
    hp = 100;
    velocity = {0.f, 0.f};

    hpBarBack.setSize({50.f, 5.f});
    hpBarBack.setFillColor(sf::Color::Black);

    hpBarFront.setSize({50.f, 5.f});
    hpBarFront.setFillColor(sf::Color::Green);
}

void Player::handleInput(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key jump) {
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(left))
        velocity.x = -speed;
    else if (sf::Keyboard::isKeyPressed(right))
        velocity.x = speed;

    if (sf::Keyboard::isKeyPressed(jump) && isOnGround) {
        velocity.y = -300.f;
        isOnGround = false; // tránh nhảy nhiều lần liên tiếp
    }
}

void Player::update(float deltaTime) {
    velocity.y += 500.f * deltaTime;
    sprite.move(velocity * deltaTime);

    float groundY = 600.f;
    float spriteHeight = sprite.getGlobalBounds().height;

    // Kiểm tra chạm đất
    if (sprite.getPosition().y + spriteHeight > groundY && velocity.y > 0) {
        sf::Vector2f pos = sprite.getPosition();
        pos.y = groundY - spriteHeight;
        sprite.setPosition(pos);
        velocity.y = 0.f;
        isOnGround = true;
    } else {
        isOnGround = false;
    }
    if (isHit && hitClock.getElapsedTime().asSeconds() > 0.2f) {
    isHit = false;
    sprite.setColor(sf::Color::White); // Trả lại màu ban đầu
    }

    if (isAttacking && actionClock.getElapsedTime().asSeconds() > 0.2f) {
        setAction(ActionType::Idle);
    }
}





void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);

    // Cập nhật vị trí và kích thước thanh máu theo sprite
    sf::Vector2f pos = sprite.getPosition();
    hpBarBack.setPosition(pos.x, pos.y - 10);
    hpBarFront.setPosition(pos.x, pos.y - 10);

    hpBarFront.setSize({50.f * (hp / 100.f), 5.f});

    window.draw(hpBarBack);
    window.draw(hpBarFront);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}

bool Player::isDead() const {
    return hp <= 0;
}
void Player::setHit() {
    isHit = true;
    sprite.setColor(sf::Color::Red);  // Đổi màu thành đỏ
    hitClock.restart();
}

void Player::setAction(ActionType type) {
    switch (type) {
        case ActionType::Idle:
            texture.loadFromFile("assets/images/player_1.png");
            isAttacking = false;
            break;
        case ActionType::Kick:
            texture.loadFromFile("assets/images/player_1_kick.png");
            isAttacking = true;
            actionClock.restart();
            break;
    }
    sprite.setTexture(texture);
}

