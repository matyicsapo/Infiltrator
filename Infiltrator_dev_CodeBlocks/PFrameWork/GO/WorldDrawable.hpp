#ifndef WORLDDRAWABLE_HPP
#define WORLDDRAWABLE_HPP

#include "DrawableBase.hpp"
#include "../DrawManager.hpp"

class WorldDrawable : virtual public DrawableBase {
protected:
	virtual sf::Vector2f CalcFakePos () { return basePos * windowFakeScale; }

public:
	WorldDrawable (int layerDepth, unsigned int entityType = 0) : DrawableBase(layerDepth, entityType) {
//		Drawables->AddWorldSpace(this);
	}
	WorldDrawable (WorldDrawable const& xWorldDrawable) : DrawableBase(xWorldDrawable) {
//			Drawables->AddWorldSpace(this);
	}

	virtual ~WorldDrawable () {
//		Drawables->PopWorldSpace(this);
	}
};

#endif // WORLDDRAWABLE_HPP
