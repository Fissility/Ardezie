#pragma once

#include "GuiElement.h"
#include "../../utils/Event.hpp"


class Button : public GuiElement {
private:

	bool insideButton(int x, int y);
	bool hasSprite = false;
	// Button indetifier, generally useful
	size_t buttonId;
	// Element that holds the solid color background of the button
	sf::RectangleShape solidBackground;
	// Optional sprite that is drawn inside the botton
	sf::Sprite sprite;

public:

	// Event, triggered when the button is pressed
	Event<size_t> onPressed;

	Button(sf::Vector2f position, sf::Vector2f size, size_t buttonId);
	void onWindowEvent(sf::Event& event);
	void onWindowDraw(sf::RenderWindow& window);
	void onResize(sf::Vector2f newSize);
	void setBackgroundColor(sf::Color color);
	void setSprite(sf::Texture& texture);
	void scaleSprite();
	sf::Vector2f getTotalSize();

};