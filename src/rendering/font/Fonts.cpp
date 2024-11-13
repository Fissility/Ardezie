#include "Fonts.h"

sf::Font Fonts::textFont;
sf::Font Fonts::TeXFont;

void Fonts::loadFonts() {

	Fonts::TeXFont.loadFromFile("./fonts/cmu_typewritter.ttf");
	Fonts::textFont.loadFromFile("./fonts/cmu_serif.ttf");

}
