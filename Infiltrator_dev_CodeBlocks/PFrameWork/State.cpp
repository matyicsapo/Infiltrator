#include "State.hpp"

#include "StateMachine.hpp"
#include "SFMLGameManager.hpp"

void State::Init () {
	off = false;
}

void GameState::HandleSFEvents (std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::Closed:
				Game->GetRenderWindow()->Close();
			break;
			default:
			break;
		}

		itSfEvent++;
	}
}
