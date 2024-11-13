#include "Textbox.h"
#include <iostream>


Textbox::Textbox(sf::Vector2f position, sf::Vector2f size, sf::Font& font, std::string& inputString) : font(font), text(inputString) {

    setPos(position);
    this->size = size;
    this->minY = size.y;

    positionTransform.translate(position);

    //rectBorder.setPosition(position);
    rectBorder.setSize(size);

    solidBackground.setPosition(sf::Vector2f(borderThickness, borderThickness));
    solidBackground.setSize(size - sf::Vector2f(borderThickness * 2, borderThickness * 2));
    wrapWidth = size.x - textHorzionalMargin * 2;

    cursor.setFillColor(sf::Color::Black);
    cursor.setSize(sf::Vector2f(2, lineSpacing));

    partitionText();

}

void Textbox::update() {
    GuiElement::update();
    if (selected) {
        if (cursorBlink.getElapsedTime().asMilliseconds() >= 500) {
            cursorBlink.restart();
            cursorShown = !cursorShown;
            renderStale = true;
        }
    }
}

/**
* @brief Checks if a screen position is inside the text box.
* @param x = The x coordinate
* @param y = The y coordinate
*/
bool Textbox::insideTexbox(int x, int y) {
    sf::Vector2f pos = getActualPosition();
    return x >= pos.x && y >= pos.y && x <= pos.x + size.x && y <= pos.y + size.y;
}

/**
* @brief Refreshes an converts the continous text string into a sectioned list of strings which are then directly displayed.
*/
void Textbox::partitionText() {

    lines.clear();
    linesStartingIndex.clear();

    size_t offset = 0;
    size_t line = 0;
    size_t lastIndex = 0;


    for (size_t i = 0; i < text.size(); i++) {
        int advance = font.getGlyph(text[i], characterSize, false).advance;
        bool newLine = text[i] == '\n';
        if (offset + advance >= wrapWidth || newLine) {
            sf::Text t;
            t.setFont(font);
            t.setString(text.substr(lastIndex, i - lastIndex));
            t.setPosition(sf::Vector2f(textHorzionalMargin, textVerticalMarginUp + line * lineSpacing));
            t.setFillColor(sf::Color::Black);
            t.setCharacterSize(characterSize);
            lines.push_back(t);
            linesStartingIndex.push_back(lastIndex);
            lastIndex = i + newLine;
            line++;
            offset = 0;
        }
        if (!newLine) {
            offset += advance;
        }
    }
    sf::Text t;
    t.setFont(font);
    t.setString(text.substr(lastIndex, text.size() - lastIndex));
    t.setPosition(sf::Vector2f(textHorzionalMargin, textVerticalMarginUp + line * lineSpacing));
    t.setFillColor(sf::Color::Black);
    t.setCharacterSize(characterSize);
    lines.push_back(t);
    linesStartingIndex.push_back(lastIndex);

    rectBorder.setSize(size);
    solidBackground.setSize(size - sf::Vector2f(borderThickness * 2, borderThickness * 2));

    onTextChanged.trigger();

    updateHeight();

}

/*
* @brief Restarts cursor blinking
*/
void Textbox::restartCursor() {
    cursorBlink.restart();
    cursorShown = true;
}

/*
* @brief Updates the size of the textbox based on the current list of lines
*/
void Textbox::updateHeight() {
    unsigned int minSize = lines.size() * lineSpacing + textVerticalMarginUp + textVerticalMarginDown;
    if (minSize <= minY && size.y >= minY) {
        minSize = minY;
    }
    float oldSize = this->size.y;
    this->size.y = minSize;
    rectBorder.setSize(size);
    solidBackground.setSize(size - sf::Vector2f(borderThickness * 2, borderThickness * 2));
    wrapWidth = size.x - textHorzionalMargin * 2;
    if (minSize != oldSize) {
        onSizeChanged.trigger();
    }
}

/*
* @return Returns the size of the texbox
*/
sf::Vector2f Textbox::getTotalSize() {
    return size;
}

/*
* @return Put the textbox into focus and set the cursor at the end of the inside text
*/
void Textbox::selectCursorLast() {
    selected = true;
    rectBorder.setFillColor(selectedBorderColor);
    lineColumn.y = lines.size() - 1;
    lineColumn.x = lines[lines.size() - 1].getString().getSize();
}

void Textbox::onResize(sf::Vector2f newSize) {
    this->size = newSize;
    rectBorder.setSize(size);
    solidBackground.setSize(size - sf::Vector2f(borderThickness * 2, borderThickness * 2));
    wrapWidth = size.x - textHorzionalMargin * 2;
    partitionText();
    if (lineColumn.y >= lines.size()) {
        lineColumn.y = lines.size() - 1;
        lineColumn.x = lines[lines.size() - 1].getString().getSize();
    }
}

void Textbox::onWindowEvent(sf::Event& event) {
    if (!visible) return;
    // Text box click handling
    if (event.type == sf::Event::MouseButtonPressed) {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            restartCursor();
            if (insideTexbox(mouseX, mouseY)) {
                selected = true;
                rectBorder.setFillColor(selectedBorderColor);
                sf::Vector2f pos = getActualPosition();
                unsigned int beginY = pos.y + textVerticalMarginUp;
                unsigned int beginX = pos.x + textHorzionalMargin;
                unsigned int endY = pos.y + size.y - textVerticalMarginUp;
                unsigned int endX = pos.x + size.x - textHorzionalMargin;
                if (mouseY >= beginY && mouseX >= beginX && mouseY <= endY && mouseX <= endX) {
                    unsigned line = (mouseY - beginY) / lineSpacing;
                    if (line < lines.size()) {
                        sf::Text& sfText = lines[line];
                        lineColumn.y = line;
                        lineColumn.x = sfText.getString().getSize();
                        for (size_t i = 0; i < sfText.getString().getSize(); i++) {
                            if (sfText.findCharacterPos(i).x <= mouseX && sfText.findCharacterPos(i + 1).x >= mouseX) {
                                lineColumn.x = i;
                                break;
                            }
                        }                   
                    }
                }
            } else {
                selected = false;
                rectBorder.setFillColor(unselectedBorderColor);
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            if (!insideTexbox(mouseX, mouseY)) {
                selected = false;
                rectBorder.setFillColor(unselectedBorderColor);
            }
        }
    }
    if (!selected) return;
    // Text insertion
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode < 0x80 && event.text.unicode != 0x08) {
            size_t line = lineColumn.y;
            if (line < linesStartingIndex.size()) {
                size_t index = linesStartingIndex[line] + lineColumn.x;
                if (index <= text.size()) {
                    restartCursor();
                    text.insert(index, { (event.text.unicode != 13 ? (char)event.text.unicode : '\n') });
                    lineColumn.x += 1;
                    partitionText();
                    if (lineColumn.x > lines[lineColumn.y].getString().getSize()) {
                        lineColumn.x = event.text.unicode != 13;
                        lineColumn.y += 1;
                    }
                }
            }
        }
        if (event.text.unicode == 0x08 && text.size() != 0) {
            size_t line = lineColumn.y;
            if (line < linesStartingIndex.size()) {
                size_t index = linesStartingIndex[line] + lineColumn.x;
                if (index <= text.size() && index != 0) {
                    restartCursor();
                    text.erase(index - 1, 1);
                    lineColumn.x -= 1;
                    if (lineColumn.x <= 0 && lineColumn.y != 0 && !((lineColumn.x == 0 && index - 2 < text.size() && text[index - 2] == '\n'))) {
                        lineColumn.y--;
                        lineColumn.x = lines[lineColumn.y].getString().getSize();
                    }
                    partitionText();
                }
            }
        }
    }
    // Arrow navigation
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            lineColumn.x--;
            restartCursor();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            lineColumn.x++;
            restartCursor();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            lineColumn.y--;
            restartCursor();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            lineColumn.y++;
            restartCursor();
        }
        if (lineColumn.y < 0) lineColumn.y = 0;
        if (lineColumn.y >= lines.size()) {
            lineColumn.y = lines.size() - 1;
        }
        if (lineColumn.x < 0) {
            lineColumn.y--;
            if (lineColumn.y < 0) {
                lineColumn.y = 0;
                lineColumn.x = 0;
            }
            else {
                lineColumn.x = lines[lineColumn.y].getString().getSize();
            }
        }
        if (lineColumn.x > lines[lineColumn.y].getString().getSize()) {
            lineColumn.y++;
            if (lineColumn.y >= lines.size()) {
                lineColumn.y = lines.size() - 1;
                lineColumn.x = lines[lineColumn.y].getString().getSize();
            }
            else {
                lineColumn.x = 0;
            }
        }
    }
}

void Textbox::onWindowDraw(sf::RenderWindow& window) {
    GuiElement::onWindowDraw(window);
    if (!visible) return;
    window.draw(rectBorder, positionTransform);
    window.draw(solidBackground, positionTransform);
    if (selected && cursorShown) {
        sf::Text& line = lines[lineColumn.y];
        cursor.setPosition(sf::Vector2f(line.findCharacterPos(lineColumn.x).x, textVerticalMarginUp + lineColumn.y * lineSpacing - (cursor.getSize().y - lineSpacing - 4) / 2));
        window.draw(cursor, positionTransform);
    }
    for (size_t i = 0; i < lines.size(); i++) window.draw(lines[i], positionTransform);
}

/*
* @brief Sets the border colors of the element
* @parameter unselected = The border color of the text box when it is out of focus (unselected)
* @parameter selected = The border color of the text box when it is in focus (selected)
*/
void Textbox::setBorderColors(const sf::Color& unselected, const sf::Color& selected) {
    this->unselectedBorderColor = unselected;
    this->selectedBorderColor = selected;
    rectBorder.setFillColor(unselectedBorderColor);
}

void Textbox::setBackgroundColor(const sf::Color& color) {
    solidBackground.setFillColor(color);
}