#include "MenuState_EditorLevelLoad.hpp"

#include <boost/bind.hpp>

#include "../TilingButton.hpp"
#include "../MyMagicNumbers.hpp"
#include "../PFrameWork/GO/GUI/GUIButton.hpp"
#include "../PFrameWork/ConfigParser.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/DrawManager.hpp"
#include "MenuState_MainMenu.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../PFrameWork/TINI/TINI.hpp"
#include "../GameStates/GameState_MainMenu.hpp"
#include "../Session.hpp"
#include "../FullScreenEventCatcherTint.hpp"
#include "../GameStates/GameState_Editor.hpp"

#define WHEELSCROLLSPD 1500.0f

MenuState_EditorLevelLoad::MenuState_EditorLevelLoad () {
}

void MenuState_EditorLevelLoad::Init () {
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

	event = boost::bind(&MenuState_EditorLevelLoad::ResetLevelList, this, "Levels/Main/levlist.ini", false);
	btnMain = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["menu_load"]["main"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::TOP,
							layerDepth);
	btnMain->SetPosition(sf::Vector2f(100, 100));

	event = boost::bind(&MenuState_EditorLevelLoad::ResetLevelList, this, "Levels/User/levlist.ini", true);
	btnUser = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["menu_load"]["user"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::TOP,
							layerDepth);
	btnUser->SetPosition(sf::Vector2f(100, 200));

	event = boost::bind(&MenuState_EditorLevelLoad::OnClick_back, this);
	btnBack = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["menu"]["back"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::BOTTOM,
							layerDepth);
	btnBack->SetPosition(sf::Vector2f(100, -50));

	ResetLevelList("Levels/Main/levlist.ini", false);

	fullScrTint = new FullScreenEventCatcherTint(layerDepth - 1000);
	AddScreenSpace(fullScrTint);

	SortAscendingScreen();
}

MenuState_EditorLevelLoad::~MenuState_EditorLevelLoad () {
	ClearAll();

	for (std::vector<TilingButton*>::iterator itBtn = btnsLevels.begin(); itBtn != btnsLevels.end(); itBtn++) {
		delete (*itBtn);
	}

	for (std::vector<GUIButton*>::iterator itBtn = btnsDelete.begin(); itBtn != btnsDelete.end(); itBtn++) {
		delete (*itBtn);
	}

	delete btnMain;
	delete btnUser;
	delete btnBack;

	delete fullScrTint;
}

void MenuState_EditorLevelLoad::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	if (!justDeletedLevelNames.empty()) {
		for (std::vector<std::string>::iterator itL = justDeletedLevelNames.begin();
				itL != justDeletedLevelNames.end(); ++itL) {

			for (unsigned int i = 0; i != btnsLevels.size(); ++i) {
				if (btnsLevels[i]->GetText() == (*itL)) {
					TilingButton* tb = *(btnsLevels.begin() + i);
					btnsLevels.erase(btnsLevels.begin() + i);
					delete tb;

					GUIButton* gb = *(btnsDelete.begin() + i);
					btnsDelete.erase(btnsDelete.begin() + i);
					PopScreenSpace(gb);
					delete gb;

					break;
				}
			}
		}

		justDeletedLevelNames.clear();

		CalcLevelViewOffsetY();
	}

	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::MouseWheelMoved:
				levelViewOffsetY += Game->GetDeltaTime() * WHEELSCROLLSPD * itSfEvent->MouseWheel.Delta;

				CalcLevelViewOffsetY();
			break;
			default:
			break;
		}

		itSfEvent++;
	}

	for (std::vector<TilingButton*>::iterator itBtn = btnsLevels.begin(); itBtn != btnsLevels.end(); itBtn++) {
		(*itBtn)->HandleSFEvents(sfEvents);
	}

	for (std::vector<GUIButton*>::iterator itBtn = btnsDelete.begin(); itBtn != btnsDelete.end(); itBtn++) {
		(*itBtn)->HandleSFEvents(sfEvents);
	}

	btnMain->HandleSFEvents(sfEvents);
	btnUser->HandleSFEvents(sfEvents);
	btnBack->HandleSFEvents(sfEvents);
}

void MenuState_EditorLevelLoad::Update(float dT) {
}

void MenuState_EditorLevelLoad::OnClick_back () {
	pop = true;
}

void MenuState_EditorLevelLoad::ResetLevelList (std::string levListPath, bool usermaps) {
	for (std::vector<TilingButton*>::iterator itBtn = btnsLevels.begin(); itBtn != btnsLevels.end(); itBtn++) {
		delete (*itBtn);
	}
	btnsLevels.clear();

	for (std::vector<GUIButton*>::iterator itBtn = btnsDelete.begin(); itBtn != btnsDelete.end(); itBtn++) {
		PopScreenSpace(*itBtn);
		delete (*itBtn);
	}
	btnsDelete.clear();

	levelViewOffsetY = 0;

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

	int layerDepth = 50000;
	TilingButton* tb = 0;
	boost::function<void (void)> event = 0;

	CfgContents contents_levels;
	CfgParser->GetContents(levListPath, contents_levels);

	sf::Vector2f position(100, 50);

	GameState_Editor* editor = dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top());

	if (!contents_levels.Exists(""))
		return;

	std::string path = levListPath.substr( 0, levListPath.find_last_of("/") + 1 );

	for (CfgEntryMap::iterator itE = contents_levels[""].GetIteratorBegin();
			itE != contents_levels[""].GetIteratorEnd(); ++itE) {

		if ((*itE)->second != "1") {
			continue;
		}

		event = boost::bind(&GameState_Editor::LoadLevel, editor, path + (*itE)->first + ".lev");
		tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							(*itE)->first,
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							100,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::TOP,
							layerDepth);
		tb->SetPosition(position);
		btnsLevels.push_back(tb);

		if (usermaps || MyMagicNumbers::development_build) {
			event = boost::bind(&MenuState_EditorLevelLoad::InvokeDeleteLevel, this,
								path + (*itE)->first + ".lev");
			GUIButton* delBtn = new GUIButton("Content/Textures/tool_delete.png",
											"Content/Textures/tool_delete.png",
											"Content/Textures/tool_delete.png",
											event,
											"", PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
											sf::Vector2f(0, 0),
											ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP,
											layerDepth);
			delBtn->SetPosition(sf::Vector2f(-150, position.y));
			btnsDelete.push_back(delBtn);
			AddScreenSpace(delBtn);
		}

		position.y += 100;
	}

	CalcLevelViewOffsetY();
}

void MenuState_EditorLevelLoad::CalcLevelViewOffsetY () {
	float ySpaceAvailable = Game->GetRenderWindow()->GetHeight() / Game->GetWindowFakeScale() - 100;
	float ySpaceNeeded = (btnsLevels.size()) * 100 - 40;

	float minOffset = ySpaceNeeded - ySpaceAvailable;

	if (minOffset < 0) minOffset = 0;

	if (levelViewOffsetY < -minOffset)
		levelViewOffsetY = -minOffset;
	else if (levelViewOffsetY > 0)
		levelViewOffsetY = 0;

	sf::Vector2f position(100, 50);
	if (btnsDelete.size() > 0) {
		for (unsigned int i = 0; i != btnsLevels.size(); ++i) {
			btnsLevels[i]->SetPosition(sf::Vector2f(position.x, position.y + levelViewOffsetY));
			btnsDelete[i]->SetPosition(sf::Vector2f(-150, position.y + levelViewOffsetY));

			position.y += 100;
		}
	}
	else {
		for (std::vector<TilingButton*>::iterator itBtn = btnsLevels.begin();
				itBtn != btnsLevels.end(); ++itBtn) {
			(*itBtn)->SetPosition(
				sf::Vector2f(position.x, position.y + levelViewOffsetY));

			position.y += 100;
		}
	}
}

void MenuState_EditorLevelLoad::InvokeDeleteLevel (std::string levelPath) {
	dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top())->DeleteLevel(levelPath);

	unsigned int index = levelPath.find_last_of("/\\");
	std::string lLevelName = levelPath.substr(index + 1, levelPath.length() - index - 5);
	justDeletedLevelNames.push_back(lLevelName);
}
