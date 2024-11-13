#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "../utils/Event.hpp"
#include "gui/GuiElement.h"
#include "gui/Textbox.h"
#include "gui/TeXDisplay.h"
#include "gui/Button.h"
#include "gui/LeftList.h"
#include <mutex>

namespace App {

	// The color of the statement zone-graph separation line
	static const sf::Color unselectedColor(0x708090ff);
	static const sf::Color addBoxButtonColor(0x55616Dff);
	static const sf::Color selectedColor(0x75BAFFff);
	static const sf::Color textBoxUnselectedColor(0xc200edff);
	static const sf::Color textBoxSelectedColor(0xff0000ff);

	// The width of the statement zone-graph separation line
	static const int statementGraphDelimitationWidth = 2;

	static const int newBoxButtonHeight = 50;

	extern sf::RenderWindow* windowRef;

	// The location on the x axis of the statement zone-graph separtion line
	extern int statementGraphDelimitation;

	extern int upperSpace;

	extern GuiElement screen;

	// The amount by which the expression box list has been scroller
	extern float leftListScroll;

	extern LeftList* leftList;

	extern bool stale;

	void renderLoop();
	void setStale();
	void snapScroll();
	void scrollToEnd();
};

