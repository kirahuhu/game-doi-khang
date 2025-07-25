#ifndef DIFFICULTY_MENU_H
#define DIFFICULTY_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>

enum class BotDifficulty {
    Easy,
    Medium,
    Hard
};

class DifficultyMenu {
public:
    DifficultyMenu(float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
    BotDifficulty getDifficulty() const;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;


private:
    std::vector<sf::Text> options;
    sf::Font font;
    int selectedIndex;
};

#endif

