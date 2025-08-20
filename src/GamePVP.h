#pragma once
#include "BaseGameMode.h"

class PVPMode : public BaseGameMode {
public:
    PVPMode(sf::RenderWindow& win);

    void aiControlBot(float dt) override; // PVP không cần AI
    void runGame() override;               // Override để xử lý input 2 player
    void render() override;
    void update(float dt) override;
};
