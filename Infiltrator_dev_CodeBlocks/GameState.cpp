#include "GameState.hpp"

#include "SFMLGameManager.hpp"

void GameState::HandleEvents (std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::Closed:
				SFMLGameManager::Instance()->GetRenderWindow()->Close();
			break;
			default:
			break;
		}

		itSfEvent++;
	}
}

