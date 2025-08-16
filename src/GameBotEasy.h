#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"

class EasyMode {
public:
    EasyMode(sf::RenderWindow& win); // bắt buộc truyền window từ ngoài vào
    void runGameWithBot_Easy();

private:
    void aiControlBot(float deltaTime);

    sf::RenderWindow& gameWindow; // tham chiếu, không tạo mới
    Player player;
    Player bot;
    sf::Clock clock;

    sf::RectangleShape background;
    sf::Texture backgroundTexture;
    sf::Font font;
    sf::Text vsText;
    sf::Text gameOverText;
    sf::Text exitText;    
    
    sf::Music bgMusic;
    
    sf::RectangleShape playerHealthBar;
    sf::RectangleShape botHealthBar;
    bool gameEnded;
};
