#include "MenuState_Credits.hpp"

#include <boost/bind.hpp>

#include "../TilingButton.hpp"
#include "../PFrameWork/GO/ScreenSprite.hpp"
#include "../PFrameWork/GO/ScreenString.hpp"
#include "../PFrameWork/ConfigParser.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../FullScreenEventCatcherTint.hpp"
#include "../Session.hpp"

MenuState_Credits::MenuState_Credits () {
}

void MenuState_Credits::Init () {
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
	boost::function<void (void)> event = 0;

	allTexts = new ScreenString *[10];

	int spaceX = 5;

	int leftX = -335;

	int offsetY = 30;
	allTexts[0] = new ScreenString(contents_lang["credits"]["creator_caption"],
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[0]);
	allTexts[0]->SetPosition(sf::Vector2f(leftX + allTexts[0]->GetScreenSize().x / 2, offsetY));
	allTexts[1] = new ScreenString(contents_lang["credits"]["creator_name"],
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[1]);
	allTexts[1]->SetPosition(sf::Vector2f(leftX + spaceX + allTexts[0]->GetScreenSize().x + allTexts[1]->GetScreenSize().x / 2, offsetY));
	allTexts[2] = new ScreenString("(" + contents_lang["credits"]["creator_contact"] + ")",
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[2]);
	allTexts[2]->SetPosition(sf::Vector2f(leftX + spaceX * 2 + allTexts[0]->GetScreenSize().x + allTexts[1]->GetScreenSize().x + allTexts[2]->GetScreenSize().x / 2, offsetY));

	offsetY = 75;
	allTexts[3] = new ScreenString(contents_lang["credits"]["sound_caption"],
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[3]);
	allTexts[3]->SetPosition(sf::Vector2f(leftX + allTexts[3]->GetScreenSize().x / 2, offsetY));
	allTexts[4] = new ScreenString(contents_lang["credits"]["sound_name"],
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[4]);
	allTexts[4]->SetPosition(sf::Vector2f(leftX + spaceX + allTexts[3]->GetScreenSize().x + allTexts[4]->GetScreenSize().x / 2, offsetY));
	allTexts[5] = new ScreenString("(" + contents_lang["credits"]["sound_contact"] + ")",
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[5]);
	allTexts[5]->SetPosition(sf::Vector2f(leftX + spaceX * 2 + allTexts[3]->GetScreenSize().x + allTexts[4]->GetScreenSize().x + allTexts[5]->GetScreenSize().x / 2, offsetY));

	offsetY = 120;
	allTexts[6] = new ScreenString(contents_lang["credits"]["thanks"],
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[6]);
	allTexts[6]->SetPosition(sf::Vector2f(leftX + allTexts[6]->GetScreenSize().x / 2, offsetY));

	offsetY = 160;
	allTexts[7] = new ScreenString(contents_lang["credits"]["thanks1"],
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[7]);
	allTexts[7]->SetPosition(sf::Vector2f(-300 + allTexts[7]->GetScreenSize().x / 2, offsetY));

	offsetY = 200;
	allTexts[8] = new ScreenString(contents_lang["credits"]["thanks2"],
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[8]);
	allTexts[8]->SetPosition(sf::Vector2f(-300 + allTexts[8]->GetScreenSize().x / 2, offsetY));

	offsetY = 300;
	allTexts[9] = new ScreenString("2011 - 2012",
									PFWConstants::defaultFontFile,
									PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
									layerDepth);
	AddScreenSpace(allTexts[9]);
	allTexts[9]->SetPosition(sf::Vector2f(0, offsetY));


	event = boost::bind(&MenuState_Credits::OnClick_back, this);
	backButton = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["menu"]["back"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::BOTTOM,
							layerDepth);
	backButton->SetPosition(sf::Vector2f(20, -20));

	gameLogo = new ScreenSprite("Content/Textures/title.png",
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth - 500);
	AddScreenSpace(gameLogo);
	gameLogo->SetScale(.8f);
	gameLogo->SetPosition(sf::Vector2f(0, -200));

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
	guiBox->SetPosition(sf::Vector2f(0, 150));
	guiBox->SetScale(sf::Vector2f(1.6f, 1.2f));

	fullScrTint = new FullScreenEventCatcherTint(layerDepth - 1000);
	AddScreenSpace(fullScrTint);

	SortAscendingScreen();
}

MenuState_Credits::~MenuState_Credits () {
	PopScreenSpace(fullScrTint);
	delete fullScrTint;

	delete guiBox;

	PopScreenSpace(gameLogo);
	delete gameLogo;

	delete backButton;

	if (allTexts) {
		for (int i = 0; i != 10; ++i) {
			PopScreenSpace(allTexts[i]);
			delete allTexts[i];
		}
		delete[] allTexts;
	}
}

void MenuState_Credits::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	backButton->HandleSFEvents(sfEvents);
}

void MenuState_Credits::Update(float dT) {
}

void MenuState_Credits::OnClick_back () {
	pop = true;
}
