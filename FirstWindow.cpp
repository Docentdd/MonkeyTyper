#include <iostream>
#include "FirstWindow.h"

FirstWindow::FirstWindow(DimaTyper &dimaTyper) : window(sf::VideoMode({1200, 800}), "First Window"), playT(),
                                                 playB(), arialFont(), dimaTyper(dimaTyper) {
    //Upload the fonts for the text
    if (!arialFont.loadFromFile("../../2324S_18_s30474_TW - assets/Arial.ttf")) {
        std::cout << "Error loading from file font Arial\n";
        return ;
    }
    if (!njaBoldF.loadFromFile("../../2324S_18_s30474_TW - assets/NjalBold.otf")) {
        std::cout << "Error loading from file font NjalBold\n";
        return ;
    }
    if (!typographicaF.loadFromFile("../../2324S_18_s30474_TW - assets/Typographica.ttf")) {
        std::cout << "Error loading from file font Typographica\n";
        return ;
    }
    //Upload background
    if (!texture.loadFromFile("../../2324S_18_s30474_TW - assets/backgroundForGame.jpg")) {
        std::cout << "Error loading from the file\n";
        return ;
    }
    logoImage.setTexture(texture); // done with background

    //Play button and text
    playB.setSize(sf::Vector2f(250, 50));
    playB.setFillColor(sf::Color(255, 255, 255, 0));
    playT.setFont(arialFont);
    playT.setString("Play");
    playT.setFillColor(sf::Color::Green);

    //Settings button and text
    settingsB.setSize(sf::Vector2f(140, 50));
    settingsB.setFillColor(sf::Color(255, 255, 255, 0));
    settingsT.setFont(arialFont);
    settingsT.setString("Settings");
    settingsT.setFillColor(sf::Color::White);

    //update Position for Play and Settings
    auto sizeWx = window.getSize().x / 2.0;
    auto sizeWy = window.getSize().y / 2.0 + 100;
    updatePosition(playT, playB, sizeWx, sizeWy); // set the position for PLAY
    updatePosition(settingsT, settingsB, sizeWx - 500, sizeWy + 50);

    //Creat for showing the size and font of the text in the game
    example.setString("Example 123");
    example.setFont(arialFont);  // Default font
    example.setCharacterSize(24);  // Default size
    example.setFillColor(sf::Color::White);
    example.setPosition(10, 10);
}

bool FirstWindow::run() {
    while (window.isOpen()) {
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // convert it to world coordinates
                handleMouseClick(mousePos);
                    if (game) {
                        return true; // Return true if the game should start
                    }
                }
            }
        window.clear();
        window.draw(logoImage);
        window.draw(playT);
        window.draw(playB);
        window.draw(settingsT);
        window.draw(settingsB);

        if (showSettings) { // if we will do it in the handle mouse it will not work because the window is not updating
            settingsMenu(); // Draw settings UI if settings are shown
        }
        window.display();
    }

    return false;
}

void FirstWindow::updatePosition(sf::Text &text, sf::RectangleShape &button, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(x, y);

    sf::FloatRect buttonRect = button.getLocalBounds()  ;
    button.setOrigin(buttonRect.left + buttonRect.width / 2.0f, buttonRect.top + buttonRect.height / 2.0f);
    button.setPosition(x, y);
}

void FirstWindow::handleMouseClick(sf::Vector2f mousePos) {
    if (playB.getGlobalBounds().contains(mousePos)) { // check the Play button
        game = true;
        dimaTyper.startGameRunning();
        window.close(); // Close the window
    } else if (settingsB.getGlobalBounds().contains(mousePos)) { // check the Settings button
        showSettings = !showSettings; // to make visibility and update
    } else { // check the font after clicking
        if (arialB.getGlobalBounds().contains(mousePos)) {
            dimaTyper.updateFont(arialFont);
            example.setFont(arialFont);
        }
        if (njaB.getGlobalBounds().contains(mousePos)) {
            dimaTyper.updateFont(njaBoldF);
            example.setFont(njaBoldF);
        }
        if (typographicaB.getGlobalBounds().contains(mousePos)) {
            dimaTyper.updateFont(typographicaF);
            example.setFont(typographicaF);
        }
        if (plusB.getGlobalBounds().contains(mousePos)) { // check the plus
            int newSize = dimaTyper.getTextSize() + 1;
            dimaTyper.updateSize(newSize);
            example.setCharacterSize(newSize);
        }
        if (minusB.getGlobalBounds().contains(mousePos)) { //check the minus
            int newSize = dimaTyper.getTextSize() - 1;
            dimaTyper.updateSize(newSize);
            example.setCharacterSize(newSize);
        }
    }
}

void FirstWindow::settingsMenu() {
    float posY = settingsT.getPosition().y + settingsT.getLocalBounds().height + 22;

    sf::Text titleText("Choose a font", arialFont, 24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(settingsT.getPosition().x - 50, posY - titleText.getLocalBounds().height - 11.0f);

    createFontButtonText(arialB, arialT, arialFont, "ARIAL", posY);
    createFontButtonText(njaB, njaT, njaBoldF, "NJA", posY);
    createFontButtonText(typographicaB, typographicaT, typographicaF, "Typographica", posY);

    titleSize.setString("Change the size");
    titleSize.setFont(arialFont);
    titleSize.setFillColor(sf::Color::White);
    titleSize.setPosition(typographicaB.getPosition().x - 50, posY);

    createPLusMinusButton(plusB, plusT, arialFont, "+", posY);
    createPLusMinusButton(minusB, minusT, arialFont, "-", posY);
//
    window.draw(titleText);
    window.draw(arialB);
    window.draw(arialT);
    window.draw(njaT);
    window.draw(njaB);
    window.draw(typographicaB);
    window.draw(typographicaT);
    window.draw(titleSize);
    window.draw(plusB);
    window.draw(plusT);
    window.draw(minusB);
    window.draw(minusT);
    window.draw(example);
}
//In all these methods I have references to the sf::RectangleShape and sf::Text because we need to change not local copies,
// but modify the original objects outside the function(provided by caller).
void FirstWindow::createFontButtonText(sf::RectangleShape &button, sf::Text &text, const sf::Font &font,
                                       const std::string &label, float &posY) {
    button.setSize(sf::Vector2f(100, text.getLocalBounds().height + text.getLocalBounds().height / 2));
    button.setPosition(settingsT.getPosition().x - 50, posY);
    button.setFillColor(sf::Color(255, 255, 255,0));

    text.setString(label);
    text.setFont(font);
    text.setCharacterSize(24);
    text.setPosition(settingsT.getPosition().x - 50, posY);

    posY += text.getLocalBounds().height + 11.0f;
}

void FirstWindow::createPLusMinusButton(sf::RectangleShape &button, sf::Text &buttonText, const sf::Font &font,
                                        const std::string &label, float &posY) {
    buttonText.setString(label);
    buttonText.setFont(font);
    buttonText.setCharacterSize(24);

    // Calculate the button size based on the text bounds
    button.setSize(sf::Vector2f(buttonText.getLocalBounds().width + 20, buttonText.getLocalBounds().height + 20));
    button.setFillColor(sf::Color(255, 255, 255,0));
    button.setPosition(titleSize.getPosition().x + titleSize.getLocalBounds().width + 30, posY);

    // Set the text position within the button
    buttonText.setPosition(titleSize.getPosition().x + titleSize.getLocalBounds().width + 30, posY);

    // Update posY for the next button
    posY += button.getSize().y + 10;
}

