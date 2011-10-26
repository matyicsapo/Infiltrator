#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "DrawableBase.hpp"

class Drawable : virtual public DrawableBase {
protected:
	virtual sf::Vector2f CalcFakePos () { return basePos * windowFakeScale; }

public:
	Drawable (float layerDepth) : DrawableBase(layerDepth) {}
};

#endif // DRAWABLE_HPP
