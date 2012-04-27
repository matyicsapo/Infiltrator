#include "ResourceManager.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "PFWConstants.hpp"

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
        	imgNew->SetSmooth(false); // hides tiling seams, makes sprites rough - not that bad
            return images.insert(std::pair<std::string, sf::Image*>(textureFile, imgNew)).first->second;
        }

        return 0;
    }
}

void ResourceManager::ReleaseImage (std::string textureFile) {
	std::map<std::string, sf::Image*>::iterator img = images.find(textureFile);
	if (img != images.end()) {
		delete img->second;
		images.erase(img);
	}
}

void ResourceManager::ReleaseImage (const sf::Image* img) {
	for (std::map<std::string, sf::Image*>::iterator itTexture = images.begin();
			itTexture != images.end(); itTexture++) {
		if (itTexture->second == img) {
			delete img;
			images.erase(itTexture);
		}
	}
}

sf::Font* ResourceManager::GetFont (std::string fontFile) {
	if (fonts.count(fontFile) == 1) {
        return fonts.find(fontFile)->second;
    }
    else {
        sf::Font* fontNew = new sf::Font();

        if (fontNew->LoadFromFile(fontFile, 30, PFWConstants::charset)) {
            return fonts.insert(std::pair<std::string, sf::Font*>(fontFile, fontNew)).first->second;
        }

        return 0;
    }
}

void ResourceManager::ReleaseFont (std::string fontFile) {
	std::map<std::string, sf::Font*>::iterator fnt = fonts.find(fontFile);
	if (fnt != fonts.end()) {
		delete fnt->second;
		fonts.erase(fnt);
	}
}

void ResourceManager::ReleaseFont (const sf::Font* font) {
	for (std::map<std::string, sf::Font*>::iterator itFont = fonts.begin();
			itFont != fonts.end(); itFont++) {
		if (itFont->second == font) {
			delete font;
			fonts.erase(itFont);
		}
	}
}

sf::SoundBuffer* ResourceManager::GetSound (std::string soundFile) {
    if (sounds.count(soundFile) == 1) {
        return sounds.find(soundFile)->second;
    }
    else {
        sf::SoundBuffer* soundNew = new sf::SoundBuffer();
        if (soundNew->LoadFromFile(soundFile)) {
            return sounds.insert(std::pair<std::string, sf::SoundBuffer*>(soundFile, soundNew)).first->second;
        }

        return 0;
    }
}

void ResourceManager::ReleaseSound (std::string soundFile) {
	std::map<std::string, sf::SoundBuffer*>::iterator sound = sounds.find(soundFile);
	if (sound != sounds.end()) {
		delete sound->second;
		sounds.erase(sound);
	}
}

void ResourceManager::ReleaseSound (const sf::SoundBuffer* sound) {
	for (std::map<std::string, sf::SoundBuffer*>::iterator itSound = sounds.begin();
			itSound != sounds.end(); itSound++) {
		if (itSound->second == sound) {
			delete sound;
			sounds.erase(itSound);
		}
	}
}
