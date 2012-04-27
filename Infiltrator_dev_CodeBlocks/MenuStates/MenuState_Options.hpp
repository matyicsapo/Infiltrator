#ifndef MENUSTATE_OPTIONS_HPP
#define MENUSTATE_OPTIONS_HPP

#include "../PFrameWork/State.hpp"
#include <boost/function.hpp>

class TilingButton;
class ScreenString;
class GUISlider;
class GUIToggleButton;
class GUIButton;
class FullScreenEventCatcherTint;
class DarkFader;

class MenuState_Options : public MenuState {
private:
	DarkFader* langFader;
	std::vector<ScreenString*> labels;
	std::vector<TilingButton*> buttons;
	ScreenString* label_currresolution;
	GUISlider* slider_vol_master;
	GUISlider* slider_vol_music;
	GUISlider* slider_vol_effect;
	GUIButton* resolutionLeftArrowBtn;
	GUIButton* resolutionRightArrowBtn;
	GUIToggleButton* fullScrGUIIToggle;

	FullScreenEventCatcherTint* fullScrTint;

	int itSfVideoMode;

	void OnClick_profiles ();
	void OnClick_language ();
	void LanguageSelection ();

	void OnClick_resolution (int change);

	void OnClick_back ();
//	void OnClick_revert ();
//	void OnClick_apply ();

	void OnToggleFullScr (bool enable);

public:
	MenuState_Options ();

	~MenuState_Options ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);

	virtual void OnLanguageChanged (CfgContents& contents_lang);
};

#endif // MENUSTATE_OPTIONS_HPP
