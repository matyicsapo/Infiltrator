#include "Sprite.hpp"

#include "ResourceManager.hpp"

Sprite::Sprite (std::string textureFile, float layerDepth) : DrawableBase(layerDepth) {
	sf::Image* imgPtr = ResourceManager::Instance()->GetImage(textureFile);
	if (imgPtr != 0) {
		sfSprite = new sf::Sprite(*imgPtr);

		sfSprite->SetCenter(imgPtr->GetWidth() / 2, imgPtr->GetHeight() / 2);
	}
}

void Sprite::SetImage (std::string textureFile) {
	sf::Image* imgPtr = ResourceManager::Instance()->GetImage(textureFile);
	if (imgPtr != 0) {
		if (sfSprite == 0)
			sfSprite = new sf::Sprite(*imgPtr);
		else
			sfSprite->SetImage(*imgPtr);

		sfSprite->SetCenter(imgPtr->GetWidth() / 2, imgPtr->GetHeight() / 2);
	}
}
