#include "PFrameWork/StateMachine.hpp"
#include "PFrameWork/SFMLGameManager.hpp"

#include "GameStates/GameState_Logos.hpp"

int main () {
	GameState* xInitialGameState = new GameState_Logos();

	Game->SetGameStateMachine(new StateMachine<GameState>(xInitialGameState));

	xInitialGameState->Init();

	int result = Game->Run();

	return result;
}
