#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <mutex>
#include "../../utils/Event.hpp"
#include "../font/Fonts.h"
#include "GuiElement.h"

class Textbox : public GuiElement{
private:

	// Timer used for the blinking animation of the cursor
	sf::Clock cursorBlink;

	// Text inside the text box
	std::string& text;

	// List of sf text render elemets for each wrapped line
	std::vector<sf::Text> lines;

	// List of where each text render lines starts in the texts
	std::vector<size_t> linesStartingIndex;

	// The first draw rectangle after which the background rectangle is drawn and the border obtained
	sf::RectangleShape rectBorder;
	// The background recnagel
	sf::RectangleShape solidBackground;
	// The text cursor
	sf::RectangleShape cursor;

	// The color of the text box border when it is not selected
	sf::Color unselectedBorderColor;
	// The color of the text box border when it is selected
	sf::Color selectedBorderColor;

	/*
	* @brief Used to check if the cursor is inside the texbox
	* @param x = X coordinate of the cursor
	* @param y = Y coordinate of the cursor
	* @returns TRUE if the cursor is inside the textbox, FALSE otherwise
	*/
	bool insideTexbox(int x, int y);
	/*
	* @brief Clears and remakes the lines list of the text render elements. Called after the text is changed.
	*/
	void partitionText();

	// Reference to the used font
	sf::Font& font;

public:

	// Event triggered when the contained text is changed by the user
	Event<> onTextChanged;
	Event<> onSizeChanged;

	// TRUE if the textbox is selected
	bool selected = false;
	// TRUE if the cursor is shown
	bool cursorShown = true;

	// Thickness of the textbox borders
	unsigned int borderThickness = 2;
	// Horizontal spacing between the border of the textbox and the text
	unsigned int textHorzionalMargin = 5;
	// Vertical spacing between the upper border of the texbox and the text
	unsigned int textVerticalMarginUp = 5;
	// Vertical spacing between the lower border of the textbox and the text
	unsigned int textVerticalMarginDown = 15;
	// Size of the characters used
	unsigned int characterSize = 20;
	// Wrap width of the textbox, by default set to the horizontal size minus the horizontal margin
	unsigned int wrapWidth;
	// Space between text lines
	unsigned int lineSpacing = characterSize;

	// Position of the type cursor
	sf::Vector2i lineColumn;

	int minY;

	Textbox(sf::Vector2f position, sf::Vector2f size,sf::Font& font,std::string& inputString);

	void update();

	/*
	* @brief Restarts the blinking animation of the cursor
	*/
	void restartCursor();
	/*
	* @brief Resizes the textbox if the text inside is too large
	*/
	void updateHeight();
	void onWindowEvent(sf::Event& event);
	void onWindowDraw(sf::RenderWindow& window);
	void onResize(sf::Vector2f newSize);
	void setBorderColors(const sf::Color& unselected, const sf::Color& selected);
	void setBackgroundColor(const sf::Color& color);
	void selectCursorLast();
	sf::Vector2f getTotalSize();

};

