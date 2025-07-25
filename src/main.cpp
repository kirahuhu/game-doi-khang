#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "GamePVP.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "US FIGHTERS");
    Menu menu(800, 600);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) menu.moveUp();
                else if (event.key.code == sf::Keyboard::Down) menu.moveDown();
                else if (event.key.code == sf::Keyboard::Enter) {
                    int choice = menu.getSelectedIndex();
                    if (choice == 0) {
                        // PLAY với bot: đang phát triển
                    } else if (choice == 1) {
                        window.close();
                        runGamePVP();
                    } else if (choice == 2) {
                        window.close();
                    }
                }
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    return 0;
}
