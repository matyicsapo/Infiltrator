#include "TilingScreenSprite.hpp"

// artifacts may occure at the seams so we multiply by a small amount
#define MUL_STARTSCREEN_X .95f
#define MUL_ENDSCREEN_X 1.05f

void TilingScreenSprite::Draw (sf::RenderWindow& rwin) {
	if (horizontal) {
		float startScreenX = edge01->GetScreenSize().x * MUL_STARTSCREEN_X;
		float middleSegmentScreenWidth = sfSprite->GetImage()->GetWidth() * sfSprite->GetScale().x;

		float endScreenX = (Game->GetRenderWindow()->GetWidth()
			- edge02->GetScreenSize().x) * MUL_ENDSCREEN_X;

		float xOffset = middleSegmentScreenWidth / 2;
		do {
			sfSprite->SetPosition(sf::Vector2f(startScreenX + xOffset, CalcFakePos().y));

			rwin.Draw(*sfSprite);

			xOffset += middleSegmentScreenWidth;
		}
		while ( (startScreenX + xOffset + middleSegmentScreenWidth / 2) < endScreenX);

		float remainder = endScreenX
			- (startScreenX + xOffset - middleSegmentScreenWidth / 2);

		if (remainder > 0) {
			sfSprite->SetSubRect(sf::IntRect(0, 0,
				(remainder / sfSprite->GetScale().x ),
				sfSprite->GetImage()->GetHeight()));

			sfSprite->SetCenter(remainder / sfSprite->GetScale().x / 2.0f,
								sfSprite->GetImage()->GetHeight() / 2);

			xOffset -= middleSegmentScreenWidth; // revert
			xOffset += middleSegmentScreenWidth / 2 + remainder / 2;

			sfSprite->SetPosition(sf::Vector2f(startScreenX + xOffset, CalcFakePos().y));

			rwin.Draw(*sfSprite);

			sfSprite->SetSubRect(sf::IntRect(0, 0,
				sfSprite->GetImage()->GetWidth(), sfSprite->GetImage()->GetHeight()));
			sfSprite->SetCenter(sfSprite->GetImage()->GetWidth() / 2,
								sfSprite->GetImage()->GetHeight() / 2);
		}
	}
	else {
		float startScreenY = edge01->GetScreenSize().y;
		float middleSegmentScreenHeight = sfSprite->GetImage()->GetHeight() * sfSprite->GetScale().y;

		float endScreenY = (Game->GetRenderWindow()->GetHeight()
			- edge02->GetScreenSize().y) * 2.0f;

		float yOffset = middleSegmentScreenHeight / 2;
		do {
			sfSprite->SetPosition(sf::Vector2f(CalcFakePos().x, startScreenY + yOffset));

			rwin.Draw(*sfSprite);

			yOffset += middleSegmentScreenHeight;
		}
		while ( (startScreenY + yOffset + middleSegmentScreenHeight / 2) < endScreenY);

		float remainder = endScreenY
			- (startScreenY + yOffset - middleSegmentScreenHeight / 2);

		if (remainder > 0) {
			sfSprite->SetSubRect(sf::IntRect(0, 0,
				sfSprite->GetImage()->GetWidth(),
				(remainder / sfSprite->GetScale().y)));

			sfSprite->SetCenter(sfSprite->GetImage()->GetWidth() / 2,
								remainder / sfSprite->GetScale().y / 2.0f);

			yOffset -= middleSegmentScreenHeight; // revert
			yOffset += middleSegmentScreenHeight / 2 + remainder / 2;

			sfSprite->SetPosition(sf::Vector2f(CalcFakePos().x, startScreenY + yOffset));

			rwin.Draw(*sfSprite);

			sfSprite->SetSubRect(sf::IntRect(0, 0,
				sfSprite->GetImage()->GetWidth(), sfSprite->GetImage()->GetHeight()));
			sfSprite->SetCenter(sfSprite->GetImage()->GetWidth() / 2,
								sfSprite->GetImage()->GetHeight() / 2);
		}
	}
}

sf::Vector2f TilingScreenSprite::CalcFakePos () {
	sf::Vector2f fakePos;

	switch (mAlignH) {
		case LEFT:
			fakePos.x = GetScreenSize().x / 2 + basePos.x * windowFakeScale
				;//+ sfSprite->GetImage()->GetWidth() * sfSprite->GetScale().x / 2;
		break;
		case CENTER:
			fakePos.x = Game->GetRenderWindow()->GetWidth() / 2 + basePos.x * windowFakeScale;
		break;
		case RIGHT:
			fakePos.x = Game->GetRenderWindow()->GetWidth()
				- GetScreenSize().x / 2 + basePos.x * windowFakeScale
				;//- sfSprite->GetImage()->GetWidth() * sfSprite->GetScale().x / 2;
		break;
	}

	switch (mAlignV) {
		case TOP:
			fakePos.y = GetScreenSize().y / 2 + basePos.y * windowFakeScale
				;//+ sfSprite->GetImage()->GetHeight() * sfSprite->GetScale().y / 2;
		break;
		case MIDDLE:
			fakePos.y = Game->GetRenderWindow()->GetHeight() / 2 + basePos.y * windowFakeScale;
		break;
		case BOTTOM:
			fakePos.y = Game->GetRenderWindow()->GetHeight()
				- GetScreenSize().y / 2 + basePos.y * windowFakeScale
				;//- sfSprite->GetImage()->GetHeight() * sfSprite->GetScale().y / 2;
		break;
	}

	return fakePos;
}
