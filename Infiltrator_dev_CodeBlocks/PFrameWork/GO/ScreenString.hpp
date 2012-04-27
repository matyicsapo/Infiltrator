#ifndef SCREENSTRING_HPP
#define SCREENSTRING_HPP

#include "ScreenSpaceDrawable.hpp"
#include "DrawableString.hpp"
#include "../PFWConstants.hpp"

class ScreenString : public DrawableString, public ScreenSpaceDrawable {
public:
	ScreenString (std::string text,
				std::string fontFile,
				float size,
				AlignH alignH = LEFT, AlignV alignV = TOP,
				int layerDepth = 0)
		: DrawableBase(layerDepth),
		DrawableString(text, fontFile, size, layerDepth),
		ScreenSpaceDrawable(alignH, alignV, layerDepth) {}

	ScreenString (std::string text, float size = PFWConstants::defaultFontSize, AlignH alignH = LEFT, AlignV alignV = TOP, int layerDepth = 0)
		: DrawableBase(layerDepth), DrawableString(text, size, layerDepth), ScreenSpaceDrawable(alignH, alignV, layerDepth) {}

	ScreenString (AlignH alignH = LEFT, AlignV alignV = TOP, int layerDepth = 0)
		: DrawableBase(layerDepth), DrawableString(layerDepth), ScreenSpaceDrawable(alignH, alignV, layerDepth) {}

    ScreenString (ScreenString const& xScreenString)
		: GameObject(xScreenString), DrawableBase(xScreenString), DrawableString(xScreenString), ScreenSpaceDrawable(xScreenString) {}

};

#endif // SCREENSTRING_HPP
