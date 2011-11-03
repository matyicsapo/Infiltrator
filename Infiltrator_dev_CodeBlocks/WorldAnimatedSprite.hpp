#ifndef WORLDANIMATEDSPRITE_HPP
#define WORLDANIMATEDSPRITE_HPP

#include "Drawable.hpp"
#include "AnimatedSprite.hpp"

class WorldAnimatedSprite : public AnimatedSprite, public Drawable {
public:
	WorldAnimatedSprite (const SpriteAnimation* anim, float layerDepth = 0)
		: DrawableBase(layerDepth), AnimatedSprite(anim, layerDepth), Drawable(layerDepth) {}

    WorldAnimatedSprite (WorldAnimatedSprite const& xWorldAnimatedSprite)
		: DrawableBase(xWorldAnimatedSprite), AnimatedSprite(xWorldAnimatedSprite), Drawable(xWorldAnimatedSprite) {}
};

#endif // WORLDANIMATEDSPRITE_HPP
