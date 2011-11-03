#ifndef SCREENSPACEDRAWABLE_HPP
#define SCREENSPACEDRAWABLE_HPP

#include "SFMLGameManager.hpp"
#include "DrawableBase.hpp"

class ScreenSpaceDrawable : virtual public DrawableBase {
public:
	enum AlignH {LEFT, CENTER, RIGHT};
	enum AlignV {TOP, MIDDLE, BOTTOM};

	ScreenSpaceDrawable (AlignH alignH, AlignV alignV, float layerDepth) : DrawableBase(layerDepth),
		mAlignH(alignH), mAlignV(alignV) {}

protected:
	AlignH mAlignH;
	AlignV mAlignV;

	virtual sf::Vector2f CalcFakePos () {
		sf::Vector2f fakePos;

		switch (mAlignH) {
			case LEFT:
				fakePos.x = GetScreenSize().x / 2 + basePos.x * windowFakeScale;
			break;
			case CENTER:
				fakePos.x = SFMLGameManager::Instance()->GetRenderWindow()->GetWidth() / 2 + basePos.x * windowFakeScale;
			break;
			case RIGHT:
				fakePos.x = SFMLGameManager::Instance()->GetRenderWindow()->GetWidth() - GetScreenSize().x / 2 + basePos.x * windowFakeScale;
			break;
		}

		switch (mAlignV) {
			case TOP:
				fakePos.y = GetScreenSize().y / 2 + basePos.y * windowFakeScale;
			break;
			case MIDDLE:
				fakePos.y = SFMLGameManager::Instance()->GetRenderWindow()->GetHeight() / 2 + basePos.y * windowFakeScale;
			break;
			case BOTTOM:
				fakePos.y = SFMLGameManager::Instance()->GetRenderWindow()->GetHeight() - GetScreenSize().y / 2 + basePos.y * windowFakeScale;
			break;
		}

		return fakePos;
	}
};

#endif // SCREENSPACEDRAWABLE_HPP
