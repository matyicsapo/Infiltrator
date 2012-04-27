#include "MenuState_EditorPause.hpp"

#include <boost/bind.hpp>

#include "../GameStates/GameState_Editor.hpp"
#include "../TilingButton.hpp"
#include "../PFrameWork/ConfigParser.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/DrawManager.hpp"
#include "../GameStates/GameState_MainMenu.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../FullScreenEventCatcherTint.hpp"
#include "MenuState_Options.hpp"
#include "../Session.hpp"
#include "MenuState_EditorLevelLoad.hpp"
#include "../DarkFader.hpp"

MenuState_EditorPause::MenuState_EditorPause () : darkFader(0) {
}

void MenuState_EditorPause::Init () {
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
	boost::function<void (void)>
	onClick = boost::bind(&MenuState_EditorPause::OnClick_menu_editorpause_edit, this);
	TilingButton*
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
							contents_lang["menu_editorpause"]["edit"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	onClick = boost::bind(&MenuState_EditorPause::OnClick_menu_editorpause_test, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
							contents_lang["menu_editorpause"]["test"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	onClick = boost::bind(&MenuState_EditorPause::OnClick_menu_editorpause_save, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
							contents_lang["menu_editorpause"]["save"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	onClick = boost::bind(&MenuState_EditorPause::OnClick_menu_editorpause_load, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
							contents_lang["menu_editorpause"]["load"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	onClick = boost::bind(&MenuState_EditorPause::OnClick_menu_options, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
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
	onClick = boost::bind(&MenuState_EditorPause::OnClick_menu_editorpause_mainmenu, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
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

MenuState_EditorPause::~MenuState_EditorPause () {
	ClearAll();

	for (std::vector<TilingButton*>::iterator itBtn = buttons.begin(); itBtn != buttons.end(); itBtn++) {
		delete (*itBtn);
	}

	delete fullScrTint;

	delete darkFader;
}

void MenuState_EditorPause::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	if (darkFader)
		return;

	for (std::vector<TilingButton*>::iterator itBtn = buttons.begin(); itBtn != buttons.end(); itBtn++) {
		(*itBtn)->HandleSFEvents(sfEvents);
	}
}

void MenuState_EditorPause::Update(float dT) {
	if (darkFader) {
		darkFader->Update(dT);

		return;
	}
}

void MenuState_EditorPause::OnClick_menu_editorpause_edit () {
	dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top())->SetTesting(false);
	pop = true;
}

void MenuState_EditorPause::OnClick_menu_editorpause_test () {
	dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top())->SetTesting(true);
	pop = true;
}

void MenuState_EditorPause::OnClick_menu_editorpause_save () {
	dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top())->InvokSaveLevel();
}

void MenuState_EditorPause::OnClick_menu_editorpause_load () {
	Game->GetGameStateMachine()->Top()->GetMenuStateMachine()->Push(new MenuState_EditorLevelLoad());
}

void MenuState_EditorPause::OnClick_menu_editorpause_mainmenu () {
	boost::function<void (void)> exitPlay = boost::bind(&MenuState_EditorPause::ExitToMainMenu, this);

	darkFader = new DarkFader(.25f, true, exitPlay, 20000);
	AddScreenSpace(darkFader);
}

void MenuState_EditorPause::OnClick_menu_options () {
	Game->GetGameStateMachine()->Top()->GetMenuStateMachine()->Push(new MenuState_Options());
}

void MenuState_EditorPause::ExitToMainMenu () {
	Game->GetGameStateMachine()->PopBack();
	Game->GetGameStateMachine()->Push(new GameState_MainMenu());
}

void MenuState_EditorPause::OnLanguageChanged (CfgContents& contents_lang) {
	buttons[0]->SetText(contents_lang["menu_editorpause"]["edit"]);
	buttons[1]->SetText(contents_lang["menu_editorpause"]["test"]);
	buttons[2]->SetText(contents_lang["menu_editorpause"]["save"]);
	buttons[3]->SetText(contents_lang["menu_editorpause"]["load"]);
	buttons[4]->SetText(contents_lang["menu_options"]["caption"]);
	buttons[5]->SetText(contents_lang["menu"]["exittomain"]);
}
