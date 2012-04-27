#ifndef MENUSTATE_EDITORPAUSE_HPP
#define MENUSTATE_EDITORPAUSE_HPP

#include "../PFrameWork/State.hpp"

#include <boost/function.hpp>

class TilingButton;
class FullScreenEventCatcherTint;
class DarkFader;

class MenuState_EditorPause : public MenuState {
private:
	DarkFader* darkFader;

	std::vector<TilingButton*> buttons;

	FullScreenEventCatcherTint* fullScrTint;

	void OnClick_menu_editorpause_edit ();
	void OnClick_menu_editorpause_test ();
	void OnClick_menu_editorpause_save ();
	void OnClick_menu_editorpause_load ();
	void OnClick_menu_editorpause_mainmenu ();

	void OnClick_menu_options ();

	void ExitToMainMenu ();

public:
	MenuState_EditorPause ();

	~MenuState_EditorPause ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);

	virtual void OnLanguageChanged (CfgContents& contents_lang);
};

#endif // MENUSTATE_EDITORPAUSE_HPP
