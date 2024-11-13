#include "Button.h"

bool Button::insideButton(int x, int y) {
    sf::Vector2f pos = getActualPosition();
    return x >= pos.x && y >= pos.y && x <= pos.x + size.x && y <= pos.y + size.y;
}

Button::Button(sf::Vector2f position, sf::Vector2f size, size_t buttonId) {
    setPos(position);
    this->size = size;
    this->buttonId = buttonId;
	solidBackground.setSize(size);
}

void Button::onWindowEvent(sf::Event& event) {
	if (!visible) return;
	if (event.type == sf::Event::MouseButtonPressed) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int mouseX = event.mouseButton.x;
			int mouseY = event.mouseButton.y;
			if (insideButton(mouseX, mouseY)) {
				onPressed.trigger(buttonId);
			}
		}
	}
}

void Button::onWindowDraw(sf::RenderWindow& window) {
	GuiElement::onWindowDraw(window);
	if (!visible) return;
	window.draw(solidBackground,positionTransform);
	if (hasSprite) {
		window.draw(sprite, positionTransform);
	}
}

void Button::onResize(sf::Vector2f newSize) {
    this->size = newSize;
	solidBackground.setSize(newSize);
	if (hasSprite) scaleSprite();
}

void Button::setBackgroundColor(sf::Color color) {
    solidBackground.setFillColor(color);
}

void Button::setSprite(sf::Texture& texture) {
	sprite.setTexture(texture);
	hasSprite = true;
	scaleSprite();
}

void Button::scaleSprite() {
	sf::Vector2u texSize = sprite.getTexture()->getSize();
	if (texSize.x - size.x > texSize.y - size.y) {
		if (texSize.x > size.x) {
			sprite.setScale(sf::Vector2f(size.x / texSize.x, size.x / texSize.x));
		}
		else {
			sprite.setScale(sf::Vector2f(1,1));
		}
	}
	else {
		if (texSize.y > size.y) {
			sprite.setScale(sf::Vector2f(size.y / texSize.y, size.y / texSize.y));
		}
		else {
			sprite.setScale(sf::Vector2f(1, 1));
		}
	}
	sf::Vector2f scale = sprite.getScale();
	sprite.setPosition(sf::Vector2f(size.x / 2 - texSize.x * scale.x / 2, size.y / 2 - texSize.y * scale.y / 2));
}

sf::Vector2f Button::getTotalSize() {
	return size;
}
