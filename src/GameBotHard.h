#ifndef HARDMODE_H
#define HARDMODE_H

#include <SFML/Graphics.hpp>
#include "Player.h"

class HardMode {
public:
    HardMode(sf::RenderWindow& window);
    void runGameWithBot_Hard();
    sf::Vector2f getOverlap(const sf::FloatRect& rect1, const sf::FloatRect& rect2);

private:
    sf::RenderWindow& window;
    Player player;
    Player bot;
    sf::RectangleShape background;
    sf::Texture backgroundTexture;
    sf::RectangleShape playerHealthBar;
    sf::RectangleShape botHealthBar;
    sf::Font font;
    sf::Text vsText;
    sf::Text gameOverText;
    bool gameEnded;
    sf::Clock clock;
    sf::Text exitText;


    void aiControlBot(float deltaTime);
};

#endif
