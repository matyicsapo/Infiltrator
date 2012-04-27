#ifndef MENUSTATE_PLAYPAUSE_HPP
#define MENUSTATE_PLAYPAUSE_HPP

#include "../PFrameWork/State.hpp"

#include <boost/function.hpp>

class TilingButton;
class FullScreenEventCatcherTint;
class DarkFader;

class MenuState_PlayPause : public MenuState {
private:
	std::vector<TilingButton*> buttons;

	FullScreenEventCatcherTint* fullScrTint;

	DarkFader* darkFader;

	void OnClick_Resume ();
	void OnClick_Restart ();
	void OnClick_Options ();
	void OnClick_ExitToMain ();

	void RestartLevel ();
	void ExitPlay ();

public:
	MenuState_PlayPause ();

	~MenuState_PlayPause ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);

	virtual void OnLanguageChanged (CfgContents& contents_lang);
};

#endif // MENUSTATE_PLAYPAUSE_HPP
