#ifndef GAMESTATEMACHINE_HPP
#define GAMESTATEMACHINE_HPP

#include "GameState.hpp"

class GameStateMachine {
	GameState* pCurrentState;

public:
	GameStateMachine () : pCurrentState(0) {}
	GameStateMachine (GameState* pGameState) : pCurrentState(pGameState) {}

	~GameStateMachine () {
	    delete pCurrentState; pCurrentState = 0;
	}

	void Update () {
		if (pCurrentState) pCurrentState->Execute(this);
	}

	void SetCurrentState (GameState* pNewGameState) { pCurrentState = pNewGameState; }

	void ChangeState (GameState* pNewGameState) {
		if (!pNewGameState) return;

		//call the exit method of the existing state
		if (pCurrentState) {
			pCurrentState->Exit(this);
			delete pCurrentState;
		}

		//change state to the new state
		pCurrentState = pNewGameState;

		//call the entry method of the new state
		pCurrentState->Enter(this);
	}
};

#endif
