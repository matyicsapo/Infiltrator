#ifndef WORLDSHAPE_HPP
#define WORLDSHAPE_HPP

#include "WorldDrawable.hpp"
#include "Shape.hpp"

class WorldShape : public Shape, public WorldDrawable {
public:
	WorldShape (sf::Shape* s, int layerDepth)
		: DrawableBase(layerDepth), Shape(s, layerDepth), WorldDrawable(layerDepth) {}

	WorldShape (WorldShape const& xWorldShape)
		: DrawableBase(xWorldShape), Shape(xWorldShape), WorldDrawable(xWorldShape) {}
};

#endif // WORLDSHAPE_HPP
