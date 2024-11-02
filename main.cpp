#include <iostream>
#include <SFML/Graphics.hpp>


class GameWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text text;

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    void render() {
        window.clear(sf::Color::White);
        window.draw(text);
        window.display();
    }
public:
    GameWindow(int width, int height, const std::string& title) {
        window.create(sf::VideoMode(width, height), title);

        if (!font.loadFromFile("/Users/mac/CLionProjects/asmt-5-game-engine-Androshchuk-Iryna/Kharkiv Tone 04.10.2020.ttf")) { // Змініть шлях до шрифту
            std::cerr << "Error" << std::endl;
        }

        text.setFont(font);
        text.setString("Hangmen game");
        text.setFillColor(sf::Color::Black);
        text.setPosition(250, 100);
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            render();
        }
    }
};

int main() {
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;

    GameWindow gameWindow(800, 600, "Hangmen Game");
    gameWindow.run();

    return 0;
}
