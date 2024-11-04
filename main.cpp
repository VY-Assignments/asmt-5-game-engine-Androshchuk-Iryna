#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

using namespace std;

class Player{
private:
    string name;
    int score;
};

class Hangman{
private:
    int parts;
public:
    void draw(sf::RenderWindow& window) {
        sf::RectangleShape verticalPart(sf::Vector2f(10, 200));
        verticalPart.setPosition(500, 150);
        verticalPart.setFillColor(sf::Color::Black);
        window.draw(verticalPart);

        sf::RectangleShape horizontalPart(sf::Vector2f(100, 10));
        horizontalPart.setPosition(400, 150);
        horizontalPart.setFillColor(sf::Color::Black);
        window.draw(horizontalPart);


        if (parts >= 1) {
            sf::CircleShape head(20);
            head.setPosition(400, 170);
            head.setFillColor(sf::Color::Black);
            window.draw(head);
        }

        if (parts >= 2) {
            sf::RectangleShape body(sf::Vector2f(10, 100));
            body.setPosition(415, 190);
            body.setFillColor(sf::Color::Black);
            window.draw(body);
        }

        if (parts >= 3) {
            sf::RectangleShape leftArm(sf::Vector2f(50, 10));
            leftArm.setOrigin(50, 5);
            leftArm.setPosition(415, 210);
            leftArm.setFillColor(sf::Color::Black);
            leftArm.setRotation(-45);
            window.draw(leftArm);
        }

        if (parts >= 4) {
            sf::RectangleShape rightArm(sf::Vector2f(50, 10));
            rightArm.setOrigin(0, 5);
            rightArm.setPosition(415, 210);
            rightArm.setFillColor(sf::Color::Black);
            rightArm.setRotation(45);
            window.draw(rightArm);
        }

        if (parts >= 5) {
            sf::RectangleShape leftLeg(sf::Vector2f(10, 50));
            leftLeg.setOrigin(5, 0);
            leftLeg.setPosition(415, 290);
            leftLeg.setFillColor(sf::Color::Black);
            leftLeg.setRotation(-30);
            window.draw(leftLeg);
        }

        if (parts >= 6) {
            sf::RectangleShape rightLeg(sf::Vector2f(10, 50));
            rightLeg.setOrigin(5, 0);
            rightLeg.setPosition(415, 290);
            rightLeg.setFillColor(sf::Color::Black);
            rightLeg.setRotation(30);
            window.draw(rightLeg);
        }
    }

    void addPart() {
        if (parts <= 6) {
            parts++;
        }
    }

    bool checkIfComplete() const {
        return parts > 6;
    }
};

class WordSelector {

};

class Game {

};


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
    Hangman hangman;

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
            if (currentScreen == Screens::GameDificaulty) {
                easyButton->handleEvent(event);
                mediumButton->handleEvent(event);
                hardButton->handleEvent(event);
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
            easyButton->draw(window);
            mediumButton->draw(window);
            hardButton->draw(window);
            window.draw(difficultText);
        } else if (currentScreen == Screens::Game) {
            hangman.draw(window);
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
            hangman.addPart();
        });
        mediumButton = new Button(300, 400, 200, 50, "Medium", font, [this]() {
            currentScreen = Screens::Game;
            hangman.addPart();
        });
        hardButton = new Button(300, 500, 200, 50, "Hard", font, [this]() {
            currentScreen = Screens::Game;
            hangman.addPart();
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