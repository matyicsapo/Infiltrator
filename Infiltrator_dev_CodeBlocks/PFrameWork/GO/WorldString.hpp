#ifndef WORLDSTRING_HPP
#define WORLDSTRING_HPP

#include "WorldDrawable.hpp"
#include "DrawableString.hpp"

class WorldString : public DrawableString, public WorldDrawable {
public:
	WorldString (std::string text, std::string fontFile, float size, int layerDepth = 0)
		: DrawableBase(layerDepth), DrawableString(text, fontFile, size, layerDepth), WorldDrawable(layerDepth) {}

	WorldString (std::string text, float size, int layerDepth = 0)
		: DrawableBase(layerDepth), DrawableString(text, size, layerDepth), WorldDrawable(layerDepth) {}

	WorldString (int layerDepth = 0)
		: DrawableBase(layerDepth), DrawableString(layerDepth), WorldDrawable(layerDepth) {}

    WorldString (WorldString const& xWorldString)
		: DrawableBase(xWorldString), DrawableString(xWorldString), WorldDrawable(xWorldString) {}
};

#endif // WORLDSTRING_HPP
