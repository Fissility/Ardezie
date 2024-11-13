#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace Utils {

	sf::Texture* getTextureFromPath(const char* path) {
		sf::Texture* tex = new sf::Texture;
		tex->loadFromFile(path);
		return tex;
	}

}