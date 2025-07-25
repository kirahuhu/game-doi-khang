#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "DifficultyMenu.h"
#include "GamePVP.h"
// #include "GameBot.h" // bạn sẽ làm sau

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
                        // Hiện menu chọn độ khó
                        DifficultyMenu diffMenu(800, 600);
                        bool choosing = true;
                        while (choosing && window.isOpen()) {
                            while (window.pollEvent(event)) {
                                if (event.type == sf::Event::Closed)
                                    window.close();
                                else if (event.type == sf::Event::KeyPressed) {
                                    if (event.key.code == sf::Keyboard::Up)
                                        diffMenu.moveUp();
                                    else if (event.key.code == sf::Keyboard::Down)
                                        diffMenu.moveDown();
                                    else if (event.key.code == sf::Keyboard::Enter) {
                                        // Bạn sẽ viết hàm này sau
                                        // runGameWithBot(diffMenu.getDifficulty());
                                        choosing = false;
                                    }
                                }
                            }

                            window.clear();
                            diffMenu.draw(window);
                            window.display();
                        }
                    } else if (choice == 1) {
                        window.close();
                        runGamePVP(); // chơi 2 người
                    } else if (choice == 2) {
                        window.close(); // thoát
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
