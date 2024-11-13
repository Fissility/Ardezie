#include "TeXDisplay.h"
#include "../../tex_renderer/tex_renderer.h"
#include "../Utils.h"

bool TeXDisplay::insideTeXDisplay(int x, int y) {
	sf::Vector2f pos = getActualPosition();
	return x >= pos.x && y >= pos.y && x <= pos.x + size.x && y <= pos.y + size.y;
}


TeXDisplay::TeXDisplay(sf::Vector2f position, sf::Vector2f size, ExpressionInfo ei) :

text(ei.line)

{
	this->size = size;
	this->index = ei.index;
	setPos(position);

	closeButton = new Button(sf::Vector2f(size.x, 0), sf::Vector2f(20, size.y), 0);
	closeButton->setBackgroundColor(closeButtonColor);
	closeButtonTexture.loadFromFile("./x.png");
	closeButton->setSprite(closeButtonTexture);

	closeButton->onPressed += std::bind(&TeXDisplay::close, this, std::placeholders::_1);

	rectBorder.setSize(size);

	solidBackground.setPosition(sf::Vector2f(borderThickness, borderThickness));
	solidBackground.setSize(size - sf::Vector2f(borderThickness * 2, borderThickness * 2));
	solidBackground.setFillColor(sf::Color::White);

	textbox = new Textbox(sf::Vector2f(0, size.y), sf::Vector2f(size.x, 40),Fonts::TeXFont, ei.line);
	textbox->onTextChanged += std::bind(&TeXDisplay::onTextUpdated, this); // Refresh sprite when user types in
	textbox->onSizeChanged += onTextboxChanged; // External event for the change in size of the textbox

	closeButton->setParent(this);
	closeButton->visible = false;
	addChild(closeButton);
	textbox->setParent(this);
	textbox->visible = false;
	addChild(textbox);

	refresh(true);
}

void TeXDisplay::onWindowEvent(sf::Event& event) {
	GuiElement::onWindowEvent(event);
	if (event.type == sf::Event::MouseButtonPressed) {
		int mouseX = event.mouseButton.x;
		int mouseY = event.mouseButton.y;
		if (insideTeXDisplay(mouseX, mouseY)) {
			drawLock.lock();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				showTextbox = true;
				selected = true;
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				showTextbox = false;
				selected = true;
			}
			if (showTextbox) {
				textbox->selectCursorLast();
			}
			onTextboxChanged.trigger();
			drawLock.unlock();
		}
		else {
			selected = textbox->selected;
		}

	}
	textbox->visible = showTextbox;
	closeButton->visible = selected;
	if (selected) {
		drawLock.lock();
		rectBorder.setFillColor(selectedColor);
		drawLock.unlock();
	}
	else if (event.type == sf::Event::MouseMoved) {
		int mouseX = event.mouseMove.x;
		int mouseY = event.mouseMove.y;
		if (insideTeXDisplay(mouseX, mouseY)) {
			drawLock.lock();
			rectBorder.setFillColor(selectedColor);
			drawLock.unlock();
		}
		else {
			drawLock.lock();
			rectBorder.setFillColor(borderColor);
			drawLock.unlock();
		}
	}
	else if (!selected) {
		drawLock.lock();
		rectBorder.setFillColor(borderColor);
		drawLock.unlock();
	}
}

void TeXDisplay::onWindowDraw(sf::RenderWindow& window) {
	if (needsToUpdate) refresh(false);
	drawLock.lock();
	window.draw(rectBorder, positionTransform);
	window.draw(solidBackground, positionTransform);
	window.draw(*TeXEquationSprite, positionTransform);
	GuiElement::onWindowDraw(window);
	drawLock.unlock();
}

void TeXDisplay::onResize(sf::Vector2f newSize) {
	this->size = newSize;
	drawLock.lock();
	rectBorder.setSize(size);
	solidBackground.setSize(size - sf::Vector2f(borderThickness * 2, borderThickness * 2));
	drawLock.unlock();
	refresh(false);
	textbox->onResize(sf::Vector2f(newSize.x,textbox->getSize().y));
	closeButton->setPos(sf::Vector2f(newSize.x,closeButton->getPosition().y));
}

void TeXDisplay::onTextUpdated() {
	refresh(false);
}

/*
* @brief Updates the TEX sprite
* @param force = If TRUE ignores the render timout and restarts the timer
*/
void TeXDisplay::refresh(bool force) {
	if (updateTimer.getElapsedTime().asMilliseconds() <= updateTimeMin * text.size() && !force) {
		needsToUpdate = true;
		return;
	}
	updateTimer.restart();
	needsToUpdate = false;
	drawLock.lock();
	// Calling the TEX renderer
	TeXRenderer::TeXImageRenderedererResponse response = TeXRenderer::TeX2Image(
		text, mfl::points::quantity(fontSize),
		mfl::pixels::quantity(size.x - formulaHorizontalMargin*2),
		mfl::pixels::quantity(size.y - formulaHorizontalMargin*2),
		mfl::pixels::quantity(0)
	);
	// Creating a sprite from the TEX image
	if (!response.error.has_value()) {
		if (TeXEquationTexture != nullptr) delete TeXEquationTexture;
		if (TeXEquationSprite != nullptr) delete TeXEquationSprite;
		TeXEquationTexture = new sf::Texture();
		TeXEquationTexture->loadFromImage(response.image);
		TeXEquationSprite = new sf::Sprite();
		TeXEquationSprite->setTexture(*TeXEquationTexture);
		TeXEquationSprite->setPosition(sf::Vector2f(formulaHorizontalMargin, formulaHorizontalMargin));
	}
	drawLock.unlock();
}

void TeXDisplay::setBackgroundColor(sf::Color backgroundColor) {
	solidBackground.setFillColor(backgroundColor);
	textbox->setBackgroundColor(backgroundColor);
}

void TeXDisplay::setBorderColor(sf::Color borderColor,sf::Color selectedColor, sf::Color textBorderColor, sf::Color textSelectedColor) {
	rectBorder.setFillColor(borderColor);
	textbox->setBorderColors(textBorderColor,textSelectedColor);
	this->borderColor = borderColor;
	this->selectedColor = selectedColor;
}

void TeXDisplay::close(size_t id) {
	old = true;
}

sf::Vector2f TeXDisplay::getTotalSize() {
	return sf::Vector2f(size.x,textbox->getTotalSize().y * showTextbox + size.y);
}

std::string& TeXDisplay::getTextRef() {
	return text;
}
