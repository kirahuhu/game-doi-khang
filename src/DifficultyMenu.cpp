#include "DifficultyMenu.h"
#include <iostream>

DifficultyMenu::DifficultyMenu(float width, float height) {
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Không thể tải font cho Difficulty Menu!\n";
    }

    if (!backgroundTexture.loadFromFile("assets/images/difficultymenu_background.jpg")) {
    std::cerr << "Không thể tải ảnh nền màn chọn độ khó!\n";
}
backgroundSprite.setTexture(backgroundTexture);

// Scale vừa cửa sổ (giả sử là 800x600)
sf::Vector2u bgSize = backgroundTexture.getSize();
backgroundSprite.setScale(800.f / bgSize.x, 600.f / bgSize.y);

    std::vector<std::string> labels = {
        "EASY",
        "MEDIUM",
        "HARD"
    };

    for (int i = 0; i < labels.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(36);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setPosition(width / 2.f - 100, height / 2.f + i * 60 - 50);
        options.push_back(text);
    }

    selectedIndex = 0;
}

void DifficultyMenu::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    for (auto& text : options)
        window.draw(text);
}

void DifficultyMenu::moveUp() {
    if (selectedIndex > 0) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        options[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void DifficultyMenu::moveDown() {
    if (selectedIndex < options.size() - 1) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        options[selectedIndex].setFillColor(sf::Color::Red);
    }
}

int DifficultyMenu::getSelectedIndex() const {
    return selectedIndex;
}

BotDifficulty DifficultyMenu::getDifficulty() const {
    switch (selectedIndex) {
        case 0: return BotDifficulty::Easy;
        case 1: return BotDifficulty::Medium;
        case 2: return BotDifficulty::Hard;
        default: return BotDifficulty::Easy;
    }
}

void DifficultyMenu::handleSelection(sf::RenderWindow& window) {
    window.close();
    switch (selectedIndex) {
        case 1: // MEDIUM
            {
                MediumMode game(window);
                game.runGameWithBot_Medium(); // Khởi chạy chế độ medium
            }
            break;
        case 0: // EASY
            std::cout << "Chế độ Easy chưa được triển khai!\n";
            break;
        case 2: // HARD
            std::cout << "Chế độ Hard chưa được triển khai!\n";
            break;
    }
}
