#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

class Player {


private:
    sf::Sprite sprite;
    sf::Texture texture;
    std::string imagePrefix;

    sf::Vector2f velocity;
    bool isHit = false;
    sf::Clock hitClock;

    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarFront;

    bool isAttacking = false;
    sf::Clock actionClock;

    float speed;
    int hp;
    bool isOnGround;

    public:
    enum class ActionType { Idle, Kick, Punch, Jump, RunLeft, RunRight, Hitten };

    Player(const std::string& textureFile, const std::string& prefix, sf::Vector2f startPos);

    void handleInput(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key jump,
                     sf::Keyboard::Key punch, sf::Keyboard::Key kick, bool isPlayer1);

    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    void takeDamage(int amount);
    bool isDead() const;
    sf::FloatRect getBounds() const;
    void setHit();
    void setAction(ActionType type);


};

#endif
