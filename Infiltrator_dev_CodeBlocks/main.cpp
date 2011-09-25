#include "SFMLGameManager.hpp"

#include "GameState_Logos.hpp"

int main () {
    // a dinamikusan lefoglalt mem�ri�r�l a hozz�juk tartoz� oszt�lyok gondoskodnak ez�rt ezek t�rl�s�vel nem kell(szabad) foglalozni
	GameState* pInitialGameState = new GameState_Logos();
	GameStateMachine* pGameStateMachine = new GameStateMachine(pInitialGameState);

	SFMLGameManager::Instance()->SetGameStateMachine(pGameStateMachine);

	pInitialGameState->Enter(pGameStateMachine);

	int result = SFMLGameManager::Instance()->Run();

	return result;
}
