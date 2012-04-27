#ifndef WORLDSPRITE_HPP
#define WORLDSPRITE_HPP

#include "WorldDrawable.hpp"
#include "Sprite.hpp"

class WorldSprite : public Sprite, public WorldDrawable {
public:
	WorldSprite (std::string textureFile, int layerDepth = 0, unsigned int entityType = 0)
		: GameObject(entityType), DrawableBase(layerDepth, entityType), Sprite(textureFile, layerDepth, entityType), WorldDrawable(layerDepth, entityType) {}

	WorldSprite (int layerDepth = 0, unsigned int entityType = 0)
		: GameObject(entityType), DrawableBase(layerDepth, entityType), Sprite(layerDepth, entityType), WorldDrawable(layerDepth, entityType) {}

    WorldSprite (WorldSprite const& xWorldSprite)
		: GameObject(xWorldSprite), DrawableBase(xWorldSprite), Sprite(xWorldSprite), WorldDrawable(xWorldSprite) {}
};

#endif // WORLDSPRITE_HPP
