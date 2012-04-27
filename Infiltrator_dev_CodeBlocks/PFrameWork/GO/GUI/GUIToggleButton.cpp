#include "GUIToggleButton.hpp"

void GUIToggleButton::HandleSFEvents (std::list<sf::Event>& sfEvents) {
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
					SetActive(!active, true, hover);

					itSfEvent =	--sfEvents.erase(itSfEvent);
				}
			break;
			default:
			break;
		}

		itSfEvent++;
	}
}

void GUIToggleButton::SetActive (bool activated, bool callevent, bool hovertx) {
	active = activated;

	if (activated) {
		if (onActivate && callevent) {
			onActivate();
		}

		SetImage(hovertx ? txActiveHover : txActiveNormal);
	}
	else {
		if (onDeactivate && callevent) {
			onDeactivate();
		}

		SetImage(hovertx ? txInactiveHover : txInactiveHover);
	}
}

