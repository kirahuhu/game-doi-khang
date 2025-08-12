#ifndef MEDIUMMODE_H
#define MEDIUMMODE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"


class MediumMode {
public:
    MediumMode();
    MediumMode(sf::RenderWindow& window);
    void runGameWithBot_Medium();
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
    sf::Music bgMusic;
    sf::Text exitText;
    sf::Text gameOverText; // Thêm để hiển thị kết quả
    bool gameEnded; // Thêm để theo dõi trạng thái game
    sf::Clock clock; // Thêm để tính deltaTime động
    void aiControlBot(float deltaTime);
};

#endif