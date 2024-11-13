#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <mutex>
#include "GuiElement.h"
#include "Textbox.h"
#include "Button.h"
#include "../../slate/SlateContext.h"

class TeXDisplay : public GuiElement {

private:

	// Draw thread lock
	std::mutex drawLock;

	// Texture of the rendered TeX string (reconstructed each time)
	sf::Texture* TeXEquationTexture;
	// Sprite of the rendered TeX string (reconstructed each time)
	sf::Sprite* TeXEquationSprite;

	// The first draw rectangle after which the background rectangle is drawn and the border obtained
	sf::RectangleShape rectBorder;
	// The background recnagel
	sf::RectangleShape solidBackground;

	// Color of the background solid color (as of now it isn't visible as the TeX render is over it)
	sf::Color backgroundColor;
	// Border non-selected color
	sf::Color borderColor;
	// Border selected color
	sf::Color selectedColor;
	// Close button color
	sf::Color closeButtonColor = sf::Color(0xa80600ff);

	// Texture of the closeButton
	sf::Texture closeButtonTexture;

	// Timer used to timeout the rendering
	sf::Clock updateTimer;

	// Text string reference
	std::string& text;

	// The textbox assosciated with the display
	Textbox* textbox;

	// The close button
	Button* closeButton;

	bool insideTeXDisplay(int x, int y);

public:

	Event<> onTextboxChanged;

	// Thickness of the textbox borders
	unsigned int borderThickness = 2;
	unsigned int formulaHorizontalMargin = 10;

	// TeX font size
	unsigned int fontSize = 11;

	// TRUE if the rendered result is stale, happenes if the rendertimout was activated
	bool needsToUpdate = true;
	// ms/character in the TeX string, which results in the timeout
	unsigned int updateTimeMin = 2; // ms

	bool showTextbox = false;
	bool selected = false;

	size_t index;

	TeXDisplay(sf::Vector2f position, sf::Vector2f size, ExpressionInfo ei);

	void onWindowEvent(sf::Event& event);
	void onWindowDraw(sf::RenderWindow& window);
	void onResize(sf::Vector2f newSize);
	void onTextUpdated();
	void refresh(bool force);
	void setBackgroundColor(sf::Color backgroundColor);
	void setBorderColor(sf::Color borderColor, sf::Color selectedColor, sf::Color textBorderColor, sf::Color textSelectedColor);
	void close(size_t id);
	sf::Vector2f getTotalSize();
	std::string& getTextRef();
};