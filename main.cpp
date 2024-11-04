#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

enum Screens {
    Start,
    Name,
    GameDificaulty,
    Game
};

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;

public:
    Button(float x, float y, float width, float height, const std::string& buttonText, sf::Font& font, std::function<void()> onClick): onClick(onClick) {
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color::Black);

        text.setFont(font);
        text.setString(buttonText);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + 20, y + 10);
        text.setCharacterSize(24);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (shape.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                onClick();
            }
        }
    }
};

class StartGameWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text startText;
    sf::Text gameText;
    sf::Text userInputText;
    Button* startButton;
    Button* easyButton;
    Button* mediumButton;
    Button* hardButton;
    Screens currentScreen = Screens::Start;
    std::string userInput;
    sf::Text difficultText;

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (currentScreen == Screens::Start) {
                startButton->handleEvent(event);
            } else if (currentScreen == Screens::Name) {
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b') {
                        if (!userInput.empty()) {
                            userInput.pop_back();
                        }
                    } else if (event.text.unicode < 128) {
                        userInput += static_cast<char>(event.text.unicode);
                    }
                    userInputText.setString(userInput);
                } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    currentScreen = Screens::GameDificaulty;
                    difficultText.setString("Hi, " + userInput + "! Choose difficulty:\n");

                }
            }
        }
    }

    void render() {
        window.clear(sf::Color::White);

        if (currentScreen == Screens::Start) {
            window.draw(startText);
            startButton->draw(window);
        } else if (currentScreen == Screens::Name) {
            window.draw(gameText);
            window.draw(userInputText);
        } else if (currentScreen == Screens::GameDificaulty) {
            window.draw(difficultText);
            easyButton->draw(window);
            mediumButton->draw(window);
            hardButton->draw(window);
        }

        window.display();
    }

public:
    StartGameWindow(int width, int height, const std::string& title) {
        window.create(sf::VideoMode(width, height), title);

        if (!font.loadFromFile("/Users/mac/CLionProjects/asmt-5-game-engine-Androshchuk-Iryna/Kharkiv Tone 04.10.2020.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }

        startText.setFont(font);
        startText.setString("Hangman Name");
        startText.setFillColor(sf::Color::Black);
        startText.setPosition(250, 100);

        gameText.setFont(font);
        gameText.setString("Input your name:");
        gameText.setFillColor(sf::Color::Black);
        gameText.setPosition(250, 100);

        userInputText.setFont(font);
        userInputText.setFillColor(sf::Color::Black);
        userInputText.setPosition(250, 200);

        startButton = new Button(300, 300, 200, 50, "Start Game", font, [this]() {
            currentScreen = Screens::Name;
        });

        easyButton = new Button(300, 300, 200, 50, "Easy", font, [this]() {
            currentScreen = Screens::Game;
        });
        mediumButton = new Button(300, 400, 200, 50, "Medium", font, [this]() {
            currentScreen = Screens::GameDificaulty;
        });
        hardButton = new Button(300, 500, 200, 50, "Hard", font, [this]() {
            currentScreen = Screens::GameDificaulty;
        });

        difficultText.setFont(font);
        difficultText.setString("Hi, " + userInput + "! Choose difficulty:\n");
        difficultText.setFillColor(sf::Color::Black);
        difficultText.setPosition(200, 100);
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            render();
        }
    }
};



int main() {
    StartGameWindow gameWindow(800, 600, "Hangman Name");
    gameWindow.run();

    return 0;
}