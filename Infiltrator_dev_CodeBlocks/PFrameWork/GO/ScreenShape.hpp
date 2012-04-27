#ifndef SCREENSHAPE_HPP
#define SCREENSHAPE_HPP

#include "ScreenSpaceDrawable.hpp"
#include "Shape.hpp"

class ScreenShape : public Shape, public ScreenSpaceDrawable {
public:
	ScreenShape (sf::Shape* s, ScreenSpaceDrawable::AlignH alignH, ScreenSpaceDrawable::AlignV alignV, int layerDepth)
		: DrawableBase(layerDepth), Shape(s, layerDepth), ScreenSpaceDrawable(alignH, alignV, layerDepth) {}

	ScreenShape (ScreenShape const& xScreenShape)
		: GameObject(xScreenShape), DrawableBase(xScreenShape), Shape(xScreenShape), ScreenSpaceDrawable(xScreenShape) {}
};

#endif // SCREENSHAPE_HPP
