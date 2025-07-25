#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


class Player {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    bool isHit = false;
    sf::Clock hitClock;

    sf::RectangleShape hpBarBack; // viền đen
    sf::RectangleShape hpBarFront; // máu còn lại

    bool isAttacking = false;
    sf::Clock actionClock;

    float speed;
    int hp;
    bool isOnGround;

public:
    Player(const std::string& textureFile, sf::Vector2f startPos);

    void handleInput(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key jump);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    void takeDamage(int amount);
    bool isDead() const;
    sf::FloatRect getBounds() const;
    void setHit();  // khai báo hàm
    enum class ActionType { Idle, Kick };

    void setAction(ActionType type);

};

#endif
