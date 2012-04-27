#ifndef MENUSTATE_MAINMENU_HPP
#define MENUSTATE_MAINMENU_HPP

#include "../PFrameWork/State.hpp"

#include <boost/function.hpp>

class TilingButton;
class FullScreenEventCatcherTint;
class ScreenSprite;

class MenuState_MainMenu : public MenuState {
private:
	boost::function<void (GameState*)> changeGameState;

	std::vector<TilingButton*> buttons;

	ScreenSprite* gameLogo;

	FullScreenEventCatcherTint* fullScrTint;

	void OnClick_menu_main_play ();
	void OnClick_menu_main_editor ();
	void OnClick_menu_main_options ();
	void OnClick_menu_main_credits ();
	void OnClick_menu_main_exit ();

public:
	MenuState_MainMenu (boost::function<void (GameState*)> changeGameState);

	~MenuState_MainMenu ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);

	virtual void OnLanguageChanged (CfgContents& contents_lang);
};

#endif // MENUSTATE_MAINMENU_HPP
