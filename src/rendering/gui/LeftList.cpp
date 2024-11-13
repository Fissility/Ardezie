#include "LeftList.h"
#include "../App.hpp"
#include <iostream>

LeftList::LeftList(sf::Vector2f position, int width, int boxHeight, int buttonHeight, sf::Color newBoxColor, SlateContext& assosciatedInstance) :
	assosciatedInstance(assosciatedInstance),
	plusTexture()
{
	this->position = position;
	this->width = width;
	this->boxHeight = boxHeight;
	this->buttonHeight = buttonHeight;

	addNewExpressionButton = new Button(sf::Vector2f(0, 0), sf::Vector2f(width, buttonHeight), 0);
	plusTexture.loadFromFile("./plus.png");
	addNewExpressionButton->setBackgroundColor(newBoxColor);
	addNewExpressionButton->setSprite(plusTexture);
	addNewExpressionButton->onPressed += std::bind(&LeftList::onAddButtonPressed,this,std::placeholders::_1);
	addNewExpressionButton->setParent(this);
	addChild(addNewExpressionButton);

	recalculateTransofrm();
}

void LeftList::update() {
	GuiElement::update();
	for (size_t i = 0; i < expressionBoxList.size();i++) {
		if (expressionBoxList[i]->old) {
			if (i < expressionBoxList.size() - 1) {
				expressionBoxList[i + 1]->selected = true;
				expressionBoxList[i + 1]->showTextbox = expressionBoxList[i]->showTextbox;
			}
			assosciatedInstance.removeExpresion(expressionBoxList[i]->index);
			for (TeXDisplay* tt : expressionBoxList) if (tt->index > expressionBoxList[i]->index) tt->index--;
			expressionBoxList.erase(std::find(expressionBoxList.begin(), expressionBoxList.end(), expressionBoxList[i]));
			repositionListElements();
			renderStale = true;
			break;
		}
	}
}

int LeftList::getFullSize() {
	int total = 0;
	for (TeXDisplay* t : expressionBoxList) {
		total += t->getTotalSize().y;
	}
	return total + buttonHeight;
}

void LeftList::onAddButtonPressed(size_t id) {

	float yOffset = 0;
	for (int i = 0; i < expressionBoxList.size(); i++) {
		yOffset += expressionBoxList[i]->getTotalSize().y;
	}
	TeXDisplay* tex = new TeXDisplay(sf::Vector2f(0, 0), sf::Vector2f(width, boxHeight), assosciatedInstance.newExpression());
	tex->setOffset(sf::Vector2f(0, yOffset));
	tex->setBorderColor(App::unselectedColor, App::selectedColor, App::textBoxUnselectedColor, App::textBoxSelectedColor);
	tex->onTextboxChanged += std::bind(&LeftList::repositionListElements, this);
	expressionBoxList.push_back(tex);
	addChild(tex);
	tex->setParent(this);
	addNewExpressionButton->setOffset(sf::Vector2f(0, yOffset + tex->getSize().y));
	recalculateTransofrm();

	newBox.trigger();
	setStale();
	
}

/*
* @brief Repositions the expression boxes
*/
void LeftList::repositionListElements() {
	float yOffset = 0;
	for (int i = 0; i < expressionBoxList.size(); i++) {
		expressionBoxList[i]->setOffset(sf::Vector2f(0,yOffset));
		yOffset += expressionBoxList[i]->getTotalSize().y;
	}
	addNewExpressionButton->setOffset(sf::Vector2f(0, yOffset));
	recalculateTransofrm();
}

void LeftList::changeWidth(int newWidth) {
	this->width = newWidth;
	for (GuiElement* e : children) {
		e->onResize(sf::Vector2f(width, e->getSize().y));
	}
}
