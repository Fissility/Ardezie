#pragma once

#include "../../slate/SlateContext.h"
#include "./GuiElement.h"
#include "./Button.h"
#include "TeXDisplay.h"

class LeftList : public GuiElement{

private:

	// Width of the list
	int width;
	// Height of the expression boxes
	int boxHeight;
	// Height of the add new box button
	int buttonHeight;
	// Associated string list with the expression boxes
	SlateContext& assosciatedInstance;

	sf::Texture plusTexture;
	Button* addNewExpressionButton;

	std::vector<TeXDisplay*> expressionBoxList;

public:

	Event<> newBox;

	LeftList(sf::Vector2f position, int width, int boxHeight, int buttonHeight, sf::Color newBoxColor, SlateContext& assosciatedInstance);
	void update();
	int getFullSize();
	void onAddButtonPressed(size_t id);
	void changeWidth(int newWidth);
	void repositionListElements();
};

