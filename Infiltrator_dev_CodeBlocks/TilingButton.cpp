#include "TilingButton.hpp"

#include "PFrameWork/SFMLGameManager.hpp"
//#include <iostream>

void TilingButton::ReleaseResources () {
	Sprite::ReleaseResources();
	DrawableString::ReleaseResources();
}

void TilingButton::SetActive (bool active) {
	this->active = active;
	this->hover = hover;

	if (active) {
		sfString->SetColor(sf::Color::White);

		SetImage(txActiveHover[1]);
		edgeLeft->SetImage(txActiveHover[0]);
		edgeRight->SetImage(txActiveHover[2]);

		onActivate();
	}
	else {
		sfString->SetColor(sf::Color(128, 128, 128));

		SetImage(txInactiveNormal[1]);
		edgeLeft->SetImage(txInactiveNormal[0]);
		edgeRight->SetImage(txInactiveNormal[2]);
	}
}

void TilingButton::HandleSFEvents (std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {

		switch (itSfEvent->Type) {
			case sf::Event::MouseMoved: {
				sf::Rect<float> btnBounds(CalcFakePos().x - (GetScreenSize().x / 2),
											CalcFakePos().y - (GetScreenSize().y / 2 ),
											CalcFakePos().x + (GetScreenSize().x / 2),
											CalcFakePos().y + (GetScreenSize().y / 2 ) );

				if (btnBounds.Contains(itSfEvent->MouseMove.X, itSfEvent->MouseMove.Y)) {
					if (!hover) {
						// if got in/on
						sfString->SetColor(sf::Color::White);
						hover = true;
						SetImage(txInactiveHover[1]);
						edgeLeft->SetImage(txInactiveHover[0]);
						edgeRight->SetImage(txInactiveHover[2]);
					}
				}
				else if (hover) {
					// if got out/off
					sfString->SetColor(sf::Color(128, 128, 128));
					active = false;
					hover = false;
					SetImage(txInactiveNormal[1]);
					edgeLeft->SetImage(txInactiveNormal[0]);
					edgeRight->SetImage(txInactiveNormal[2]);
				}
			} break;
			case sf::Event::MouseButtonPressed:
				if (hover) {
					active = true;

					SetImage(txActiveHover[1]);
					edgeLeft->SetImage(txActiveHover[0]);
					edgeRight->SetImage(txActiveHover[2]);
					onActivate();

					// EATing the event
					// erasing this element returning the iterator to the next or 'end' if this was the last
					// dec by 1 'cause incrementing at end
					itSfEvent =	--sfEvents.erase(itSfEvent);
				}
			break;
			case sf::Event::MouseButtonReleased:
				if (active) {
					active = false;
					if (hover) {
						SetImage(txInactiveHover[1]);
						edgeLeft->SetImage(txInactiveHover[0]);
						edgeRight->SetImage(txInactiveHover[2]);
					}
					else {
						SetImage(txInactiveNormal[1]);
						edgeLeft->SetImage(txInactiveNormal[0]);
						edgeRight->SetImage(txInactiveNormal[2]);
					}
				}
			break;
			default:
			break;
		}

		itSfEvent++;
	}
}

// artifacts may occure at the seams so we multiply by a small amount
#define MUL_STARTSCREEN_X_LEFT 0.95f
#define MUL_ENDSCREEN_X_LEFT 1.01f

#define MUL_STARTSCREEN_X_CENTER 0.995f
#define MUL_ENDSCREEN_X_CENTER 1.005f

#define MUL_STARTSCREEN_X_RIGHT 0.995f
#define MUL_ENDSCREEN_X_RIGHT 1.0f

void TilingButton::Draw (sf::RenderWindow& rwin) {
	float scaledWidth = width * baseScale.x;

	float startScreenX = CalcFakePos().x
		- scaledWidth / 2 * windowFakeScale
		+ edgeLeft->GetScreenSize().x;

	float endScreenX = CalcFakePos().x
		+ scaledWidth * windowFakeScale / 2
		- edgeLeft->GetScreenSize().x;

	switch (mAlignH) {
		case LEFT:
			startScreenX *= MUL_STARTSCREEN_X_LEFT;
			endScreenX *= MUL_ENDSCREEN_X_LEFT;
		break;
		case CENTER:
			startScreenX *= MUL_STARTSCREEN_X_CENTER;
			endScreenX *= MUL_ENDSCREEN_X_CENTER;
		break;
		case RIGHT:
			startScreenX *= MUL_STARTSCREEN_X_RIGHT;
			endScreenX *= MUL_ENDSCREEN_X_RIGHT;
		break;
	}

	float middleSegmentScreenWidth = sfSprite->GetImage()->GetWidth() * sfSprite->GetScale().x;

	float xOffset = middleSegmentScreenWidth / 2;
	do {
		sfSprite->SetPosition(sf::Vector2f(startScreenX + xOffset, CalcFakePos().y));

		rwin.Draw(*sfSprite);

		xOffset += middleSegmentScreenWidth;
	}
	while ( (startScreenX + xOffset + middleSegmentScreenWidth / 2) < endScreenX);

	float remainderScreen = endScreenX - (startScreenX + xOffset - middleSegmentScreenWidth / 2);

	if (remainderScreen > 0) {
		sfSprite->SetSubRect(sf::IntRect(0, 0,
			(remainderScreen / sfSprite->GetScale().x),
			sfSprite->GetImage()->GetHeight()));

		sfSprite->SetCenter(remainderScreen / sfSprite->GetScale().x / 2.0f,
			sfSprite->GetImage()->GetHeight() / 2);

		xOffset -= middleSegmentScreenWidth; // revert
		xOffset += middleSegmentScreenWidth / 2 + remainderScreen / 2;

		sfSprite->SetPosition(sf::Vector2f(startScreenX + xOffset, CalcFakePos().y));

		rwin.Draw(*sfSprite);

		sfSprite->SetSubRect(sf::IntRect(0, 0,
			sfSprite->GetImage()->GetWidth(), sfSprite->GetImage()->GetHeight()));
		sfSprite->SetCenter(sfSprite->GetImage()->GetWidth() / 2,
							sfSprite->GetImage()->GetHeight() / 2);
	}

	rwin.Draw(*sfString);
}

void TilingButton::SetPosition (sf::Vector2f position) {
	Sprite::SetPosition(position);
//	DrawableString::SetPosition(position + textOffset);

	float scaledWidth = width * baseScale.x;

	switch (mAlignH) {
		case LEFT:
			edgeLeft->SetPosition(
				sf::Vector2f(
				basePos.x
				, position.y
				)
			);

			edgeRight->SetPosition(
				sf::Vector2f(
				(basePos.x + scaledWidth - edgeRight->GetScreenSize().x / windowFakeScale)
				, position.y
				)
			);

		break;
		case CENTER:
			edgeLeft->SetPosition(
				sf::Vector2f(
				(basePos.x - scaledWidth / 2 + edgeLeft->GetScreenSize().x / 2 / windowFakeScale)
				, position.y
				)
			);

			edgeRight->SetPosition(
				sf::Vector2f(
				(basePos.x + scaledWidth / 2 - edgeRight->GetScreenSize().x / 2 / windowFakeScale)
				, position.y
				)
			);

		break;
		case RIGHT:
			edgeLeft->SetPosition(
				sf::Vector2f(
				-(-basePos.x + scaledWidth - edgeRight->GetScreenSize().x / windowFakeScale)
				, position.y
				)
			);

			edgeRight->SetPosition(
				sf::Vector2f(
				basePos.x
				, position.y
				)
			);
		break;
	}

//	switch (mAlignV) {
//		case TOP:
//			std::cout << edgeLeft->GetPosition().x << " " << edgeLeft->GetPosition().y << std::endl;
//		break;
//		default:
//		break;
//	}
}

void TilingButton::Move (sf::Vector2f offset) {
	Sprite::Move(offset);
//	DrawableString::Move(offset);

	edgeLeft->Move(offset);
	edgeRight->Move(offset);
}

void TilingButton::SetText (std::string text) {
	DrawableString::SetText(text);

	width = (sfString->GetRect().Right - sfString->GetRect().Left) / windowFakeScale;
	width += (Resources->GetImage(txInactiveNormal[0])->GetWidth() * 2);

	if (minWidth > 0) {
		if (width < minWidth) {
			width = minWidth;
		}
	}

	SetPosition(GetPosition());
}
