#ifndef WORLDTIMEANIMATEDSPRITE_HPP
#define WORLDTIMEANIMATEDSPRITE_HPP

#include "WorldDrawable.hpp"
#include "TimeAnimatedSprite.hpp"

class WorldTimeAnimatedSprite : public TimeAnimatedSprite, public WorldDrawable {
public:
	WorldTimeAnimatedSprite (int layerDepth = 0)
		: DrawableBase(layerDepth),
		TimeAnimatedSprite(layerDepth),
		WorldDrawable(layerDepth) {}

	WorldTimeAnimatedSprite (const SpriteAnimation* anim, int layerDepth = 0)
		: DrawableBase(layerDepth),
		TimeAnimatedSprite(anim, layerDepth),
		WorldDrawable(layerDepth) {}

    WorldTimeAnimatedSprite (WorldTimeAnimatedSprite const& xWorldTimeAnimatedSprite)
		: GameObject(xWorldTimeAnimatedSprite),
		DrawableBase(xWorldTimeAnimatedSprite),
		TimeAnimatedSprite(xWorldTimeAnimatedSprite),
		WorldDrawable(xWorldTimeAnimatedSprite) {}
};

#endif // WORLDTIMEANIMATEDSPRITE_HPP
