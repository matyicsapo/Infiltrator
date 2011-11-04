#include "SFMLGameManager.hpp"

#include "GameState_Logos.hpp"

//#include "GameState_MainMenu.hpp"

int main () {
	GameState* xInitialGameState = new GameState_Logos();
	//GameState* xInitialGameState = new GameState_MainMenu();
	GameStateMachine* xGameStateMachine = new GameStateMachine(xInitialGameState);

	Game->SetGameStateMachine(xGameStateMachine);

	xInitialGameState->Enter();

	int result = Game->Run();

	return result;
}
