#ifndef GROUNDSPRITE_HPP
#define GROUNDSPRITE_HPP

#include "PFrameWork/GO/WorldSprite.hpp"

class GroundSprite : public WorldSprite {
public:
	GroundSprite (std::string textureFile, int layerDepth = 0)
		: DrawableBase(layerDepth), WorldSprite(textureFile, layerDepth) {}

	virtual void Draw (sf::RenderWindow& rwin);
};

#endif // GROUNDSPRITE_HPP
