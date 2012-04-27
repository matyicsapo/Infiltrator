#ifndef MENUSTATE_CREDITS_HPP
#define MENUSTATE_CREDITS_HPP

#include "../PFrameWork/State.hpp"

#include <boost/function.hpp>

class TilingButton;
class FullScreenEventCatcherTint;
class TilingButton;
class ScreenSprite;
class ScreenString;

class MenuState_Credits : public MenuState {
private:
	ScreenString** allTexts;

	TilingButton* guiBox;
	ScreenSprite* gameLogo;
	FullScreenEventCatcherTint* fullScrTint;

	TilingButton* backButton;

	void OnClick_back ();

public:
	MenuState_Credits ();

	~MenuState_Credits ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);
};

#endif // MENUSTATE_CREDITS_HPP
