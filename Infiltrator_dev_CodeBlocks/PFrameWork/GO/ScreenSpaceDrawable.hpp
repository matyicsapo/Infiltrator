#ifndef SCREENSPACEDRAWABLE_HPP
#define SCREENSPACEDRAWABLE_HPP

#include "DrawableBase.hpp"
#include "../DrawManager.hpp"

class ScreenSpaceDrawable : virtual public DrawableBase {
public:
	enum AlignH {LEFT, CENTER, RIGHT};
	enum AlignV {TOP, MIDDLE, BOTTOM};

	ScreenSpaceDrawable (AlignH alignH, AlignV alignV, int layerDepth) : DrawableBase(layerDepth),
		mAlignH(alignH), mAlignV(alignV) { Drawables->AddScreenSpace(this); }

	virtual ~ScreenSpaceDrawable () { Drawables->PopScreenSpace(this); }

protected:
	AlignH mAlignH;
	AlignV mAlignV;

	virtual sf::Vector2f CalcFakePos ();
};

#endif // SCREENSPACEDRAWABLE_HPP
