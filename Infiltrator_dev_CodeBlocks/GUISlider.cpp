#include "GUISlider.hpp"

#include "PFrameWork/SFMLGameManager.hpp"
//		#include "PFrameWork/State/StateMachine.hpp"

#include <cmath>

#define WHEELSCROLLSPD 1.0f

void GUISlider::HandleSFEvents (std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::MouseMoved: {
				sf::Rect<float> btnBounds(CalcFakePos().x - (GetScreenSize().x / 2),
											CalcFakePos().y - (GetScreenSize().y / 2 ),
											CalcFakePos().x + (GetScreenSize().x / 2),
											CalcFakePos().y + (GetScreenSize().y / 2 ) );

				hover = btnBounds.Contains(itSfEvent->MouseMove.X, itSfEvent->MouseMove.Y);

				if (dragging && itSfEvent->MouseMove.X != lastMousePressedX) {
					float originalX = sliderThingy->GetPosition().x;

					float mouseMotionX = itSfEvent->MouseMove.X - lastMousePressedX;

					sliderThingy->Move(sf::Vector2f(mouseMotionX / windowFakeScale, 0));

					// (width - (edgeRight->GetScreenSize().x / windowFakeScale / baseScale.x) * 2)
					// this is the width of the full bar without the edges
					float w = width - (edgeRight->GetScreenSize().x / windowFakeScale / baseScale.x) * 2;

					// zero position for the sliderThingy
					float zeroX = 0;

					switch (mAlignH) {
						case LEFT:
							zeroX = -sliderThingy->GetScreenSize().x / 2 / windowFakeScale / baseScale.x
								+ edgeLeft->GetScreenSize().x / windowFakeScale / baseScale.x
								+ basePos.x;
						break;
						case CENTER:
							zeroX = -w / 2 + basePos.x;
						break;
						case RIGHT:
							zeroX = sliderThingy->GetScreenSize().x / 2 / windowFakeScale / baseScale.x
								- edgeLeft->GetScreenSize().x / windowFakeScale / baseScale.x
								- w
								+ basePos.x;
						break;
					}

					if (sliderThingy->GetPosition().x < zeroX) {
						sliderThingy->SetPosition(sf::Vector2f(zeroX, sliderThingy->GetPosition().y));

						value = 0;
					}
					else
					if (sliderThingy->GetPosition().x > zeroX + w) {
						sliderThingy->SetPosition(sf::Vector2f(zeroX + w, sliderThingy->GetPosition().y));

						value = 1;
					}
					else {
						value = (sliderThingy->GetPosition().x
									- (GetPosition().x - width / 2))
										/ width;
					}

					float mul = (sliderThingy->GetPosition().x - originalX) / (mouseMotionX / windowFakeScale);
					lastMousePressedX += (itSfEvent->MouseMove.X - lastMousePressedX) * mul;

					if (onChanged) onChanged(this->value);
				}
			} break;
			case sf::Event::MouseWheelMoved:
				if (hover) {
					value += Game->GetDeltaTime() * WHEELSCROLLSPD * itSfEvent->MouseWheel.Delta;

					SetValue(value);
				}
			break;
			case sf::Event::MouseButtonPressed: {
				sf::Vector2f wMouse = Game->ConvertCoordsWindowToWorld(itSfEvent->MouseButton.X,
																		itSfEvent->MouseButton.Y);

				if (sliderThingy->Contains(wMouse)) {
					if (sliderThingy->IsOpaque(wMouse)) {
						dragging = true;

						lastMousePressedX = itSfEvent->MouseButton.X;
					}
				}
				else if (hover) {
					value = (itSfEvent->MouseButton.X - (CalcFakePos().x - GetScreenSize().x / 2))
						/ GetScreenSize().x;

					SetValue(value);

					dragging = true;

					lastMousePressedX = itSfEvent->MouseButton.X;
				}
			} break;
			case sf::Event::MouseButtonReleased:
				dragging = false;

				if (onChanged) onChanged(this->value);
			break;
			default:
			break;
		}

		itSfEvent++;
	}
}

// artifacts may occure at the seams so we multiply by a small amount
#define MUL_STARTSCREEN_X 1.00f
#define MUL_ENDSCREEN_X 1.005f

void GUISlider::Draw (sf::RenderWindow& rwin) {
	float scaledWidth = width * baseScale.x;

	float startScreenX = CalcFakePos().x
		- scaledWidth / 2 * windowFakeScale
		+ edgeLeft->GetScreenSize().x;
	startScreenX *= MUL_STARTSCREEN_X;

	float endScreenX = CalcFakePos().x
		+ scaledWidth * windowFakeScale / 2
		- edgeLeft->GetScreenSize().x;
	endScreenX *= MUL_ENDSCREEN_X;

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
}

void GUISlider::SetValue (float value) {
	if (value < 0)
		this->value = 0;
	else if (value > 1)
		this->value = 1;
	else
		this->value = value;

	// (width - (edgeRight->GetScreenSize().x / windowFakeScale / baseScale.x) * 2)
	// this is the width of the full bar without the edges
	float w = width - (edgeRight->GetScreenSize().x / windowFakeScale / baseScale.x) * 2;

	// zero position for the sliderThingy
	float zeroX = 0;

	switch (mAlignH) {
		case LEFT:
			zeroX = -sliderThingy->GetScreenSize().x / 2 / windowFakeScale / baseScale.x
				+ edgeLeft->GetScreenSize().x / windowFakeScale / baseScale.x
				+ basePos.x;

			sliderThingy->SetPosition(sf::Vector2f(zeroX + w * this->value, sliderThingy->GetPosition().y));
		break;
		case CENTER:
			zeroX = -w / 2 + basePos.x;

			sliderThingy->SetPosition(sf::Vector2f(zeroX + w * this->value, sliderThingy->GetPosition().y));
		break;
		case RIGHT:
			zeroX = sliderThingy->GetScreenSize().x / 2 / windowFakeScale / baseScale.x
				- edgeLeft->GetScreenSize().x / windowFakeScale / baseScale.x
				- w
				+ basePos.x;

			sliderThingy->SetPosition(sf::Vector2f(zeroX + w * this->value, sliderThingy->GetPosition().y));
		break;
	}

	if (onChanged) onChanged(this->value);
}

void GUISlider::SetPosition (sf::Vector2f position) {
	Sprite::SetPosition(position);

	sliderThingy->SetPosition(position);
	SetValue(value);

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

	switch (mAlignV) {
		case TOP:
			sliderThingy->SetPosition(sf::Vector2f(sliderThingy->GetPosition().x,
				basePos.y - sliderThingy->GetScreenSize().y / 2 / windowFakeScale + GetScreenSize().y / 2 / windowFakeScale
			));
		break;
		case MIDDLE:
		break;
		case BOTTOM:
			sliderThingy->SetPosition(sf::Vector2f(sliderThingy->GetPosition().x,
				basePos.y + sliderThingy->GetScreenSize().y / 2 / windowFakeScale - GetScreenSize().y / 2 / windowFakeScale
			));
		break;
	}
}
