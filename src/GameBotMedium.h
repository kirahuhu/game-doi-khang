#ifndef MEDIUMMODE_H
#define MEDIUMMODE_H

#include <SFML/Graphics.hpp>
#include "Player.h"

class MediumMode {
public:
    MediumMode();
    MediumMode(sf::RenderWindow& window);
    void runGameWithBot_Medium();

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

    void aiControlBot();
};

#endif