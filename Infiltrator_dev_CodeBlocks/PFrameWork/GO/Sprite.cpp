#include "Sprite.hpp"

#include "../ResourceManager.hpp"

Sprite::Sprite (std::string textureFile, int layerDepth) : DrawableBase(layerDepth) {
	if (textureFile != "") {
		sf::Image* imgPtr = Resources->GetImage(textureFile);

		if (imgPtr != 0) {
			sfSprite = new sf::Sprite(*imgPtr);

			sfSprite->SetCenter(imgPtr->GetWidth() / 2, imgPtr->GetHeight() / 2);
		}
	}
	else
		sfSprite = 0;
}

void Sprite::SetImage (std::string textureFile) {
	sf::Image* imgPtr = Resources->GetImage(textureFile);
	if (imgPtr != 0) {
		if (sfSprite == 0)
			sfSprite = new sf::Sprite(*imgPtr);
		else
			sfSprite->SetImage(*imgPtr);

		sfSprite->SetCenter(imgPtr->GetWidth() / 2, imgPtr->GetHeight() / 2);
	}
}
