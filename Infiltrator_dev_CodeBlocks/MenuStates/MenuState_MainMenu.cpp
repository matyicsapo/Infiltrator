#include "MenuState_MainMenu.hpp"

#include <boost/bind.hpp>

#include "../GameStates/GameState_Editor.hpp"
#include "../GameStates/GameState_MainMenu.hpp"
#include "../TilingButton.hpp"
#include "../PFrameWork/ConfigParser.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/DrawManager.hpp"
#include "MenuState_Options.hpp"
#include "MenuState_PlayLevelLoad.hpp"
#include "MenuState_Credits.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../FullScreenEventCatcherTint.hpp"
#include "../Session.hpp"

MenuState_MainMenu::MenuState_MainMenu (boost::function<void (GameState*)> changeGameState)
		: changeGameState(changeGameState) {
}

void MenuState_MainMenu::Init () {
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
	int yOffset = -50;
	boost::function<void (void)>
	onClick = boost::bind(&MenuState_MainMenu::OnClick_menu_main_play, this);
	TilingButton*
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
							contents_lang["menu_main"]["play"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	onClick = boost::bind(&MenuState_MainMenu::OnClick_menu_main_editor, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
							contents_lang["menu_main"]["editor"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	onClick = boost::bind(&MenuState_MainMenu::OnClick_menu_main_options, this);
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
	onClick = boost::bind(&MenuState_MainMenu::OnClick_menu_main_credits, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
							contents_lang["menu_main"]["credits"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	yOffset += vSpacing;
	onClick = boost::bind(&MenuState_MainMenu::OnClick_menu_main_exit, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							onClick,
							contents_lang["menu"]["exitgame"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(0, yOffset));
	buttons.push_back(tb);

	gameLogo = new ScreenSprite("Content/Textures/title.png",
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth - 500);
	AddScreenSpace(gameLogo);
	gameLogo->SetScale(sf::Vector2f(.8f, .8f));
	gameLogo->SetPosition(sf::Vector2f(0, -200));

	fullScrTint = new FullScreenEventCatcherTint(layerDepth - 1000);
	AddScreenSpace(fullScrTint);

	SortAscendingScreen();
}

MenuState_MainMenu::~MenuState_MainMenu () {
	for (std::vector<TilingButton*>::iterator itBtn = buttons.begin(); itBtn != buttons.end(); itBtn++) {
		delete (*itBtn);
	}

	PopScreenSpace(fullScrTint);
	delete fullScrTint;
}

void MenuState_MainMenu::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	for (std::vector<TilingButton*>::iterator itBtn = buttons.begin(); itBtn != buttons.end(); itBtn++) {
		(*itBtn)->HandleSFEvents(sfEvents);
	}
}

void MenuState_MainMenu::Update(float dT) {
}

void MenuState_MainMenu::OnClick_menu_main_play () {
	Game->GetGameStateMachine()->Top()->GetMenuStateMachine()->Push(new MenuState_PlayLevelLoad());
}

void MenuState_MainMenu::OnClick_menu_main_editor () {
	changeGameState(new GameState_Editor(""));
}

void MenuState_MainMenu::OnClick_menu_main_options () {
	Game->GetGameStateMachine()->Top()->GetMenuStateMachine()->Push(new MenuState_Options());
}

void MenuState_MainMenu::OnClick_menu_main_credits () {
	Game->GetGameStateMachine()->Top()->GetMenuStateMachine()->Push(new MenuState_Credits());
}

void MenuState_MainMenu::OnClick_menu_main_exit () {
	Game->GetRenderWindow()->Close();
}

void MenuState_MainMenu::OnLanguageChanged (CfgContents& contents_lang) {
	buttons[0]->SetText(contents_lang["menu_main"]["play"]);
	buttons[1]->SetText(contents_lang["menu_main"]["editor"]);
	buttons[2]->SetText(contents_lang["menu_options"]["caption"]);
	buttons[3]->SetText(contents_lang["menu_main"]["credits"]);
	buttons[4]->SetText(contents_lang["menu"]["exitgame"]);
}
