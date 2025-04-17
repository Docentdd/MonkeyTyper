//
// Created by nakon on 5/15/2024.
//

#ifndef MONKEYTYPERPROJECT_FIRSTWINDOW_H
#define MONKEYTYPERPROJECT_FIRSTWINDOW_H


#include <SFML/Graphics.hpp>
#include "DimaTyper.h"

class DimaTyper;

class FirstWindow {

public:
    FirstWindow(DimaTyper &dimaTyper);
    bool run();
private:
    DimaTyper  &dimaTyper;

    sf::RenderWindow window;

    sf::Font arialFont, njaBoldF, empireF, typographicaF, warbonesF;

    sf::Texture texture;
    sf::Sprite logoImage;

    sf::RectangleShape playB, settingsB;
    sf::RectangleShape arialB, njaB, typographicaB;
    sf::RectangleShape plusB, minusB;

    sf::Text playT, settingsT, titleSize;
    sf::Text arialT, njaT, typographicaT;
    sf::Text plusT, minusT;
    sf::Text example;

    bool game = false;
    bool showSettings = false;

    static void updatePosition(sf::Text& text, sf::RectangleShape& button, float x, float y);
    void handleMouseClick(sf::Vector2f mousePos);

    void createFontButtonText(sf::RectangleShape &button, sf::Text &buttonText, const sf::Font &font, const std::string &label, float& posY);
    void settingsMenu();
    void createPLusMinusButton(sf::RectangleShape &button, sf::Text &buttonText, const sf::Font &font, const std::string &label, float& posY);
};


#endif //MONKEYTYPERPROJECT_FIRSTWINDOW_H
