#ifndef GAMESTATEMACHINE_HPP
#define GAMESTATEMACHINE_HPP

#include "../SFMLEventHandler.hpp"
#include "GameState.hpp"

class GameStateMachine : SFMLEventHandler {
private:
	GameState* currentState;

public:
	GameStateMachine (GameState* initialGameState) : currentState(initialGameState) {}

	~GameStateMachine () {
	    delete currentState; currentState = 0;
	}

	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents) {
		if (currentState) {
			currentState->HandleSFEvents(sfEvents);
		}
	}

	void Update (float dT) {
		if (currentState) {
			currentState->Update(dT);
		}
	}

	void SetCurrentState (GameState* newGameState) { currentState = newGameState; }

	void ChangeState (GameState* newGameState);
};

#endif // GAMESTATEMACHINE_HPP
