#include "GameState_Logos.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "../PFrameWork/DrawManager.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../PFrameWork/PFWConstants.hpp"
#include "../PFrameWork/GO/ScreenSprite.hpp"
#include "../DarkFader.hpp"
#include "GameState_MainMenu.hpp"
#include "../PFrameWork/GO/SoundEffect.hpp"
#include "../PFrameWork/GO/Music.hpp"
#include "../PFrameWork/TINI/TINI.hpp"
#include "../MenuStates/MenuState_Language.hpp"
#include "../MyMagicNumbers.hpp"
#include "../Session.hpp"
#include "../PFrameWork/SoundManager.hpp"

#define SHOWDURATION 1.0f
#define FADEDURATION .25f

GameState_Logos::GameState_Logos () : finished(false), inoutFader(0), currentLogoSprite(0) {
	mMenuStateMachine = new StateMachine<MenuState>();
}

GameState_Logos::~GameState_Logos () {
	ClearAll();

	delete inoutFader;
	delete currentLogoSprite;
	delete mMenuStateMachine;

	Game->SetClear(false);
}

void GameState_Logos::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	mMenuStateMachine->RefreshStack();

	mMenuStateMachine->HandleSFEvents(sfEvents);

	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::Closed:
				Game->GetRenderWindow()->Close();
			break;
			case sf::Event::KeyPressed:
				if (itSfEvent->Key.Code == sf::Key::Escape ||
						itSfEvent->Key.Code == sf::Key::Return ||
						itSfEvent->Key.Code == sf::Key::Space) {
					if (mMenuStateMachine->IsEmpty() && !finished) {
						Finish();
					}
				}
			break;
			default:
			break;
		}

		itSfEvent++;
	}
}

void GameState_Logos::Init () {
	GameState::Init();

	boost::function<void (void)>
	onFinishFadeLight = boost::bind(&GameState_Logos::OnFinishFadeLight, this);
	inoutFader = new DarkFader(.50f, false, onFinishFadeLight, 5000);
	AddScreenSpace(inoutFader);

	logos.push_back(std::pair<std::string, sf::Color>("Content/Textures/KasapoGames.png", sf::Color::Black));
	logos.push_back(std::pair<std::string, sf::Color>("Content/Textures/sfml-big.png", sf::Color::White));

	currentLogo = logos.begin();

	currentLogoSprite = new ScreenSprite(currentLogo->first, ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE);
	AddScreenSpace(currentLogoSprite);

	Game->SetClearColor(currentLogo->second);
	currentLogo++;

	Game->SetClear(true);

	TINI::TINIObject globalConf("Saves/global.ini");
	std::string
	s = globalConf.GetValue("", "vol_master");
	if (s != "") Sounds->SetVol_Master(Convert->ToNum<float>(s));
	s = globalConf.GetValue("", "vol_music");
	if (s != "") Sounds->SetVol_Music(Convert->ToNum<float>(s));
	s = globalConf.GetValue("", "vol_effect");
	if (s != "") Sounds->SetVol_Effect(Convert->ToNum<float>(s));
	if (!CurrentSession->LoadProfile(globalConf.GetValue("", "last_profile"))) {
		CurrentSession->language = "";
	}
	if (CurrentSession->language == "") {
		CurrentSession->language = globalConf.GetValue("", "language");

		std::string sw = globalConf.GetValue("", "clientwidth");
		std::string sh = globalConf.GetValue("", "clientheight");
		Game->Create(sw != "" ? Convert->ToNum<int>(sw) : PFWConstants::baseResolution.x,
				sh != "" ? Convert->ToNum<int>(sh) : PFWConstants::baseResolution.y,
				globalConf.GetValue("", "fullscr") == "1" ? true : false);
	}

	Music* backgroundMusic = new Music("Content/Audio/bgm.wav");
	backgroundMusic->Play();
	backgroundMusic->SetLoop(true);
	Sounds->RefreshVol_Music_Of(backgroundMusic);

	SortAscendingScreen();
}

void GameState_Logos::Update (float dT) {
	if (inoutFader && !inoutFader->Finished()) {
		inoutFader->Update(dT);

		return;
	}

	if (!mMenuStateMachine->IsEmpty()) {
		mMenuStateMachine->Update(dT);

		return;
	}

	if (finished) {
		StartMainMenu();

		return;
	}

	showTimeLeft -= dT;

	if (showTimeLeft <= 0) {
		if (currentLogo == logos.end()) {
			Finish();
		}
		else {
			boost::function<void (void)>
			fadeToNextLogo = boost::bind(&GameState_Logos::FadeToNextLogo, this);
			inoutFader = new DarkFader(FADEDURATION, true, fadeToNextLogo, 5000);
			AddScreenSpace(inoutFader);
			SortAscendingScreen();
		}
	}
}

void GameState_Logos::DrawScreenSpace (sf::RenderWindow& rwin) {
	State::DrawScreenSpace(rwin);

	mMenuStateMachine->DrawScreenSpace(rwin);
}

void GameState_Logos::OnFinishFadeLight () {
	PopScreenSpace(inoutFader);
	delete inoutFader;
	inoutFader = 0;

 	showTimeLeft = SHOWDURATION;
}

void GameState_Logos::FadeToNextLogo () {
	PopScreenSpace(currentLogoSprite);
	delete currentLogoSprite;
	currentLogoSprite = new ScreenSprite(currentLogo->first, ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE);
	AddScreenSpace(currentLogoSprite);
	Game->SetClearColor(currentLogo->second);
	currentLogo++;

	PopScreenSpace(inoutFader);
	delete inoutFader;
	boost::function<void (void)>
	onFinishFadeLight = boost::bind(&GameState_Logos::OnFinishFadeLight, this);
	inoutFader = new DarkFader(FADEDURATION, false, onFinishFadeLight, 5000);
	AddScreenSpace(inoutFader);

	SortAscendingScreen();
}

void GameState_Logos::StartMainMenu () {
	pop = true;
	Game->GetGameStateMachine()->Push(new GameState_MainMenu());
}

void GameState_Logos::StartLanguageSelection () {
	PopScreenSpace(currentLogoSprite);
	delete currentLogoSprite;
	currentLogoSprite = 0;

	mMenuStateMachine->Push(new MenuState_Language());
}

void GameState_Logos::Finish () {
	if (CurrentSession->language == "") {
		boost::function<void (void)>
		startLanguageSelection = boost::bind(&GameState_Logos::StartLanguageSelection, this);
		inoutFader = new DarkFader(FADEDURATION, true, startLanguageSelection, 5000);
	}
	else {
		boost::function<void (void)>
		startMainMenu = boost::bind(&GameState_Logos::StartMainMenu, this);
		inoutFader = new DarkFader(FADEDURATION, true, startMainMenu, 5000);
	}

	AddScreenSpace(inoutFader);
	SortAscendingScreen();

	finished = true;
}
