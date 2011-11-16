#include "GameStateMachine.hpp"

void GameStateMachine::ChangeState (GameState* newGameState) {
	if (!newGameState) return;

	if (currentState) {
		currentState->Exit();
		delete currentState;
	}

	currentState = newGameState;

	currentState->Enter();
}
