#include "Player.h"
#include <iostream>

Player::Player(const std::string& textureFile, const std::string& prefix, sf::Vector2f startPos)
    : imagePrefix(prefix) {
    isOnGround = false;

    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Lỗi: Không thể load " << textureFile << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(startPos);
    speed = 200.f;
    hp = 200;
    velocity = {0.f, 0.f};

    hpBarBack.setSize({50.f, 5.f});
    hpBarBack.setFillColor(sf::Color::Black);

    hpBarFront.setSize({50.f, 5.f});
    hpBarFront.setFillColor(sf::Color::Green);
}

void Player::handleInput(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key jump,
                         sf::Keyboard::Key punch, sf::Keyboard::Key kick, bool isPlayer1) {
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(left)) {
    velocity.x = -speed;
    setAction(ActionType::RunLeft);
} else if (sf::Keyboard::isKeyPressed(right)) {
    velocity.x = speed;
    setAction(ActionType::RunRight);
} else {
    setAction(ActionType::Idle);  // khi không ấn gì
}
    if (sf::Keyboard::isKeyPressed(jump) && isOnGround) {
        velocity.y = -300.f;
        isOnGround = false;
        setAction(ActionType::Jump);
    }

    if (sf::Keyboard::isKeyPressed(punch)) {
        setAction(ActionType::Punch);
    } else if (sf::Keyboard::isKeyPressed(kick)) {
        setAction(ActionType::Kick);
    }
}

void Player::update(float deltaTime) {
    velocity.y += 500.f * deltaTime;
    sprite.move(velocity * deltaTime);

    float groundY = 600.f;
    float spriteHeight = sprite.getGlobalBounds().height;

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
        sprite.setColor(sf::Color::White);
    }

    if (isAttacking && actionClock.getElapsedTime().asSeconds() > 0.2f) {
        setAction(ActionType::Idle);
    }
}


void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);

    sf::Vector2f pos = sprite.getPosition();
    hpBarBack.setPosition(pos.x, pos.y - 10);
    hpBarFront.setPosition(pos.x, pos.y - 10);

    hpBarFront.setSize({50.f * (hp / 200.f), 5.f});
    window.draw(hpBarBack);
    window.draw(hpBarFront);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
    setAction(ActionType::Hitten);
}

bool Player::isDead() const {
    return hp <= 0;
}

void Player::setHit() {
    isHit = true;
    sprite.setColor(sf::Color::Red);
    hitClock.restart();
}

void Player::setAction(ActionType type) {
    std::string path = "assets/images/" + imagePrefix;

    switch (type) {
        case ActionType::Idle:
            texture.loadFromFile(path + ".png");
            //velocity.x = 0.f;
            isAttacking = false;
            break;
        case ActionType::Kick:
            texture.loadFromFile(path + "_kick.png");
            isAttacking = true;
            actionClock.restart();
            break;
        case ActionType::Punch:
            texture.loadFromFile(path + "_punch.png");
            isAttacking = true;
            actionClock.restart();
            break;
        case ActionType::Jump:
            texture.loadFromFile(path + "_jump.png");
            break;
        case ActionType::RunLeft:
            texture.loadFromFile(path + "_runleft.png");
            //velocity.x = -speed;
            break;
        case ActionType::RunRight:
            texture.loadFromFile(path + "_runright.png");
            //velocity.x = speed;
            break;
        case ActionType::Hitten:
            texture.loadFromFile(path + "_hitten.png");
            isAttacking = true; // Coi như cũng đang "animation" tạm thời
            actionClock.restart();
            break;
    }

    sprite.setTexture(texture);
}

