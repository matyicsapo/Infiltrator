#include "MenuState_Options.hpp"

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "../TilingButton.hpp"
#include "../PFrameWork/ConfigParser.hpp"
#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/SoundManager.hpp"
#include "../PFrameWork/DrawManager.hpp"
#include "../PFrameWork/GO/ScreenString.hpp"
#include "../GUISlider.hpp"
#include "../PFrameWork/GO/GUI/GUIToggleButton.hpp"
#include "../PFrameWork/GO/GUI/GUIButton.hpp"
#include "../GameStates/GameState_MainMenu.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "MenuState_MainMenu.hpp"
#include "MenuState_Language.hpp"
#include "MenuState_StartLogin.hpp"
#include "../FullScreenEventCatcherTint.hpp"
#include "../PFrameWork/PFWConstants.hpp"
#include "../Session.hpp"
#include "../DarkFader.hpp"
#include "../GameStates/GameState_Editor.hpp"

MenuState_Options::MenuState_Options () : langFader(0) {
}

void MenuState_Options::Init () {
	MenuState::Init();

	int layerDepth = 50000;

	CfgContents contents_lang;
	CfgParser->GetContents(std::string("Content/LanguageFiles/") + CurrentSession->language
		+ std::string(".ini"), contents_lang);

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

	ScreenString* label = 0;
	boost::function<void (void)> event = 0;
	TilingButton* tb = 0;

	const int x_h1 = -350;
	const int x_h2 = -300;
	const int x_h3 = -200;
	const int x_h4 = 0;

	const int y_limit = 350;
	const int y_h1 = 55;
	const int y_h2 = 65;
	const int y_else = 70;
	const int y_slidersmallest = 40;
	int y_curr_offset;

	const int sliderWidth = 300;

	y_curr_offset = -y_limit;
	label = new ScreenString(contents_lang["menu_options"]["caption"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h1, y_curr_offset));
	labels.push_back(label);
	label->SetScale(1.2f);

	y_curr_offset += y_h1;
	label = new ScreenString(contents_lang["misc"]["caption_profile"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h3, y_curr_offset));
	labels.push_back(label);

	event = boost::bind(&MenuState_Options::OnClick_profiles, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							CurrentSession->profileName,
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							100,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(x_h4, y_curr_offset));
	buttons.push_back(tb);

	y_curr_offset += y_else;
	label = new ScreenString(contents_lang["misc"]["caption_language"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h3, y_curr_offset));
	labels.push_back(label);

	event = boost::bind(&MenuState_Options::OnClick_language, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang[""]["lang"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(x_h4, y_curr_offset));
	buttons.push_back(tb);

	y_curr_offset += y_h2;
	label = new ScreenString(contents_lang["menu_options"]["caption_graphics"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h2, y_curr_offset));
	labels.push_back(label);

	y_curr_offset += y_h1;
	label = new ScreenString(contents_lang["menu_options"]["caption_resolution"],
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h3, y_curr_offset));
	labels.push_back(label);

	event = boost::bind(&MenuState_Options::OnClick_resolution, this, -1);
	resolutionLeftArrowBtn = new GUIButton("Content/Textures/GUI/darkarrowbutton.png",
								"Content/Textures/GUI/arrowbutton.png",
								"Content/Textures/GUI/pressedarrowbutton.png",
								event,
								"", PFWConstants::defaultFontFile, PFWConstants::defaultFontSize, sf::Vector2f(0, 0),
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	AddScreenSpace(resolutionLeftArrowBtn);
	resolutionLeftArrowBtn->SetPosition(sf::Vector2f(x_h4, y_curr_offset));

	{
		unsigned int w = Game->GetRenderWindow()->GetWidth();
		unsigned int h = Game->GetRenderWindow()->GetHeight();

		for (int i = 0; i < Game->GetSFVideoModesCount(); i++) {
			if (sf::VideoMode::GetMode(i).Width == w
						&&
					sf::VideoMode::GetMode(i).Height == h) {
				itSfVideoMode = i;
				break;
			}
		}

		std::stringstream ss;
		std::string s;
		ss << Game->GetRenderWindow()->GetWidth() << "x" << Game->GetRenderWindow()->GetHeight();
		ss >> s;
		label_currresolution = new ScreenString(s,
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(label_currresolution);
		label_currresolution->SetPosition(
			sf::Vector2f(x_h4 + resolutionLeftArrowBtn->GetScreenSize().x * 2, y_curr_offset));
	}

	event = boost::bind(&MenuState_Options::OnClick_resolution, this, 1);
	resolutionRightArrowBtn = new GUIButton("Content/Textures/GUI/darkarrowbutton.png",
								"Content/Textures/GUI/arrowbutton.png",
								"Content/Textures/GUI/pressedarrowbutton.png",
								event,
								"", PFWConstants::defaultFontFile, PFWConstants::defaultFontSize, sf::Vector2f(0, 0),
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
	AddScreenSpace(resolutionRightArrowBtn);
	resolutionRightArrowBtn->SetPosition(sf::Vector2f(
		x_h4 + resolutionLeftArrowBtn->GetScreenSize().x * 2 + label->GetScreenSize().x,
		y_curr_offset));
	resolutionRightArrowBtn->Rotate(180);

	y_curr_offset += y_else;
	label = new ScreenString(contents_lang["menu_options"]["caption_fullscr"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h3, y_curr_offset));
	labels.push_back(label);

	{
		boost::function <void (void)> activate = boost::bind(&MenuState_Options::OnToggleFullScr, this, true);
		boost::function <void (void)> deactivate = boost::bind(&MenuState_Options::OnToggleFullScr, this, false);
		fullScrGUIIToggle = new GUIToggleButton("Content/Textures/GUI/toggle_inactive_normal.png",
								"Content/Textures/GUI/toggle_inactive_hover.png",
								"Content/Textures/GUI/toggle_active_normal.png",
								"Content/Textures/GUI/toggle_active_hover.png",
								activate,
								deactivate,
								"",
								PFWConstants::defaultFontFile,
								PFWConstants::defaultFontSize,
								sf::Vector2f(0, 0),
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
								layerDepth);
		AddScreenSpace(fullScrGUIIToggle);
		fullScrGUIIToggle->SetPosition(sf::Vector2f(x_h4, y_curr_offset));
		fullScrGUIIToggle->SetActive(Game->IsFullScreen());
	}

	y_curr_offset += y_h2;
	label = new ScreenString(contents_lang["menu_options"]["caption_audio"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h2, y_curr_offset));
	labels.push_back(label);

	y_curr_offset += y_h1;
	label = new ScreenString(contents_lang["menu_options"]["caption_vol_master"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h3, y_curr_offset));
	labels.push_back(label);

	slider_vol_master = new GUISlider(this, Sounds->GetVol_Master(),
							sliderWidth,
							"Content/Textures/GUI/sliderbarleft.png",
							"Content/Textures/GUI/sliderbarmiddle.png",
							"Content/Textures/GUI/sliderbarright.png",
							"Content/Textures/GUI/sliderbarthingy.png",
							boost::bind(&SoundManager::SetVol_Master, Sounds, _1),
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	slider_vol_master->SetPosition(sf::Vector2f(x_h4 + sliderWidth / 2, y_curr_offset));

	y_curr_offset += y_slidersmallest;
	label = new ScreenString(contents_lang["menu_options"]["caption_vol_music"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h3, y_curr_offset));
	labels.push_back(label);

	slider_vol_music = new GUISlider(this, Sounds->GetVol_Music(),
							sliderWidth,
							"Content/Textures/GUI/sliderbarleft.png",
							"Content/Textures/GUI/sliderbarmiddle.png",
							"Content/Textures/GUI/sliderbarright.png",
							"Content/Textures/GUI/sliderbarthingy.png",
							boost::bind(&SoundManager::SetVol_Music, Sounds, _1),
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	slider_vol_music->SetPosition(sf::Vector2f(x_h4 + sliderWidth / 2, y_curr_offset));

	y_curr_offset += y_slidersmallest;
	label = new ScreenString(contents_lang["menu_options"]["caption_vol_effect"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	AddScreenSpace(label);
	label->SetPosition(sf::Vector2f(x_h3, y_curr_offset));
	labels.push_back(label);

	slider_vol_effect = new GUISlider(this, Sounds->GetVol_Effect(),
							sliderWidth,
							"Content/Textures/GUI/sliderbarleft.png",
							"Content/Textures/GUI/sliderbarmiddle.png",
							"Content/Textures/GUI/sliderbarright.png",
							"Content/Textures/GUI/sliderbarthingy.png",
							boost::bind(&SoundManager::SetVol_Effect, Sounds, _1),
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	slider_vol_effect->SetPosition(sf::Vector2f(x_h4 + sliderWidth / 2, y_curr_offset));

	y_curr_offset = y_limit - 50;
	event = boost::bind(&MenuState_Options::OnClick_back, this);
	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["menu"]["back"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
							layerDepth);
	tb->SetPosition(sf::Vector2f(-420, y_curr_offset));
	buttons.push_back(tb);

//	event = boost::bind(&MenuState_Options::OnClick_revert, this);
//	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
//							event,
//							contents_lang["menu_options"]["revert"],
//							PFWConstants::defaultFontFile,
//							PFWConstants::defaultFontSize,
//							sf::Vector2f(0, 0),
//							-1,
//							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
//							layerDepth);
//	tb->SetPosition(sf::Vector2f(0, y_curr_offset));
//	buttons.push_back(tb);

//	event = boost::bind(&MenuState_Options::OnClick_apply, this);
//	tb = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
//							event,
//							contents_lang["menu_options"]["apply"],
//							PFWConstants::defaultFontFile,
//							PFWConstants::defaultFontSize,
//							sf::Vector2f(0, 0),
//							-1,
//							ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE,
//							layerDepth);
//	tb->SetPosition(sf::Vector2f(220, y_curr_offset));
//	buttons.push_back(tb);

	fullScrTint = new FullScreenEventCatcherTint(layerDepth - 1000);
	AddScreenSpace(fullScrTint);

	SortAscendingScreen();
}

MenuState_Options::~MenuState_Options () {
	for (std::vector<TilingButton*>::iterator itBtn = buttons.begin(); itBtn != buttons.end(); itBtn++) {
		delete (*itBtn);
	}

	for (std::vector<ScreenString*>::iterator itLabel = labels.begin(); itLabel != labels.end(); itLabel++) {
		delete (*itLabel);
	}

	PopScreenSpace(label_currresolution);
	delete label_currresolution;

	PopScreenSpace(resolutionLeftArrowBtn);
	delete resolutionLeftArrowBtn;
	PopScreenSpace(resolutionRightArrowBtn);
	delete resolutionRightArrowBtn;

	delete slider_vol_master;
	delete slider_vol_music;
	delete slider_vol_effect;

	PopScreenSpace(fullScrGUIIToggle);
	delete fullScrGUIIToggle;

	PopScreenSpace(fullScrTint);
	delete fullScrTint;

	PopScreenSpace(langFader);
	delete langFader;
}

void MenuState_Options::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	if (langFader)
		return;

	for (std::vector<TilingButton*>::iterator itBtn = buttons.begin(); itBtn != buttons.end(); itBtn++) {
		(*itBtn)->HandleSFEvents(sfEvents);
	}

	resolutionLeftArrowBtn->HandleSFEvents(sfEvents);
	resolutionRightArrowBtn->HandleSFEvents(sfEvents);

	slider_vol_master->HandleSFEvents(sfEvents);
	slider_vol_music->HandleSFEvents(sfEvents);
	slider_vol_effect->HandleSFEvents(sfEvents);

	fullScrGUIIToggle->HandleSFEvents(sfEvents);
}

void MenuState_Options::Update(float dT) {
	if (langFader) {
		if (langFader->Finished()) {
			PopScreenSpace(langFader);
			delete langFader;
			langFader = 0;
		}
		else langFader->Update(dT);
	}
}

void MenuState_Options::OnClick_profiles () {
	CurrentSession->SaveProfile();

	Game->GetGameStateMachine()->Top()->GetMenuStateMachine()->Push(new MenuState_StartLogin(false));
}

void MenuState_Options::OnClick_language () {
	boost::function<void (void)>
	onFinishFadeDark = boost::bind(&MenuState_Options::LanguageSelection, this);

	langFader = new DarkFader(.25f, true, onFinishFadeDark, 5000);
	AddScreenSpace(langFader);
}

void MenuState_Options::LanguageSelection () {
	dynamic_cast<GameState*>(Game->GetGameStateMachine()->Top())->GetMenuStateMachine()->Push(new MenuState_Language());
}

void MenuState_Options::OnClick_resolution (int change) {
	if (Game->IsFullScreen()) {
		// try to skip non monitor aspect ratio fullscreen resolutions
		// base on the "assumption" that the 0th mode as being the best is of the monitor's
			// aspect ratio

		unsigned int initial_itSfVideoMode = itSfVideoMode;

		bool cool = false;

		float b = (float)sf::VideoMode::GetMode(0).Width / (float)sf::VideoMode::GetMode(0).Height;

		// Should never end up in an infinite loop
		do {
			itSfVideoMode += change;

			if ( itSfVideoMode > Game->GetSFVideoModesCount() ) {
				itSfVideoMode = 0;
			}
			else
			if ( itSfVideoMode < 0 ) {
				itSfVideoMode = Game->GetSFVideoModesCount() - 1;
			}

//std::cout << sf::VideoMode::GetMode(itSfVideoMode).Width << " "
//	<< sf::VideoMode::GetMode(itSfVideoMode).Height << std::endl;

			float a = (float)sf::VideoMode::GetMode(itSfVideoMode).Width
				/ (float)sf::VideoMode::GetMode(itSfVideoMode).Height;

			if (a == b)
				cool = true;
		} while (!cool);

		if (cool && (sf::VideoMode::GetMode(initial_itSfVideoMode).Width
					!= sf::VideoMode::GetMode(itSfVideoMode).Width
				|| sf::VideoMode::GetMode(initial_itSfVideoMode).Height
					!= sf::VideoMode::GetMode(itSfVideoMode).Height)) {

//			Game->Create(sf::VideoMode::GetMode(itSfVideoMode).Width,
//						sf::VideoMode::GetMode(itSfVideoMode).Height);
		}
	}
	else {
		itSfVideoMode += change;

		if ( itSfVideoMode > Game->GetSFVideoModesCount() ) {
			itSfVideoMode = 0;
		}
		else
		if ( itSfVideoMode < 0 ) {
			itSfVideoMode = Game->GetSFVideoModesCount() - 1;
		}

		Game->Create(sf::VideoMode::GetMode(itSfVideoMode).Width,
					sf::VideoMode::GetMode(itSfVideoMode).Height);
	}

	std::stringstream ss;
	std::string s;

	ss << Game->GetRenderWindow()->GetWidth() << "x" << Game->GetRenderWindow()->GetHeight();
	ss >> s;

	label_currresolution->SetText(s);
}

void MenuState_Options::OnClick_back () {
	CurrentSession->SaveProfile();

	pop = true;
}

//void MenuState_Options::OnClick_revert () {
//}
//
//void MenuState_Options::OnClick_apply () {
//}

void MenuState_Options::OnToggleFullScr (bool enable) {
	if (enable) {
		itSfVideoMode = 0;
	}

	Game->Create(sf::VideoMode::GetMode(itSfVideoMode).Width, sf::VideoMode::GetMode(itSfVideoMode).Height, enable);

	std::stringstream ss;
	std::string s;

	ss << Game->GetRenderWindow()->GetWidth() << "x" << Game->GetRenderWindow()->GetHeight();

	ss >> s;
	label_currresolution->SetText(s);
}

void MenuState_Options::OnLanguageChanged (CfgContents& contents_lang) {
	std::string b[4][2];
	b[0][0] = "";
	b[0][1] = "lang";
	b[1][0] = "menu";
	b[1][1] = "back";
//	b[2][0] = "menu_options";
//	b[2][1] = "revert";
//	b[3][0] = "menu_options";
//	b[3][1] = "apply";
	for (unsigned int i = 1; i != buttons.size(); ++i) {
		buttons[i]->SetText( contents_lang[b[i - 1][0]][b[i - 1][1]] );
	}

	std::string l[10];
	l[0] = "caption";
//	l[1] = "caption_profile";
//	l[2] = "caption_language";
	l[3] = "caption_graphics";
	l[4] = "caption_resolution";
	l[5] = "caption_fullscr";
	l[6] = "caption_audio";
	l[7] = "caption_vol_master";
	l[8] = "caption_vol_music";
	l[9] = "caption_vol_effect";
	for (unsigned int i = 0; i != labels.size(); ++i) {
		labels[i]->SetText( contents_lang["menu_options"][l[i]] );
	}

	labels[1]->SetText(contents_lang["misc"]["caption_profile"]);
	labels[2]->SetText(contents_lang["misc"]["caption_language"]);
}
