#ifndef MENUSTATE_LANGUAGE_HPP
#define MENUSTATE_LANGUAGE_HPP

#include <vector>

#include "../PFrameWork/State.hpp"

class GUIButton;
class DarkFader;
class FullScreenEventCatcherTint;

class MenuState_Language : public MenuState {
private:
	FullScreenEventCatcherTint* background;

	DarkFader* inoutFader;

	std::vector<GUIButton*> langBtns;

	void SetLanguage (std::string lang);
	void FadeOut (std::string lang);

public:
	MenuState_Language ();

	~MenuState_Language ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);
};

#endif // MENUSTATE_LANGUAGE_HPP
