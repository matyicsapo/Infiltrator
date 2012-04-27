#ifndef MENUSTATE_STARTLOGIN_HPP
#define MENUSTATE_STARTLOGIN_HPP

#include "../PFrameWork/State.hpp"

class TilingButton;
class FullScreenEventCatcherTint;
class ScreenSprite;
class ScreenString;
class GUITextfield;
class GUIButton;
class DarkFader;

class MenuState_StartLogin : public MenuState {
private:
	bool startup;

	std::string* profileNameArray;
	int profileNameArraySize;
	int currProfileIndex;
	std::string lastProfileName;

	DarkFader* langFader;

	ScreenSprite* gameLogo;

	FullScreenEventCatcherTint* fullScrTint;
	TilingButton* guiBox;

	ScreenString* profileLabel;
	GUIButton* profileLeftArrowBtn;
	GUIButton* profileRightArrowBtn;
	GUITextfield* profileTextField;

	ScreenString* languageLabel;
	TilingButton* languageBtn;

	TilingButton* exitBtn;

	TilingButton* playBtn;

	void Login ();

	void FadeToLanguageSelection ();
	void LanguageSelection ();
	void OnClick_ChangeProfile (int change);
	void OnClick_Baxit (); // back or exit..
//	void OnApply_ProfileChanged ();

public:
	MenuState_StartLogin (bool startup);

	~MenuState_StartLogin ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);

	virtual void OnLanguageChanged (CfgContents& contents_lang);
};

#endif // MENUSTATE_STARTLOGIN_HPP
