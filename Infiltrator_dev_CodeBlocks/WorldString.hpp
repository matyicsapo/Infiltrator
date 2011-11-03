#ifndef WORLDSTRING_HPP
#define WORLDSTRING_HPP

#include "Drawable.hpp"
#include "DrawableString.hpp"

class WorldString : public DrawableString, public Drawable {
public:
	WorldString (std::string text, std::string fontFile, float size, float layerDepth = 0)
		: DrawableBase(layerDepth), DrawableString(text, fontFile, size, layerDepth), Drawable(layerDepth) {}

	WorldString (std::string text, float size, float layerDepth = 0)
		: DrawableBase(layerDepth), DrawableString(text, size, layerDepth), Drawable(layerDepth) {}

	WorldString (float layerDepth = 0)
		: DrawableBase(layerDepth), DrawableString(layerDepth), Drawable(layerDepth) {}

    WorldString (WorldString const& xWorldString)
		: DrawableBase(xWorldString), DrawableString(xWorldString), Drawable(xWorldString) {}
};

#endif // WORLDSTRING_HPP
