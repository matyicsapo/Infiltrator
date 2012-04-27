#include "GameState_Play.hpp"

#include <boost/bind.hpp>

#include "../PFrameWork/StateMachine.hpp"
#include "../MenuStates/MenuState_PlayPause.hpp"
#include "../MenuStates/MenuState_PlayLevelLoad.hpp"
#include "../MenuStates/MenuState_LevelFinish.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/GameObjectManager.hpp"
#include "GameState_MainMenu.hpp"
#include "../Guard.hpp"
#include "../Player.hpp"
#include "../Princess.hpp"
#include "../MyMagicNumbers.hpp"
#include "../DarkFader.hpp"
#include "../Session.hpp"

//#include <iostream>

GameState_Play::GameState_Play (std::string levelPath)
		: GameState_BasePlayable(levelPath) {
}

void GameState_Play::Init () {
	GameState_BasePlayable::Init();

	if (Nav->GetNavMap()) {
		OnFinishedNavMapGen(true);
	}
	else {
		inoutFader->SetFinished(true);

		DoGenerateNavMap();
	}

	SortAscendingScreen();
}

void GameState_Play::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	GameState_BasePlayable::HandleSFEvents(sfEvents);
}

void GameState_Play::Update (float dT) {
	GameState_BasePlayable::Update(dT);

	if (inoutFader || !mMenuStateMachine->IsEmpty())
		return;

	UpdateGamePlay(dT);

	if (finished) {
		if (player) player->PauseAudio();
		if (princess) princess->PauseAudio();

		const EntityMap& guards = (*GameObjects)[ID_GUARD];
		for (EntityMap::const_iterator itG = guards.begin(); itG != guards.end(); itG++) {
			dynamic_cast<Guard*>(itG->second)->PauseAudio();
		}
	}
}

void GameState_Play::OnPause (bool pause) {
	GameState_BasePlayable::OnPause(pause);

	if (pause) {
		mMenuStateMachine->Push(new MenuState_PlayPause());
	}
	else {
		// only the top => this only takes you back 1 menu
		mMenuStateMachine->PopBack();
	}
}

void GameState_Play::OnFinishedNavMapGen (bool successful) {
	GameState_BasePlayable::OnFinishedNavMapGen(successful);

	if (successful) {
		inoutFader->SetFinished(false);

		SaveLevel(levelPath, levelPath.substr(0, levelPath.find_last_of("/\\") + 1) + "levlist.ini");

		StartPlaying();


//NavMap* navMap = Nav->GetNavMap();
//for (NavMap::iterator itN = navMap->begin(); itN != navMap->end(); ++itN) {
//
//std::cout << itN->first.x << " " << itN->first.y << std::endl;
//
//	for (std::deque<sf::Vector2i>::iterator itA = itN->second.begin();
//			itA != itN->second.end(); ++itA) {
//
//std::cout << "  " << (*itA).x << " " << (*itA).y << std::endl;
//
//	}
//}


	}
	else {
		pop = true;
		Game->GetGameStateMachine()->Push(new GameState_MainMenu(new MenuState_PlayLevelLoad()));
	}
}

void GameState_Play::OnLevelLoaded (bool successful) {
	if (!successful) {
		pop = true;
		Game->GetGameStateMachine()->Push(new GameState_MainMenu(new MenuState_PlayLevelLoad()));
	}
}

std::string GameState_Play::GetLevelPath () {
	return levelPath;
}

void GameState_Play::OnGameFinished (bool successful) {
	GameState_BasePlayable::OnGameFinished(successful);

	if (successful) {
		CurrentSession->allGold += goldPickedUp;

		CurrentSession->SaveProfile();
	}

	mMenuStateMachine->Push(new MenuState_LevelFinish(successful));
}
