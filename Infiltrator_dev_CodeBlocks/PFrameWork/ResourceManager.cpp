#include "ResourceManager.hpp"

#include <SFML/Graphics.hpp>

ResourceManager* ResourceManager::Instance () {
	static ResourceManager instance;

	return &instance;
}

ResourceManager::~ResourceManager () {
	for (std::map<std::string, sf::Image*>::iterator itImg = images.begin(); itImg != images.end(); itImg++) {
		delete itImg->second;
	}

	for (std::map<std::string, sf::Font*>::iterator itFont = fonts.begin(); itFont != fonts.end(); itFont++) {
		delete itFont->second;
	}
}

sf::Image* ResourceManager::GetImage (std::string textureFile) {
    if (images.count(textureFile) == 1) {
        return images.find(textureFile)->second;
    }
    else {
        sf::Image* imgNew = new sf::Image();
        if (imgNew->LoadFromFile(textureFile)) {
            return images.insert(std::pair<std::string, sf::Image*>(textureFile, imgNew)).first->second;
        }

        return 0;
    }
}

sf::Font* ResourceManager::GetFont (std::string fontFile) {
	if (fonts.count(fontFile) == 1) {
        return fonts.find(fontFile)->second;
    }
    else {
        sf::Font* fontNew = new sf::Font();
        if (fontNew->LoadFromFile(fontFile)) {
            return fonts.insert(std::pair<std::string, sf::Font*>(fontFile, fontNew)).first->second;
        }

        return 0;
    }
}
