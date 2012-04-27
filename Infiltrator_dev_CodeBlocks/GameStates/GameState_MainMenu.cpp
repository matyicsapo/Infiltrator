#include "GameState_MainMenu.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../DarkFader.hpp"
#include "../MenuStates/MenuState_MainMenu.hpp"
#include "../MenuStates/MenuState_StartLogin.hpp"
#include "../GroundSprite.hpp"
#include "../Session.hpp"

GameState_MainMenu::GameState_MainMenu (MenuState* plusMenuStateToPush)
		: mMenuStateMachine(0),
		plusMenuStateToPush(plusMenuStateToPush),
		inoutFader(0) {
	mMenuStateMachine = new StateMachine<MenuState>();
}

GameState_MainMenu::~GameState_MainMenu () {
	ClearAll();

	delete mMenuStateMachine;
	delete inoutFader;
	delete groundSprite;
}

void GameState_MainMenu::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	mMenuStateMachine->RefreshStack();

	if (inoutFader == 0) {
		mMenuStateMachine->HandleSFEvents(sfEvents);
	}

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

void GameState_MainMenu::Init () {
	GameState::Init();

	Game->GetWorldCamera2D()->SetZoom(1);

	if (CurrentSession->profileName != "") {
		boost::function<void (GameState*)>
		changeGameState = boost::bind(&GameState_MainMenu::ScheduleChangeGameState, this, _1);
		mMenuStateMachine->Push(new MenuState_MainMenu(changeGameState));
	}
	else {
		mMenuStateMachine->Push(new MenuState_StartLogin(true));
	}

	if (plusMenuStateToPush) {
		mMenuStateMachine->Push(plusMenuStateToPush);
	}

	boost::function<void (void)>
	onFinishFadeLight = boost::bind(&GameState_MainMenu::OnFinishFadeLight, this);

	inoutFader = new DarkFader(.25f, false, onFinishFadeLight, 5000);
	AddScreenSpace(inoutFader);

	groundSprite = new GroundSprite("Content/Textures/grass.png", -10);
	AddWorldSpace(groundSprite);

	SortAscendingScreen();
	SortAscendingWorld();
}

void GameState_MainMenu::OnFinishFadeLight () {
	PopScreenSpace(inoutFader);
	delete inoutFader;
	inoutFader = 0;
}

void GameState_MainMenu::Update (float dT) {
	if (inoutFader == 0) {
		mMenuStateMachine->Update(dT);
	}
	else {
		inoutFader->Update(dT);
	}
}

void GameState_MainMenu::DrawScreenSpace (sf::RenderWindow& rwin) {
	mMenuStateMachine->DrawScreenSpace(rwin);

	State::DrawScreenSpace(rwin);
}

void GameState_MainMenu::ScheduleChangeGameState (GameState* gameState) {
	boost::function<void (void)>
	onFinishFadeDark = boost::bind(&GameState_MainMenu::DoChangeGameState, this, gameState);

	inoutFader = new DarkFader(.25f, true, onFinishFadeDark, 5000);
	AddScreenSpace(inoutFader);
}

void GameState_MainMenu::DoChangeGameState (GameState* gameState) {
	pop = true;
	Game->GetGameStateMachine()->Push(gameState);
}

StateMachine<MenuState>* GameState_MainMenu::GetMenuStateMachine () {
	return mMenuStateMachine;
}

void GameState_MainMenu::Login () {
	boost::function<void (GameState*)>
	changeGameState = boost::bind(&GameState_MainMenu::ScheduleChangeGameState, this, _1);

	mMenuStateMachine->Push(new MenuState_MainMenu(changeGameState));
}

void GameState_MainMenu::OnLanguageChanged (CfgContents& contents_lang) {
	mMenuStateMachine->OnLanguageChanged(contents_lang);
}
