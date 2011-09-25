#include "SFMLGameManager.hpp"

#include "GameState_Logos.hpp"

int main () {
    // a dinamikusan lefoglalt memóriáról a hozzájuk tartozó osztályok gondoskodnak ezért ezek törlésével nem kell(szabad) foglalozni
	GameState* pInitialGameState = new GameState_Logos();
	GameStateMachine* pGameStateMachine = new GameStateMachine(pInitialGameState);

	SFMLGameManager::Instance()->SetGameStateMachine(pGameStateMachine);

	pInitialGameState->Enter(pGameStateMachine);

	int result = SFMLGameManager::Instance()->Run();

	return result;
}
