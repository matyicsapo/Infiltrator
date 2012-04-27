#ifndef MENUSTATE_LEVELFINISH_HPP
#define MENUSTATE_LEVELFINISH_HPP

#include "../PFrameWork/State.hpp"

#include <vector>

class TilingButton;
class FullScreenEventCatcherTint;
class ScreenSprite;
class ScreenString;
class DarkFader;

class MenuState_LevelFinish : public MenuState {
private:
	bool successful;

	std::vector<ScreenString*> strings;
	std::vector<ScreenSprite*> sprites;

	TilingButton* reBtn;
	TilingButton* mmBtn;

	FullScreenEventCatcherTint* fullScrTint;
	TilingButton* guiBox;

	DarkFader* darkFader;

	void OnClick_ExitToMain ();
	void OnClick_Restart ();
	void RestartLevel ();
	void ExitPlay ();

public:
	MenuState_LevelFinish (bool successful);
	~MenuState_LevelFinish ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);
};

#endif // MENUSTATE_LEVELFINISH_HPP
