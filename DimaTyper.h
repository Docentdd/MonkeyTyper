//
// Created by nakon on 5/21/2024.
//

#ifndef MONKEYTYPERPROJECT_DIMATYPER_H
#define MONKEYTYPERPROJECT_DIMATYPER_H


#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "FirstWindow.h"



class DimaTyper {
    public:
        DimaTyper();
        bool run();
        void updateFont(const sf::Font &newFont);
        void updateSize(const int &newSize);
        int getTextSize() const;
        void startGameRunning();

private:
    sf::RenderWindow window;
    int textSize{20};
    float textSpeed;

    std::vector<std::string> wordsFileVec;

    sf::Font arialFont;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Text rectangleT;
    std::string inputT;

    sf::Text scoreT;
    int score = 0;

    sf::Text example;

    std::vector<sf::Text> textObjects;

    float time;

    sf::Keyboard keyboard;


    void getWordsFromFile(const std::string& fileName);
    std::string randomWord();

    void createText();
    void moveDrawText();


    void lowerBar();

    void keyboardInput(const sf::Event &event);

    void outOfBorder();
    int count = 0;

    void gameOver();
    sf::RectangleShape restartButton,exitButton;
    sf::Text restartText,exitText;
    bool stopGame;
    void handleMouseClick2(sf::Vector2f mousePose);

    bool gameRunning = false;


    bool isPaused;
    sf::RectangleShape pause;


    void highlightText(sf::Text &text);
    void increaseTextSize();
    void decreaseTextSize();



};


#endif //MONKEYTYPERPROJECT_DIMATYPER_H
