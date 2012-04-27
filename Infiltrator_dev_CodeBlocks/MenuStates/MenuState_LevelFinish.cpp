#include "MenuState_LevelFinish.hpp"

#include <boost/bind.hpp>

#include "../GameStates/GameState_MainMenu.hpp"
#include "../GameStates/GameState_Play.hpp"
#include "../TilingButton.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/DrawManager.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../FullScreenEventCatcherTint.hpp"
#include "../PFrameWork/GO/ScreenSprite.hpp"
#include "../Session.hpp"
#include "../DarkFader.hpp"
#include "../PFrameWork/GO/ScreenString.hpp"
#include "../PFrameWork/SoundManager.hpp"

#include <iostream>

MenuState_LevelFinish::MenuState_LevelFinish (bool successful)
	: successful(successful), darkFader(0) {
}

void MenuState_LevelFinish::Init () {
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

	int layerDepth = 5000;
	boost::function<void (void)> action = 0;

	ScreenString* sstr = 0;
	ScreenSprite* sspr = 0;

	if (successful) {
		sstr = new ScreenString(contents_lang["levelfinish"]["congrats"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(sstr);
		strings.push_back(sstr);
		sstr->SetPosition(sf::Vector2f(0, -150));
		sstr->SetScale(2);

		sstr = new ScreenString(contents_lang["levelfinish"]["won"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(sstr);
		strings.push_back(sstr);
		sstr->SetPosition(sf::Vector2f(0, -40));

		sstr = new ScreenString(contents_lang["levelfinish"]["gold"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(sstr);
		strings.push_back(sstr);
		int lastSizeX = sstr->GetScreenSize().x;
		sstr->SetPosition(sf::Vector2f(-200 + lastSizeX / 2, 30));
		sstr = new ScreenString(Convert->ToString(dynamic_cast<GameState_BasePlayable*>(Game->GetGameStateMachine()->Top())->GetGoldPickedUp()),
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(sstr);
		strings.push_back(sstr);
		sstr->SetPosition(sf::Vector2f(-190 + lastSizeX + sstr->GetScreenSize().x / 2, 30));
		lastSizeX += sstr->GetScreenSize().x;
		sspr = new ScreenSprite("Content/Textures/coin.png",
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(sspr);
		sprites.push_back(sspr);
		sspr->SetPosition(sf::Vector2f(-180 + lastSizeX + sspr->GetScreenSize().x / 2, 25));

		sstr = new ScreenString(contents_lang["levelfinish"]["allgold"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(sstr);
		strings.push_back(sstr);
		sstr->SetPosition(sf::Vector2f(-200 + sstr->GetScreenSize().x / 2, 90));
		lastSizeX = sstr->GetScreenSize().x;
		sstr = new ScreenString(Convert->ToString(CurrentSession->allGold),
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(sstr);
		strings.push_back(sstr);
		sstr->SetPosition(sf::Vector2f(-190 + lastSizeX + sstr->GetScreenSize().x / 2, 90));
		lastSizeX += sstr->GetScreenSize().x;
		sspr = new ScreenSprite("Content/Textures/coin.png",
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(sspr);
		sprites.push_back(sspr);
		sspr->SetPosition(sf::Vector2f(-180 + lastSizeX + sspr->GetScreenSize().x / 2, 85));
	}
	else {
		sstr = new ScreenString(contents_lang["levelfinish"]["lost"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(sstr);
		strings.push_back(sstr);
		sstr->SetPosition(sf::Vector2f(0, -90));
		sstr->SetScale(1.75f);
	}

	action = boost::bind(&MenuState_LevelFinish::OnClick_Restart, this);
	reBtn = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							action,
							contents_lang["levelfinish"][successful ? "re_success" : "re_fail"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	AddScreenSpace(reBtn);
	reBtn->SetPosition(sf::Vector2f(-350 + reBtn->GetScreenSize().x, 160));

	action = boost::bind(&MenuState_LevelFinish::OnClick_ExitToMain, this);
	mmBtn = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							action,
							contents_lang["levelfinish"]["tomm"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	AddScreenSpace(mmBtn);
	mmBtn->SetPosition(sf::Vector2f(305 - mmBtn->GetScreenSize().x / 2, 160));

	std::string txProfileBG[3] {
		"Content/Textures/GUI/profilebgleft.png",
		"Content/Textures/GUI/profilebgmiddle.png",
		"Content/Textures/GUI/profilebgright.png" };
	guiBox = new TilingButton(this, txProfileBG, txProfileBG, txProfileBG,
								0,
								"",
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								sf::Vector2f(0, 0),
								450,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth - 500);
	guiBox->SetPosition(sf::Vector2f(0, 0));
	guiBox->SetScale(1.5f);

	fullScrTint = new FullScreenEventCatcherTint(layerDepth - 1000);
	AddScreenSpace(fullScrTint);

	SortAscendingScreen();
}

MenuState_LevelFinish::~MenuState_LevelFinish () {
	delete reBtn;
	delete mmBtn;

	PopScreenSpace(fullScrTint);
	delete fullScrTint;

	delete guiBox;

	for (std::vector<ScreenString*>::iterator itS = strings.begin(); itS != strings.end(); ++itS) {
		PopScreenSpace(*itS);
		delete (*itS);
	}

	for (std::vector<ScreenSprite*>::iterator itS = sprites.begin(); itS != sprites.end(); ++itS) {
		PopScreenSpace(*itS);
		delete (*itS);
	}

	PopScreenSpace(darkFader);
	delete darkFader;
}

void MenuState_LevelFinish::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	reBtn->HandleSFEvents(sfEvents);
	mmBtn->HandleSFEvents(sfEvents);
}

void MenuState_LevelFinish::Update(float dT) {
	if (darkFader)
		darkFader->Update(dT);
}

void MenuState_LevelFinish::OnClick_ExitToMain () {
	boost::function<void (void)> exitPlay = boost::bind(&MenuState_LevelFinish::ExitPlay, this);

	darkFader = new DarkFader(.25f, true, exitPlay, 20000);
	AddScreenSpace(darkFader);
}

void MenuState_LevelFinish::OnClick_Restart () {
	boost::function<void (void)> restartLevel = boost::bind(&MenuState_LevelFinish::RestartLevel, this);

	darkFader = new DarkFader(.25f, true, restartLevel, 20000);
	AddScreenSpace(darkFader);
}

void MenuState_LevelFinish::RestartLevel () {
	Game->GetGameStateMachine()->PopBack();
	Game->GetGameStateMachine()->Push(
		new GameState_Play(
			static_cast<GameState_Play*>(Game->GetGameStateMachine()->Top())->GetLevelPath() ));
}

void MenuState_LevelFinish::ExitPlay () {
	Game->GetGameStateMachine()->PopBack();
	Game->GetGameStateMachine()->Push(new GameState_MainMenu());
}
