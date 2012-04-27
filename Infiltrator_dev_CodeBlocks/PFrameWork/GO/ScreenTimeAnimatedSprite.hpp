#ifndef SCREENTIMEANIMATEDSPRITE_HPP
#define SCREENTIMEANIMATEDSPRITE_HPP

#include "ScreenSpaceDrawable.hpp"
#include "TimeAnimatedSprite.hpp"

class ScreenTimeAnimatedSprite : public TimeAnimatedSprite, public ScreenSpaceDrawable {
public:
	ScreenTimeAnimatedSprite (AlignH alignH = LEFT, AlignV alignV = TOP, int layerDepth = 0)
		: DrawableBase(layerDepth),
		TimeAnimatedSprite(layerDepth),
		ScreenSpaceDrawable(alignH, alignV, layerDepth) {}

	ScreenTimeAnimatedSprite (const SpriteAnimation* anim,
							AlignH alignH = LEFT, AlignV alignV = TOP,
							int layerDepth = 0)
		: DrawableBase(layerDepth),
		TimeAnimatedSprite(anim, layerDepth),
		ScreenSpaceDrawable(alignH, alignV, layerDepth) {}

    ScreenTimeAnimatedSprite (ScreenTimeAnimatedSprite const& xScreenTimeAnimatedSprite)
		: GameObject(xScreenTimeAnimatedSprite),
		DrawableBase(xScreenTimeAnimatedSprite),
		TimeAnimatedSprite(xScreenTimeAnimatedSprite),
		ScreenSpaceDrawable(xScreenTimeAnimatedSprite) {}
};

#endif // SCREENTIMEANIMATEDSPRITE_HPP
