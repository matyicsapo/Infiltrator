#ifndef SCREENSPRITE_HPP
#define SCREENSPRITE_HPP

#include "ScreenSpaceDrawable.hpp"
#include "Sprite.hpp"

class ScreenSprite : public Sprite, public ScreenSpaceDrawable {
public:
	ScreenSprite (std::string textureFile, AlignH alignH = LEFT, AlignV alignV = TOP, float layerDepth = 0)
		: DrawableBase(layerDepth), Sprite(textureFile, layerDepth), ScreenSpaceDrawable(alignH, alignV, layerDepth) {}

	ScreenSprite (AlignH alignH = LEFT, AlignV alignV = TOP, float layerDepth = 0)
		: DrawableBase(layerDepth), Sprite(layerDepth), ScreenSpaceDrawable(alignH, alignV, layerDepth) {}

    ScreenSprite (ScreenSprite const& xScreenSprite)
		: DrawableBase(xScreenSprite), Sprite(xScreenSprite), ScreenSpaceDrawable(xScreenSprite) {}
};

#endif // SCREENSPRITE_HPP
