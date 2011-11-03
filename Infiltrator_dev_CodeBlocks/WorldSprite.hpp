#ifndef WORLDSPRITE_HPP
#define WORLDSPRITE_HPP

#include "Drawable.hpp"
#include "Sprite.hpp"

class WorldSprite : public Sprite, public Drawable {
public:
	WorldSprite (std::string textureFile, float layerDepth = 0)
		: DrawableBase(layerDepth), Sprite(textureFile, layerDepth), Drawable(layerDepth) {}

	WorldSprite (float layerDepth = 0)
		: DrawableBase(layerDepth), Sprite(layerDepth), Drawable(layerDepth) {}

    WorldSprite (WorldSprite const& xWorldSprite)
		: DrawableBase(xWorldSprite), Sprite(xWorldSprite), Drawable(xWorldSprite) {}
};

#endif // WORLDSPRITE_HPP
