#include "Sprite.hpp"

#include <cmath>
#include "../ResourceManager.hpp"
//#include <iostream>

Sprite::Sprite (std::string textureFile, int layerDepth, unsigned int entityType) : DrawableBase(layerDepth) {
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

Sprite::Sprite (Sprite const& xSprite) : DrawableBase(xSprite) {
	sfSprite = new sf::Sprite( *(xSprite.sfSprite) );
}

Sprite::~Sprite () {
	delete sfSprite;
}

void Sprite::ReleaseResources () {
	Resources->ReleaseImage(sfSprite->GetImage());
	delete sfSprite;
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

sf::Vector2f Sprite::GetScreenSize () {
	return sf::Vector2f(sfSprite->GetImage()->GetWidth() * sfSprite->GetScale().x,
						sfSprite->GetImage()->GetHeight() * sfSprite->GetScale().y);
}

bool Sprite::Contains (sf::Vector2f globalPosition) {
	sf::Vector2f size(sfSprite->GetImage()->GetWidth() * GetScale().x,
						sfSprite->GetImage()->GetHeight() * GetScale().y);

	return std::abs(globalPosition.x - basePos.x) <= size.x / 2
			&&
		std::abs(globalPosition.y - basePos.y) <= size.y / 2;
}

bool Sprite::IsOpaque (sf::Vector2f globalPosition) {
	// a 2 megoldás azonos

//	sf::Matrix3 matrixToGlobal;
//	matrixToGlobal.SetFromTransformations(sf::Vector2f(0, 0), GetPosition(), GetRotation(), GetScale());
//	sf::Matrix3 matrixToLocal = matrixToGlobal.GetInverse();
//	sf::Vector2f localPosition = matrixToLocal.Transform(globalPosition);

	sf::Vector2f localPosition = sfSprite->TransformToLocal(globalPosition) / windowFakeScale;

	return sfSprite->GetPixel(localPosition.x, localPosition.y).a != 0;
}
