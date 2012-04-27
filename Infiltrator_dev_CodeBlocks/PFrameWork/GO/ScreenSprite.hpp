#ifndef SCREENSPRITE_HPP
#define SCREENSPRITE_HPP

#include "ScreenSpaceDrawable.hpp"
#include "Sprite.hpp"

class ScreenSprite : public Sprite, public ScreenSpaceDrawable {
public:
	ScreenSprite (std::string textureFile, int layerDepth = 0, unsigned int entityType = 0)
		: GameObject(entityType), DrawableBase(layerDepth, entityType), Sprite(textureFile, layerDepth, entityType), ScreenSpaceDrawable(LEFT, TOP, layerDepth, entityType) {}

	ScreenSprite (std::string textureFile, AlignH alignH = LEFT, AlignV alignV = TOP, int layerDepth = 0, unsigned int entityType = 0)
		: GameObject(entityType), DrawableBase(layerDepth, entityType), Sprite(textureFile, layerDepth, entityType), ScreenSpaceDrawable(alignH, alignV, layerDepth, entityType) {}

	ScreenSprite (AlignH alignH = LEFT, AlignV alignV = TOP, int layerDepth = 0, unsigned int entityType = 0)
		: GameObject(entityType), DrawableBase(layerDepth, entityType), Sprite(layerDepth, entityType), ScreenSpaceDrawable(alignH, alignV, layerDepth, entityType) {}

    ScreenSprite (ScreenSprite const& xScreenSprite)
		: GameObject(xScreenSprite), DrawableBase(xScreenSprite), Sprite(xScreenSprite), ScreenSpaceDrawable(xScreenSprite) {}

	bool Contains (sf::Vector2f globalPosition);
};

#endif // SCREENSPRITE_HPP
