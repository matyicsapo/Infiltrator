#ifndef GAMESTATEMACHINE_HPP
#define GAMESTATEMACHINE_HPP

#include "SFMLEventHandler.hpp"
#include "GameState.hpp"

class GameStateMachine : SFMLEventHandler {
private:
	GameState* currentState;

public:
	GameStateMachine (GameState* initialGameState) : currentState(initialGameState) {}

	~GameStateMachine () {
	    delete currentState; currentState = 0;
	}

	virtual void HandleEvents (std::list<sf::Event>& sfEvents) {
		if (currentState) {
			currentState->HandleEvents(sfEvents);
		}
	}

	void Update (float dT) {
		if (currentState) {
			currentState->Update(dT);
		}
	}

	void SetCurrentState (GameState* newGameState) { currentState = newGameState; }

	void ChangeState (GameState* newGameState) {
		if (!newGameState) return;

		if (currentState) {
			currentState->Exit();
			delete currentState;
		}

		currentState = newGameState;

		currentState->Enter();
	}
};

#endif // GAMESTATEMACHINE_HPP
