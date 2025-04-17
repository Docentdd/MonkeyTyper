#include <fstream>
#include <iostream>
#include <cassert>
#include "DimaTyper.h"
#include <random>

DimaTyper::DimaTyper() : window(sf::VideoMode(1200, 800), "dimaTyper", sf::Style::Titlebar | sf::Style::Close),
                         textSpeed(20), stopGame(false), isPaused(false) {
    if (!arialFont.loadFromFile("../../2324S_18_s30474_TW - assets/Arial.ttf")) {
        std::cout << "Problem loading font Arial\n";
        return ;
    }
    if (!backgroundTexture.loadFromFile("../../2324S_18_s30474_TW - assets/backgroundForGame.jpg")) {
        std::cout << "Problem loading background\n";
        return ;
    }
}

bool DimaTyper::run() {
    sf::Clock clock; // keep track of the creating text periodically
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) { // keyboard or mouse input
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                isPaused = !isPaused; // Toggle pause state
                clock.restart();
                clock.getElapsedTime().asSeconds() + 2;// when pause time doesn't go
            }
            if (event.type == sf::Event::TextEntered) {
                keyboardInput(event); // Process text input event
            }
            if (stopGame && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                handleMouseClick2(mousePos);
            }
            if (event.key.code == sf::Keyboard::Up) {
                increaseTextSize(); // Increase text size
            }
            if (event.key.code == sf::Keyboard::Down) {
                decreaseTextSize(); // Increase text size
            }
        }
            outOfBorder();

            if (stopGame) {
                gameOver();
            } else if (!isPaused) {
                if (clock.getElapsedTime().asSeconds() >= 2) { //create text every two seconds
                    createText(); // Create new text at random position
                    clock.restart(); // Restart the clock
                }
                moveDrawText();
                window.clear();
                window.draw(backgroundSprite);

                for (auto &text: textObjects) {
                    highlightText(text);
                }

                lowerBar(); // Draw the lower bar with score and input
                backgroundSprite.setTexture(backgroundTexture);
                window.display(); // already rendered window
            } else {//that else when the game is Paused
                // When paused, just draw the current screen and show the pause effect
                window.clear();
                window.draw(backgroundSprite);

                for (const auto &text: textObjects) {
                    window.draw(text);
                }

                lowerBar(); // Draw the lower bar with score and input

                // Draw the semi-transparent pause overlay
                pause.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
                pause.setFillColor(sf::Color(0, 0, 0, 150));
                window.draw(pause);

                window.display();
            }
        }
        return true; // ens that loop
    }


void DimaTyper::getWordsFromFile(const std::string &fileName) {
    std::ifstream read_file(fileName); // that object used ti read file from the file
    if (!read_file.is_open()) { //check
        throw std::runtime_error("Error opening file: " + fileName);
    }
    std::string line;
    while (read_file >> line)
        wordsFileVec.push_back(line); // each word add to the vector
    read_file.close(); // good to use
} // that's works correctly

std::string DimaTyper::randomWord() {
    getWordsFromFile("../../2324S_18_s30474_TW - assets/data.txt");
    if (wordsFileVec.empty()) {
        return "empty";
    }
    int indexRandom = std::rand() % wordsFileVec.size();
    return wordsFileVec[indexRandom];
} //that's works correctly

void DimaTyper::createText() {
    sf::Text text;
    text.setString(randomWord());
    text.setFont(arialFont);
    text.setCharacterSize(textSize);
    text.setFillColor(sf::Color::White);

    float textHeight = text.getLocalBounds().height;
    int minY = 100 + textHeight; // Full height to avoid intersection at the top and the lowerbar
    int maxY = window.getSize().y - textHeight; // Full height to avoid intersection at the bottom

    auto getRandomNumber = [](int min, int max) -> int { // lambda for generating random numbers
        std::random_device rd;  // Seed the random number generator
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine
        std::uniform_int_distribution<> dis(min, max); // produce special numbers in the specified range
        return dis(gen);
    };

    float posX = -text.getLocalBounds().width; // left side of the window
    float posY = getRandomNumber(minY, maxY);
    text.setPosition(posX, posY);

    textObjects.push_back(text); // created object text are added to the textObjects
}


void DimaTyper::moveDrawText() { // that is more depend on the frame rate of the machine
    if(score > 2){
        textSpeed = 50;
    }else{
        textSpeed = 20;
    }
    for (auto &text: textObjects) {
        text.move(textSpeed * 0.01f, 0); // Move text to the right; moves only horizontally
    } // multiplication are for making all movements smoother
}


void DimaTyper::updateFont(const sf::Font &newFont) {
    arialFont = newFont;
    for (auto &text: textObjects) {
        text.setFont(arialFont);
    }
}

void DimaTyper::updateSize(const int &newSize) {
    textSize = newSize;
    for (auto &text: textObjects) {
        text.setCharacterSize(textSize);
    }
}

int DimaTyper::getTextSize() const {
    return textSize;
}
//std::remove_if algorithm to move elements that match a certain condition to the end of the vector.
void DimaTyper::keyboardInput(const sf::Event &event) { // takes reference
    if (event.type == sf::Event::TextEntered) { // check if the characters was entered
        if (event.text.unicode < 128) { // ASCII characters (Unicode less than 128)
            char letter = static_cast<char>(event.text.unicode);
            if (letter == '\b') { // Backspace
                if (!inputT.empty()) {
                    inputT.pop_back();
                }
            } else if (letter == '\r' || letter == '\n') { // Enter key
                textObjects.erase(std::remove_if(textObjects.begin(), textObjects.end(),
                                                 [&](const sf::Text &text) {
                                                     if (text.getString().toAnsiString() == inputT) { // comparing the String to a standard ANSI string for comparison
                                                         score++;
                                                     }
                                                     //return true if matched, and indicating that must be deleted
                                                     return text.getString().toAnsiString() == inputT;
                                                 }), textObjects.end()); // erase the  elements that has been moved to the end of the vector
                                                 // that elements have been marked by the remove_if
                inputT.clear();
            } else if (std::isalpha(letter)) { // append to the 'inputT'
                inputT += letter;
            }
        }
    }
}

void DimaTyper::lowerBar() {
    scoreT.setString("Score " + std::to_string(score));
    scoreT.setFont(arialFont);
    scoreT.setCharacterSize(30);
    scoreT.setFillColor(sf::Color::White);
    scoreT.setPosition(window.getSize().x - 250, window.getSize().y - 780);
    window.draw(scoreT);

    rectangleT.setString("Input " + inputT);
    rectangleT.setFont(arialFont);
    rectangleT.setCharacterSize(30);
    rectangleT.setFillColor(sf::Color::White);
    rectangleT.setPosition(window.getSize().x - 550, window.getSize().y - 780);
    window.draw(rectangleT);
}

void DimaTyper::outOfBorder() {
    count = 0;
    sf::Vector2u windowSize = window.getSize();
    for (auto &text: textObjects) {
        sf::FloatRect textBounds = text.getGlobalBounds();
        bool check = textBounds.left > windowSize.x;
        if (check) {
            count++;
        }
    }
    if (count >= 3) {
        stopGame = true;
    }
}

void DimaTyper::gameOver() {
    window.clear();
    window.draw(backgroundSprite);

    restartButton.setSize(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color::Green);
    restartButton.setPosition(400, 400);

    exitButton.setSize(sf::Vector2f(200, 50));
    exitButton.setFillColor(sf::Color::Red);
    exitButton.setPosition(700, 400);

    restartText.setFont(arialFont);
    restartText.setString("Restart");
    restartText.setCharacterSize(30);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(450, 410);

    exitText.setFont(arialFont);
    exitText.setString("Exit");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(770, 410);

    window.draw(restartButton);
    window.draw(restartText);
    window.draw(exitButton);
    window.draw(exitText);

    window.display();
}

void DimaTyper::handleMouseClick2(sf::Vector2f mousePose) {
    if (restartButton.getGlobalBounds().contains(mousePose)) {
        textObjects.clear();
        inputT.clear();
        score = 0;
        count = 0;
        stopGame = false;
    }
    if (exitButton.getGlobalBounds().contains(mousePose)) {
        window.close();
    }
}

void DimaTyper::startGameRunning() {
    gameRunning = true;
}

void DimaTyper::highlightText(sf::Text &text) {
    std::string word = text.getString();
    std::string typeLetter = inputT;

    if (word.find(typeLetter) == 0) {
        sf::Text highT;
        sf::Text blackT;

        highT.setFont(arialFont);
        highT.setString(typeLetter);
        highT.setCharacterSize(text.getCharacterSize());
        highT.setFillColor(sf::Color::Green);
        highT.setPosition(text.getPosition());

        blackT.setFont(arialFont);
        blackT.setString(word.substr(typeLetter.size()));
        blackT.setCharacterSize(text.getCharacterSize());
        blackT.setFillColor(text.getFillColor());

        float typedWidth = highT.getGlobalBounds().width;
        blackT.setPosition(text.getPosition().x + typedWidth, text.getPosition().y);

        window.draw(highT);
        window.draw(blackT);
    } else {
        window.draw(text);
    }
}

void DimaTyper::increaseTextSize() {
    int newSize = getTextSize() + 1;
    updateSize(newSize);
}
void DimaTyper::decreaseTextSize(){
    int newSize = getTextSize() - 1;
    updateSize(newSize);
}