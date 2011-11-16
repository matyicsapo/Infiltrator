#ifndef WORLDSPRITE_HPP
#define WORLDSPRITE_HPP

#include "WorldDrawable.hpp"
#include "Sprite.hpp"

class WorldSprite : public Sprite, public WorldDrawable {
public:
	WorldSprite (std::string textureFile, int layerDepth = 0)
		: DrawableBase(layerDepth), Sprite(textureFile, layerDepth), WorldDrawable(layerDepth) {}

	WorldSprite (int layerDepth = 0)
		: DrawableBase(layerDepth), Sprite(layerDepth), WorldDrawable(layerDepth) {}

    WorldSprite (WorldSprite const& xWorldSprite)
		: DrawableBase(xWorldSprite), Sprite(xWorldSprite), WorldDrawable(xWorldSprite) {}
};

#endif // WORLDSPRITE_HPP
