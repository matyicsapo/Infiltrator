#include "MenuState_StartLogin.hpp"

#include <boost/bind.hpp>

#include "../GameStates/GameState_MainMenu.hpp"
#include "../TilingButton.hpp"
#include "../PFrameWork/ConfigParser.hpp"
#include "../PFrameWork/TINI/TINI.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/DrawManager.hpp"
#include "MenuState_MainMenu.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../FullScreenEventCatcherTint.hpp"
#include "../PFrameWork/GO/ScreenSprite.hpp"
#include "../Session.hpp"
#include "../PFrameWork/GO/ScreenString.hpp"
#include "../PFrameWork/GO/GUI/GUITextfield.hpp"
#include "../PFrameWork/GO/GUI/GUIButton.hpp"
#include "MenuState_Language.hpp"
#include "../DarkFader.hpp"

MenuState_StartLogin::MenuState_StartLogin (bool startup)
	: startup(startup),
	profileNameArray(0), currProfileIndex(-1), lastProfileName(""),
	langFader(0) {
}

void MenuState_StartLogin::Init () {
	MenuState::Init();

	CfgContents contents_levels;
	CfgParser->GetContents("Saves/profilelist.ini", contents_levels);
	if (contents_levels.Exists("")) {
		profileNameArraySize = contents_levels[""].GetVector()->size();
		profileNameArray = new std::string[profileNameArraySize];
		unsigned int i = 0;
		for (CfgEntryMap::iterator itE = contents_levels[""].GetIteratorBegin();
				itE != contents_levels[""].GetIteratorEnd(); ++itE) {
			profileNameArray[i] = (*itE)->first;
			++i;
		}
		currProfileIndex = 0;
		lastProfileName = profileNameArray[currProfileIndex];
	}
	else profileNameArraySize = 0;

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
	boost::function<void (void)> action = 0;

	sf::Vector2f offset(startup ? sf::Vector2f(0, 0) : sf::Vector2f(0, -200));

	profileLabel = new ScreenString(contents_lang["misc"]["caption_profile"],
										PFWConstants::defaultFontFile,
										PFWConstants::defaultFontSize,
										ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
										layerDepth);
	AddScreenSpace(profileLabel);
	profileLabel->SetPosition(sf::Vector2f(-200 + profileLabel->GetScreenSize().x / 2, 40) + offset);

	action = boost::bind(&MenuState_StartLogin::OnClick_ChangeProfile, this, -1);
	profileLeftArrowBtn = new GUIButton("Content/Textures/GUI/darkarrowbutton.png",
								"Content/Textures/GUI/arrowbutton.png",
								"Content/Textures/GUI/pressedarrowbutton.png",
								action,
								"", PFWConstants::defaultFontFile, PFWConstants::defaultFontSize, sf::Vector2f(0, 0),
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	AddScreenSpace(profileLeftArrowBtn);
	profileLeftArrowBtn ->SetPosition(sf::Vector2f(-140, 100) + offset);

	action = boost::bind(&MenuState_StartLogin::OnClick_ChangeProfile, this, 1);
	profileRightArrowBtn = new GUIButton("Content/Textures/GUI/darkarrowbutton.png",
								"Content/Textures/GUI/arrowbutton.png",
								"Content/Textures/GUI/pressedarrowbutton.png",
								action,
								"", PFWConstants::defaultFontFile, PFWConstants::defaultFontSize, sf::Vector2f(0, 0),
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	AddScreenSpace(profileRightArrowBtn);
	profileRightArrowBtn ->SetPosition(sf::Vector2f(180, 100) + offset);
	profileRightArrowBtn->Rotate(180);

	profileTextField = new GUITextfield(this, "Content/Textures/GUI/levelNameTextField_normal.png",
										"Content/Textures/GUI/levelNameTextField_hover.png",
										sf::Color(128, 128, 128),
										sf::Color::White,
										profileNameArraySize != 0 ? profileNameArray[0] : "",
										"[0-9a-zA-Z]*",
										0,
										PFWConstants::defaultFontFile,
										PFWConstants::defaultFontSize,
										0,
										ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
										layerDepth);
	profileTextField->SetPosition(sf::Vector2f(20, 100) + offset);

	languageLabel = new ScreenString(contents_lang["misc"]["caption_language"],
										PFWConstants::defaultFontFile,
										PFWConstants::defaultFontSize,
										ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
										layerDepth);
	AddScreenSpace(languageLabel);
	languageLabel->SetPosition(sf::Vector2f(-200 + languageLabel->GetScreenSize().x / 2, 180) + offset);

	action = boost::bind(&MenuState_StartLogin::FadeToLanguageSelection, this);
	languageBtn = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
								action,
								contents_lang["misc"]["caption_language"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								sf::Vector2f(0, 0),
								-1,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	languageBtn->SetPosition(sf::Vector2f(-180 + languageLabel->GetScreenSize().x + languageBtn->GetScreenSize().x / 2, 180) + offset);

	action = boost::bind(&MenuState_StartLogin::OnClick_Baxit, this);
	exitBtn = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
								action,
								contents_lang["menu"][startup ? "exitgame" : "back"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								sf::Vector2f(0, 0),
								-1,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	exitBtn->SetPosition(sf::Vector2f(-200 + exitBtn->GetScreenSize().x / 2, 250) + offset);

	action = boost::bind(&MenuState_StartLogin::Login, this);
	playBtn = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
								action,
								contents_lang["menu_login"][currProfileIndex != -1 ? "start_btn" : "create_btn"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								sf::Vector2f(0, 0),
								105,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	playBtn->SetPosition(sf::Vector2f(200 - playBtn->GetScreenSize().x / 2, 250) + offset);

	if (startup) {
		gameLogo = new ScreenSprite("Content/Textures/title.png",
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth - 500);
		AddScreenSpace(gameLogo);
		gameLogo->SetScale(.8f);
		gameLogo->SetPosition(sf::Vector2f(0, -200) + offset);
	}
	else gameLogo = 0;

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
	guiBox->SetPosition(sf::Vector2f(0, 150) + offset);

	fullScrTint = new FullScreenEventCatcherTint(layerDepth - 1000);
	AddScreenSpace(fullScrTint);

	SortAscendingScreen();
}

MenuState_StartLogin::~MenuState_StartLogin () {
	PopScreenSpace(langFader);
	delete langFader;

	PopScreenSpace(fullScrTint);
	delete fullScrTint;

	delete guiBox;

	PopScreenSpace(gameLogo);
	delete gameLogo;

	PopScreenSpace(profileLabel);
	delete profileLabel;
	PopScreenSpace(profileLeftArrowBtn);
	delete profileLeftArrowBtn;
	PopScreenSpace(profileRightArrowBtn);
	delete profileRightArrowBtn;
	delete profileTextField;

	PopScreenSpace(languageLabel);
	delete languageLabel;
	delete languageBtn;

	delete exitBtn;

	delete playBtn;

	delete[] profileNameArray;
}

void MenuState_StartLogin::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	if (langFader)
		return;

	profileLeftArrowBtn->HandleSFEvents(sfEvents);
	profileRightArrowBtn->HandleSFEvents(sfEvents);
	profileTextField->HandleSFEvents(sfEvents);

	languageBtn->HandleSFEvents(sfEvents);

	exitBtn->HandleSFEvents(sfEvents);

	if (profileTextField->GetText() != "")
		playBtn->HandleSFEvents(sfEvents);
	else
		playBtn->SetActive(false);
}

void MenuState_StartLogin::Update(float dT) {
	if (langFader) {
		if (langFader->Finished()) {
			PopScreenSpace(langFader);
			delete langFader;
			langFader = 0;
		}
		else {
			langFader->Update(dT);
		}
	}

	profileTextField->Update(dT);

	if (profileNameArraySize == 0)
		return;

	std::string p = profileTextField->GetText();

	if (p == "") {
		TINI::TINIObject langFile("Content/LanguageFiles/" + CurrentSession->language + std::string(".ini"));
		playBtn->SetText(langFile.GetValue("menu_login", "create_btn"));
		return;
	}

	if (lastProfileName == p)
		return;

	lastProfileName = p;

	for (int i = 0; i != profileNameArraySize; ++i) {
		if (p == profileNameArray[i]) {
			currProfileIndex = i;

			TINI::TINIObject langFile("Content/LanguageFiles/" + CurrentSession->language + std::string(".ini"));
			playBtn->SetText(langFile.GetValue("menu_login", "start_btn"));

			return;
		}
	}

	currProfileIndex = -1;

	TINI::TINIObject langFile("Content/LanguageFiles/" + CurrentSession->language + std::string(".ini"));
	playBtn->SetText(langFile.GetValue("menu_login", "create_btn"));
}

void MenuState_StartLogin::Login () {
	GameState_MainMenu* mainmenu = dynamic_cast<GameState_MainMenu*>(Game->GetGameStateMachine()->Top());
	mainmenu->Login();

	if (currProfileIndex == -1) {
		// new profile

		CurrentSession->profileName = profileTextField->GetText();
		CurrentSession->allGold = 0;

		CurrentSession->SaveProfile();
	}
	else {
		// loading existing profile

		CurrentSession->LoadProfile(profileTextField->GetText());
	}

	pop = true;
}

void MenuState_StartLogin::FadeToLanguageSelection () {
	boost::function<void (void)>
	onFinishFadeDark = boost::bind(&MenuState_StartLogin::LanguageSelection, this);

	langFader = new DarkFader(.25f, true, onFinishFadeDark, 5000);
	AddScreenSpace(langFader);
}

void MenuState_StartLogin::LanguageSelection () {
	GameState_MainMenu* mainmenu = dynamic_cast<GameState_MainMenu*>(Game->GetGameStateMachine()->Top());
	mainmenu->GetMenuStateMachine()->Push(new MenuState_Language());
}

void MenuState_StartLogin::OnLanguageChanged (CfgContents& contents_lang) {
	sf::Vector2f offset(startup ? sf::Vector2f(0, 0) : sf::Vector2f(0, -200));

	profileLabel->SetText(contents_lang["misc"]["caption_profile"]);

	languageLabel->SetText(contents_lang["misc"]["caption_language"]);
	languageLabel->SetPosition(sf::Vector2f(-200 + languageLabel->GetScreenSize().x / 2, 180) + offset);
	languageBtn->SetText(contents_lang[""]["lang"]);
	languageBtn->SetPosition(sf::Vector2f(-180 + languageLabel->GetScreenSize().x + languageBtn->GetScreenSize().x / 2, 180) + offset);

	exitBtn->SetText(contents_lang["menu"][startup ? "exitgame" : "back"]);
	exitBtn->SetPosition(sf::Vector2f(-200 + exitBtn->GetScreenSize().x / 2, 250) + offset);

	playBtn->SetText(
		contents_lang["menu_login"][currProfileIndex == -1 ? "create_btn" : "start_btn"]);
	playBtn->SetPosition(sf::Vector2f(200 - playBtn->GetScreenSize().x / 2, 250) + offset);
}

void MenuState_StartLogin::OnClick_ChangeProfile (int change) {
	if (profileNameArraySize == 0)
		return;

	currProfileIndex += change;

	if (currProfileIndex > profileNameArraySize - 1) {
		currProfileIndex = 0;
	}
	else if (currProfileIndex < 0) {
		currProfileIndex = profileNameArraySize - 1;

		if (currProfileIndex < 0)
			currProfileIndex = 0;
	}

	lastProfileName = profileNameArray[currProfileIndex];

	profileTextField->SetActive(true);
	profileTextField->SetText(lastProfileName, true);

	TINI::TINIObject langFile("Content/LanguageFiles/" + CurrentSession->language + std::string(".ini"));
	playBtn->SetText(langFile.GetValue("menu_login", "start_btn"));
}

void MenuState_StartLogin::OnClick_Baxit () {
	if (startup)
		Game->GetRenderWindow()->Close();
	else
		pop = true;
}
