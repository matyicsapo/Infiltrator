#include "GUIToggleButton.hpp"

void GUIToggleButton::HandleEvents (std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {

		switch (itSfEvent->Type) {
			case sf::Event::MouseMoved: {
				sf::Vector2f screenSize(GetScreenSize());
				sf::Rect<float> btnBounds(CalcFakePos().x - (screenSize.x / 2),
											CalcFakePos().y - (screenSize.y / 2 ),
											CalcFakePos().x + (screenSize.x / 2),
											CalcFakePos().y + (screenSize.y / 2 ) );

				if (btnBounds.Contains(itSfEvent->MouseMove.X, itSfEvent->MouseMove.Y)) {
					if (!hover) {
						hover = true;
						SetImage(active ? txActiveHover : txInactiveHover);
					}
				}
				else if (hover) {
					hover = false;
					SetImage(active ? txActiveNormal : txInactiveNormal);
				}
			} break;
			case sf::Event::MouseButtonPressed:
				if (hover) {
					active = !active;

					if (active) {
						SetImage(txActiveHover);
						if (onActivate)
							onActivate();
					}
					else {
						SetImage(txInactiveHover);
						if (onDeactivate)
							onDeactivate();
					}

					itSfEvent =	--sfEvents.erase(itSfEvent);
				}
			break;
			default:
				// don't care about no other event types
			break;
		}

		itSfEvent++;
	}
}

