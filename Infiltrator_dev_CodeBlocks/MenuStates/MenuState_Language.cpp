#include "MenuState_Language.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "../PFrameWork/DrawManager.hpp"

#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/StateMachine.hpp"

#include "../PFrameWork/PFWConstants.hpp"

#include "../PFrameWork/GO/ScreenSprite.hpp"
#include "../PFrameWork/GO/ScreenString.hpp"

#include "../DarkFader.hpp"

#include "../PFrameWork/GO/GUI/GUIButton.hpp"
#include "../PFrameWork/ConfigParser.hpp"
#include "../PFrameWork/TINI/TINI.hpp"

//#include <iostream>

#include "../GameStates/GameState_Logos.hpp"

#include "../MyMagicNumbers.hpp"
#include "../Session.hpp"

MenuState_Language::MenuState_Language () : inoutFader(0) {
}

MenuState_Language::~MenuState_Language () {
	for (std::vector<GUIButton*>::iterator itL = langBtns.begin(); itL != langBtns.end(); ++itL) {
		PopScreenSpace(*itL);
		delete (*itL);
	}

	PopScreenSpace(inoutFader);
	delete inoutFader;

	PopScreenSpace(background);
	delete background;
}

void MenuState_Language::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	for (std::vector<GUIButton*>::iterator itL = langBtns.begin(); itL != langBtns.end(); ++itL) {
		(*itL)->HandleSFEvents(sfEvents);
	}
}

void MenuState_Language::Init () {
	MenuState::Init();

	background = new FullScreenEventCatcherTint(7500, sf::Color(0, 0, 0, 255));
	AddScreenSpace(background);

	inoutFader = new DarkFader(.25f, false, 0, 17500);
	AddScreenSpace(inoutFader);

	boost::function<void (void)> fadeOut;

	CfgContents contents_lang;
	CfgParser->GetContents(std::string("Content/LanguageFiles/languages.ini"), contents_lang);

	int langCnt = contents_lang[""].GetVector()->size();

	float yStep = 50;
	float yOffset = -(langCnt * 100 + (langCnt - 1) * yStep) / 2 + 50;

	for (CfgEntryMap::iterator itE = contents_lang[""].GetIteratorBegin();
			itE != contents_lang[""].GetIteratorEnd(); ++itE) {
		std::string lang = (*itE)->second;

		TINI::TINIObject* tINIObject = new TINI::TINIObject("Content/LanguageFiles/" + lang + ".ini");

		std::string logofile = tINIObject->GetValue("", "logofile");

		fadeOut = boost::bind(&MenuState_Language::FadeOut, this, lang);

		GUIButton* newLangBtn = new GUIButton("Content/LanguageFiles/" + logofile,
							"Content/LanguageFiles/" + logofile,
							"Content/LanguageFiles/" + logofile,
							fadeOut,
							tINIObject->GetValue("", "lang"),
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(200, 0),
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE, 10000);
		AddScreenSpace(newLangBtn);
		newLangBtn->SetPosition(sf::Vector2f(-100, yOffset));

		yOffset += yStep + 100;

		langBtns.push_back(newLangBtn);

		delete tINIObject;
	}

	SortAscendingScreen();
}

void MenuState_Language::Update (float dT) {
	inoutFader->Update(dT);
}

void MenuState_Language::SetLanguage (std::string lang) {
	pop = true;

//	GameState_Logos* logoState = dynamic_cast<GameState_Logos*>(Game->GetGameStateMachine()->Top());
//
//	if (logoState) {
//		logoState->ContinueWithLanguage(lang);
//		return;
//	}

//	GameState_MainMenu* mainMenuState
}

void MenuState_Language::FadeOut (std::string lang) {
	inoutFader = new DarkFader(.25f, true,
								boost::bind(&MenuState_Language::SetLanguage, this, lang),
								17500);
	AddScreenSpace(inoutFader);
	SortAscendingScreen();

	TINI::TINIObject* globalConf = new TINI::TINIObject("Saves/global.ini");
	globalConf->SetValue("", "language", lang);
	delete globalConf;

	CurrentSession->language = lang;

	CfgContents contents_lang;
	CfgParser->GetContents(std::string("Content/LanguageFiles/") + CurrentSession->language
		+ std::string(".ini"), contents_lang);

	Game->GetGameStateMachine()->OnLanguageChanged(contents_lang);
}
