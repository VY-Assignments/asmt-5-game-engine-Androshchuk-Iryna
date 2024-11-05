#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <fstream>
#include <sstream>
#include <set>

using namespace std;

class Player {
private:
    string name;
    int score = 0;

public:

    void setName(const string& playerName) {
        name = playerName;
    }

    string getName() const {
        return name;
    }

    int getScore() const {
        return score;
    }

    void incrementScore() {
        score += 10;
    }

    void decreaseScore(){
        score -= 5;
    }
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
        if (parts < 6) {
            parts++;
        }
    }

    bool isGameOver() const {
        return parts >= 6;
    }
    void removePart() {
        if (parts > 0) {
            parts--;
        }
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
    set<char> guessedLetters;

public:
    string currentWord;
    void initializeGame(const string& filename, int difficulty) {
        WordSelector selector(filename);
        currentWord = selector.getRandomWord(difficulty);
        guessedLetters.clear();
    }

    bool guessLetter(char letter) {
        guessedLetters.insert(letter);
        return currentWord.find(letter) != string::npos;
    }

    string getGuessedLetters() const {
        string letters;
        for (char letter : guessedLetters) {
            letters += letter;
            letters += ' ';
        }
        return letters;
    }

    string getDisplayedWord() const {
        string displayedWord;
        for (char c : currentWord) {
            if (guessedLetters.count(c) > 0 || c == ' ') {
                displayedWord += c;
            } else {
                displayedWord += '_';
            }
        }
        return displayedWord;
    }

    bool isGameWon() const {
        for (char c : currentWord) {
            if (c != ' ' && guessedLetters.count(c) == 0) {
                return false;
            }
        }
        return true;
    }
    void revealLetter() {
        for (char c : currentWord) {
            if (c != ' ' && guessedLetters.count(c) == 0) {
                guessedLetters.insert(c);
                break;
            }
        }
    }
};

enum Screens {
    Start,
    Name,
    GameDificaulty,
    Game,
    Restart
};

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    function<void()> onClick;

public:
    Button(float x, float y, float width, float height, const string& buttonText, sf::Font& font, function<void()> onClick) : onClick(onClick) {
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
    Button* restartButton;
    string userInput;
    sf::Text difficultText;
    sf::Text wordDisplay;
    sf::Text guessedLettersDisplay;
    sf::Text winingText;
    sf::Text losingText;
    sf::Text playerNameDisplay;
    Hangman hangman;
    Player player;

    Screens currentScreen = Screens::Start;
    class Game* game = nullptr;
    bool gameWon = false;

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
                    player.setName(userInput);
                    difficultText.setString("Hi, " + player.getName() + "! Choose difficulty:\n");
                }
            }
            if (currentScreen == Screens::GameDificaulty) {
                easyButton->handleEvent(event);
                mediumButton->handleEvent(event);
                hardButton->handleEvent(event);
            }
            if (currentScreen == Screens::Game) {
                if (event.type == sf::Event::TextEntered && event.text.unicode < 128) {
                    char guessedLetter = static_cast<char>(event.text.unicode);
                    if (isalpha(guessedLetter)) {
                        if (!game->guessLetter(guessedLetter)) {
                            hangman.addPart();
                        }
                        if (hangman.isGameOver()) {
                            currentScreen = Screens::Restart;
                            losingText.setString("You lost! The word was: " + game->currentWord);
                            player.decreaseScore();
                            gameWon = false;
                        } else if (game->isGameWon()) {
                            currentScreen = Screens::Restart;
                            player.incrementScore();
                            winingText.setString("You won!");
                            gameWon = true;
                        }
                    }
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) {
                    hangman.removePart();
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) {
                    game->revealLetter(); // Відкрити одну літеру
                }
            }
            if (currentScreen == Screens::Restart) {
                restartButton->handleEvent(event);
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
            wordDisplay.setString(game->getDisplayedWord());
            window.draw(wordDisplay);

            guessedLettersDisplay.setString("Guessed Letters: " + game->getGuessedLetters());
            window.draw(guessedLettersDisplay);

            playerNameDisplay.setString("Player: " + player.getName() + " | Score: " + to_string(player.getScore()));
            window.draw(playerNameDisplay);
        } else if (currentScreen == Screens::Restart) {
            if (gameWon) {
                window.draw(winingText);
            } else {
                window.draw(losingText);
            }
            restartButton->draw(window);
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
        startText.setString("Hangman Game");
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

        winingText.setFont(font);
        winingText.setFillColor(sf::Color::Black);
        winingText.setPosition(300, 200);

        losingText.setFont(font);
        losingText.setFillColor(sf::Color::Black);
        losingText.setPosition(150, 200);

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

        restartButton = new Button(250, 400, 250, 50, "Restart Game", font, [this]() {
            currentScreen = Screens::Start;
        });

        difficultText.setFont(font);
        difficultText.setFillColor(sf::Color::Black);
        difficultText.setPosition(200, 100);

        guessedLettersDisplay.setFont(font);
        guessedLettersDisplay.setFillColor(sf::Color::Black);
        guessedLettersDisplay.setPosition(50, 50);
        guessedLettersDisplay.setCharacterSize(20);


        playerNameDisplay.setFont(font);
        playerNameDisplay.setFillColor(sf::Color::Black);
        playerNameDisplay.setPosition(50, 10);
        playerNameDisplay.setCharacterSize(20);
        playerNameDisplay.setString("Player: " + player.getName() + " Score: " + to_string(player.getScore()));

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
        hangman = Hangman();
    }
};

int main() {
    StartGameWindow gameWindow(800, 600, "Hangman Game");
    gameWindow.run();
    return 0;
}