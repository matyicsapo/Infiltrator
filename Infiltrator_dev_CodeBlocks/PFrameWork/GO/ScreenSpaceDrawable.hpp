#ifndef SCREENSPACEDRAWABLE_HPP
#define SCREENSPACEDRAWABLE_HPP

#include "DrawableBase.hpp"
#include "../DrawManager.hpp"

class ScreenSpaceDrawable : virtual public DrawableBase {
public:
	enum AlignH {LEFT, CENTER, RIGHT};
	enum AlignV {TOP, MIDDLE, BOTTOM};

protected:
	AlignH mAlignH;
	AlignV mAlignV;

	virtual sf::Vector2f CalcFakePos ();

public:
	ScreenSpaceDrawable (AlignH alignH, AlignV alignV, int layerDepth, unsigned int entityType = 0) : DrawableBase(layerDepth, entityType),
		mAlignH(alignH), mAlignV(alignV) {
//			Drawables->AddScreenSpace(this);
	}
	ScreenSpaceDrawable (ScreenSpaceDrawable const& xScreenSpaceDrawable) : DrawableBase(xScreenSpaceDrawable) {
		mAlignH = xScreenSpaceDrawable.mAlignH;
		mAlignV = xScreenSpaceDrawable.mAlignV;

//		Drawables->AddScreenSpace(this);
	}

	virtual ~ScreenSpaceDrawable () {
//		Drawables->PopScreenSpace(this);
	}

	void SetAlign (AlignH alignH, AlignV alignV);
};

#endif // SCREENSPACEDRAWABLE_HPP
