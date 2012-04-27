#include "ScreenSpaceDrawable.hpp"

#include "../SFMLGameManager.hpp"

sf::Vector2f ScreenSpaceDrawable::CalcFakePos () {
	sf::Vector2f fakePos;

	switch (mAlignH) {
		case LEFT:
			fakePos.x = GetScreenSize().x / 2 + basePos.x * windowFakeScale;
		break;
		case CENTER:
			fakePos.x = Game->GetRenderWindow()->GetWidth() / 2 + basePos.x * windowFakeScale;
		break;
		case RIGHT:
			fakePos.x = Game->GetRenderWindow()->GetWidth() - GetScreenSize().x / 2 + basePos.x * windowFakeScale;
		break;
	}

	switch (mAlignV) {
		case TOP:
			fakePos.y = GetScreenSize().y / 2 + basePos.y * windowFakeScale;
		break;
		case MIDDLE:
			fakePos.y = Game->GetRenderWindow()->GetHeight() / 2 + basePos.y * windowFakeScale;
		break;
		case BOTTOM:
			fakePos.y = Game->GetRenderWindow()->GetHeight() - GetScreenSize().y / 2 + basePos.y * windowFakeScale;
		break;
	}

	return fakePos;
}

void ScreenSpaceDrawable::SetAlign (AlignH alignH, AlignV alignV) {
	mAlignH = alignH;
	mAlignV = alignV;
}
