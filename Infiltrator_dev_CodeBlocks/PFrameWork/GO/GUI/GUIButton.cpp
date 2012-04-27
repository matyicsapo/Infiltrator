#include "GUIButton.hpp"

void GUIButton::ReleaseResources () {
	Sprite::ReleaseResources();
	DrawableString::ReleaseResources();
}

void GUIButton::HandleSFEvents (std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {

		switch (itSfEvent->Type) {
			// brackets required when declaring variables inside a 'case' block
			case sf::Event::MouseMoved: {
				sf::Vector2f screenSize(GetScreenSize());
							// could meddle with using string's size too, but buttonreisizing to text would be better
				sf::Rect<float> btnBounds(CalcFakePos().x - (screenSize.x / 2),
											CalcFakePos().y - (screenSize.y / 2 ),
											CalcFakePos().x + (screenSize.x / 2),
											CalcFakePos().y + (screenSize.y / 2 ) );

				if (btnBounds.Contains(itSfEvent->MouseMove.X, itSfEvent->MouseMove.Y)) {
					if (!hover) {
						// if got in/on
						sfString->SetColor(sf::Color::White);
						hover = true;
						SetImage(txInactiveHover);
					}
				}
				else if (hover) {
					// if got out/off
					sfString->SetColor(sf::Color(128, 128, 128));
					active = false;
					hover = false;
					SetImage(txInactiveNormal);
				}
			} break;
			case sf::Event::MouseButtonPressed:
				if (hover) {
					active = true;

					SetImage(txActiveHover);
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
					SetImage(hover ? txInactiveHover : txInactiveNormal);
				}
			break;
			default:
			break;
		}

		itSfEvent++;
	}
}
