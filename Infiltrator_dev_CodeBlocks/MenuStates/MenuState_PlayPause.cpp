#include "MenuState_PlayPause.hpp"

#include <boost/bind.hpp>

#include "../TilingButton.hpp"
#include "../PFrameWork/ConfigParser.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/DrawManager.hpp"
#include "../GameStates/GameState_MainMenu.hpp"
#include "../GameStates/GameState_Play.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../FullScreenEventCatcherTint.hpp"
#include "MenuState_Options.hpp"
#include "../Session.hpp"
#include "../DarkFader.hpp"
#include "../AI/Navigation.hpp"

MenuState_PlayPause::MenuState_PlayPause () : darkFader(0) {
}

void MenuState_PlayPause::Init () {
	MenuState::Init();

	std::string txInactiveNormal[3] {
		"Content/Textures/GUI/darkbuttonleft.png",
		"Content/Textures/GUI/darkbuttonmiddle.png",
		"Content/Textures/GUI/darkbuttonright.png" };
	std::string txInactiveHover[3] {
		"Content/Textures/GUI/buttonleft.png",
		"Content/Textures/GUI/buttonmiddle.png",
		"Content/Textures/GUI/buttonright.png" };
	std::string txActiveHover[3] {
		"Content/Textures/GUI/pressedbuttonleft.png",
		"Content/Textures/GUI/pressedbuttonmiddle.png",
		"Content/Textures/GUI/pressedbuttonright.png" };

	CfgContents contents_lang;
	CfgParser->GetContents(std::string("Content/LanguageFiles/") + CurrentSession->language
		+ std::string(".ini"), contents_lang);

	int layerDepth = 50000;
	int vSpacing = 80;
	int yOffset = -160;
	boost::function<void (void)> event = 0;
	TilingButton* tb = 0;

	event = boost::bind(&MenuState_PlayPause::OnClick_Resume, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["menu_playpause"]["resume"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	event = boost::bind(&MenuState_PlayPause::OnClick_Restart, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["menu_playpause"]["restart"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	event = boost::bind(&MenuState_PlayPause::OnClick_Options, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["menu_options"]["caption"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	event = boost::bind(&MenuState_PlayPause::OnClick_ExitToMain, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["menu"]["exittomain"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	fullScrTint = new FullScreenEventCatcherTint(layerDepth - 1000);
	AddScreenSpace(fullScrTint);

	SortAscendingScreen();
}

MenuState_PlayPause::~MenuState_PlayPause () {
	ClearAll();

	for (std::vector<TilingButton*>::iterator itBtn = buttons.begin(); itBtn != buttons.end(); itBtn++) {
		delete (*itBtn);
	}

	delete fullScrTint;

	PopScreenSpace(darkFader);
	delete darkFader;
}

void MenuState_PlayPause::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	if (darkFader)
		return;

	for (std::vector<TilingButton*>::iterator itBtn = buttons.begin(); itBtn != buttons.end(); itBtn++) {
		(*itBtn)->HandleSFEvents(sfEvents);
	}
}

void MenuState_PlayPause::Update(float dT) {
	if (darkFader) {
		darkFader->Update(dT);

		return;
	}
}

void MenuState_PlayPause::OnLanguageChanged (CfgContents& contents_lang) {
	buttons[0]->SetText(contents_lang["menu_playpause"]["resume"]);
	buttons[1]->SetText(contents_lang["menu_playpause"]["restart"]);
	buttons[2]->SetText(contents_lang["menu_options"]["caption"]);
	buttons[3]->SetText(contents_lang["menu"]["exittomain"]);
}

void MenuState_PlayPause::OnClick_Resume () {
	pop = true;
}

void MenuState_PlayPause::OnClick_Restart () {
	boost::function<void (void)> restartLevel = boost::bind(&MenuState_PlayPause::RestartLevel, this);

	darkFader = new DarkFader(.25f, true, restartLevel, 20000);
	AddScreenSpace(darkFader);
}

void MenuState_PlayPause::OnClick_Options () {
	dynamic_cast<GameState_Play*>(Game->GetGameStateMachine()->Top())->GetMenuStateMachine()->Push(new MenuState_Options());
}

void MenuState_PlayPause::OnClick_ExitToMain () {
	boost::function<void (void)> exitPlay = boost::bind(&MenuState_PlayPause::ExitPlay, this);

	darkFader = new DarkFader(.25f, true, exitPlay, 20000);
	AddScreenSpace(darkFader);
}

void MenuState_PlayPause::RestartLevel () {
	Game->GetGameStateMachine()->PopBack();
	Game->GetGameStateMachine()->Push(
		new GameState_Play(
			static_cast<GameState_Play*>(Game->GetGameStateMachine()->Top())->GetLevelPath() ));
}

void MenuState_PlayPause::ExitPlay () {
	Game->GetGameStateMachine()->PopBack();
	Game->GetGameStateMachine()->Push(new GameState_MainMenu());
}
