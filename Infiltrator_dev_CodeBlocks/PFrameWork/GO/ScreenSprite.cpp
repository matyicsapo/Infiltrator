#include "ScreenSprite.hpp"

#include "../SFMLGameManager.hpp"

#include <cmath>

bool ScreenSprite::Contains (sf::Vector2f globalPosition) {
//	sf::Vector2f size(sfSprite->GetImage()->GetWidth() * GetScale().x,
//						sfSprite->GetImage()->GetHeight() * GetScale().y);
//
//	bool horizontal;
//	bool vertical;
//
//	switch (mAlignH) {
//		case LEFT:
//			horizontal = std::abs(globalPosition.x - basePos.x)
//				<= size.x / 2;
//		break;
//		case CENTER:
//			horizontal = std::abs(globalPosition.x - (Game->GetRenderWindow()->GetWidth() / 2 + basePos.x))
//				<= size.x / 2;
//		break;
//		case RIGHT:
//			horizontal = std::abs(globalPosition.x - (Game->GetRenderWindow()->GetWidth() + basePos.x - size.x / 2))
//				<= size.x / 2;
//		break;
//	}
//
//	switch (mAlignV) {
//		case TOP:
//			vertical = std::abs(globalPosition.y - basePos.y - size.y / 2)
//				<= size.y / 2;
//		break;
//		case MIDDLE:
//			vertical = std::abs(globalPosition.y - (Game->GetRenderWindow()->GetHeight() / 2 + basePos.y))
//				<= size.y / 2;
//		break;
//		case BOTTOM:
//			vertical = std::abs(globalPosition.y - (Game->GetRenderWindow()->GetHeight() + basePos.y))
//				<= size.y / 2;
//		break;
//	}
//
//	return horizontal && vertical;


	sf::Vector2f screenSize(GetScreenSize());
	sf::Vector2f fakePos(CalcFakePos());
	sf::Rect<float> bounds(fakePos.x - (screenSize.x / 2),
							fakePos.y - (screenSize.y / 2 ),
							fakePos.x + (screenSize.x / 2),
							fakePos.y + (screenSize.y / 2 ));

	return bounds.Contains(globalPosition.x, globalPosition.y);
}
