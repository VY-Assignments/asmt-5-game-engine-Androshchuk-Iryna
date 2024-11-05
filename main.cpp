#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <fstream>
#include <sstream>

using namespace std;

class Player{
private:
    string name;
    int score;
};

class Hangman{
private:
    int parts=0;
public:
    void draw(sf::RenderWindow& window) { // the coordinates of the body part was set up by chat gpt I tried,
        // but I almost cried while doing this. And it also turned to be bad :(
        sf::RectangleShape verticalPart(sf::Vector2f(10, 200));
        verticalPart.setPosition(450, 100);
        verticalPart.setFillColor(sf::Color::Black);
        window.draw(verticalPart);

        sf::RectangleShape horizontalPart(sf::Vector2f(100, 10));
        horizontalPart.setPosition(350, 100);
        horizontalPart.setFillColor(sf::Color::Black);
        window.draw(horizontalPart);

        sf::RectangleShape rope(sf::Vector2f(10, 40));
        rope.setPosition(395, 100);
        rope.setFillColor(sf::Color::Black);
        window.draw(rope);


        if (parts >= 1) {
            sf::CircleShape head(20);
            head.setPosition(380, 140);
            head.setFillColor(sf::Color::Transparent);
            head.setOutlineThickness(5);
            head.setOutlineColor(sf::Color::Black);
            window.draw(head);
        }

        if (parts >= 2) {
            sf::RectangleShape body(sf::Vector2f(5, 80));
            body.setPosition(395, 180);
            body.setFillColor(sf::Color::Black);
            window.draw(body);
        }

        if (parts >= 3) {
            sf::RectangleShape leftArm(sf::Vector2f(50, 5));
            leftArm.setOrigin(50, 2.5);
            leftArm.setPosition(395, 200);
            leftArm.setFillColor(sf::Color::Black);
            leftArm.setRotation(-45);
            window.draw(leftArm);
        }

        if (parts >= 4) {
            sf::RectangleShape rightArm(sf::Vector2f(50, 5));
            rightArm.setOrigin(0, 2.5);
            rightArm.setPosition(395, 200);
            rightArm.setFillColor(sf::Color::Black);
            rightArm.setRotation(45);
            window.draw(rightArm);
        }

        if (parts >= 5) {
            sf::RectangleShape leftLeg(sf::Vector2f(50, 5));
            leftLeg.setOrigin(50, 2.5);
            leftLeg.setPosition(395, 260);
            leftLeg.setFillColor(sf::Color::Black);
            leftLeg.setRotation(-45);
            window.draw(leftLeg);
        }

        if (parts >= 6) {
            sf::RectangleShape rightLeg(sf::Vector2f(50, 5));
            rightLeg.setOrigin(0, 2.5);
            rightLeg.setPosition(395, 260);
            rightLeg.setFillColor(sf::Color::Black);
            rightLeg.setRotation(45);
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
private:
    vector<string> easyWords;
    vector<string> mediumWords;
    vector<string> hardWords;

public:
    WordSelector(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Could not open the file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            istringstream stream(line);
            string word;
            while (stream >> word) {
                if (word.length() >= 3 && word.length() <= 4) {
                    easyWords.push_back(word);
                } else if (word.length() >= 5 && word.length() <= 6) {
                    mediumWords.push_back(word);
                } else if (word.length() >= 7) {
                    hardWords.push_back(word);
                }
            }
        }
    }

    string getRandomWord(int difficulty) {
        vector<string>* wordList = nullptr;

        if (difficulty == 'Easy') {
            wordList = &easyWords;
        } else if (difficulty == 'Medium') {
            wordList = &mediumWords;
        } else if (difficulty == 'Hard') {
            wordList = &hardWords;
        }

        if (wordList && !wordList->empty()) {
            size_t index = rand() % wordList->size();
            return (*wordList)[index];
        }
    }
};

class Game {
private:
    string currentWord;
public:
    void initializeGame(const string& filename, int difficulty) {
        WordSelector selector(filename);
        currentWord = selector.getRandomWord(difficulty);
    }
    string getCurrentWord() const {
        return currentWord;
    }
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
    function<void()> onClick;

public:
    Button(float x, float y, float width, float height, const string& buttonText, sf::Font& font, function<void()> onClick): onClick(onClick) {
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
    string userInput;
    sf::Text difficultText;
    sf::Text wordDisplay;
    Hangman hangman;
    class Game* game = nullptr;

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

            if (game) {
                string displayedWord = game->getCurrentWord();
                for (char& ch : displayedWord) {
                    if (ch != ' ') {
//                        ch = '_';
                    }
                }
                wordDisplay.setString(displayedWord);
                window.draw(wordDisplay);
            }
        }

        window.display();
    }

public:
    StartGameWindow(int width, int height, const string& title) {
        window.create(sf::VideoMode(width, height), title);

        if (!font.loadFromFile("/Users/mac/CLionProjects/asmt-5-game-engine-Androshchuk-Iryna/Kharkiv Tone 04.10.2020.ttf")) {
            cerr << "Error loading font" << endl;
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

        wordDisplay.setFont(font);
        wordDisplay.setFillColor(sf::Color::Black);
        wordDisplay.setPosition(300, 400);

        startButton = new Button(300, 300, 200, 50, "Start Game", font, [this]() {
            currentScreen = Screens::Name;
        });

        easyButton = new Button(300, 250, 200, 50, "Easy", font, [this]() {
            startGame('Easy');
        });
        mediumButton = new Button(300, 320, 200, 50, "Medium", font, [this]() {
            startGame('Medium');
        });
        hardButton = new Button(300, 390, 200, 50, "Hard", font, [this]() {
            startGame('Hard');
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
    void startGame(int difficulty) {
        currentScreen = Screens::Game;
        game = new class Game();
        game->initializeGame("/Users/mac/CLionProjects/untitled13/words.txt", difficulty);
    }
};



int main() {
    StartGameWindow gameWindow(800, 600, "Hangman Game");
    gameWindow.run();

    return 0;
}