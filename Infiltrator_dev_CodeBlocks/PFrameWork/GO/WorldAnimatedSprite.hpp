#ifndef WORLDANIMATEDSPRITE_HPP
#define WORLDANIMATEDSPRITE_HPP

#include "WorldDrawable.hpp"
#include "AnimatedSprite.hpp"

class WorldAnimatedSprite : public AnimatedSprite, public WorldDrawable {
public:
	WorldAnimatedSprite (int layerDepth = 0)
		: DrawableBase(layerDepth),
		AnimatedSprite(layerDepth),
		WorldDrawable(layerDepth) {}

	WorldAnimatedSprite (const SpriteAnimation* anim, int layerDepth = 0)
		: DrawableBase(layerDepth),
		AnimatedSprite(anim, layerDepth),
		WorldDrawable(layerDepth) {}

    WorldAnimatedSprite (WorldAnimatedSprite const& xWorldAnimatedSprite)
		: GameObject(xWorldAnimatedSprite),
		DrawableBase(xWorldAnimatedSprite),
		AnimatedSprite(xWorldAnimatedSprite),
		WorldDrawable(xWorldAnimatedSprite) {}
};

#endif // WORLDANIMATEDSPRITE_HPP
