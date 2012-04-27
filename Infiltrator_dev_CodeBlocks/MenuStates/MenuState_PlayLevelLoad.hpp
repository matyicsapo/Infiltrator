#ifndef MENUSTATE_PLAYLEVELLOAD_HPP
#define MENUSTATE_PLAYLEVELLOAD_HPP

#include "../PFrameWork/State.hpp"

#include <boost/function.hpp>

class TilingButton;
class FullScreenEventCatcherTint;

class MenuState_PlayLevelLoad : public MenuState {
private:
	float levelViewOffsetY;

	TilingButton* btnMain;
	TilingButton* btnUser;
	TilingButton* btnBack;

	std::vector<TilingButton*> btnsLevels;

	FullScreenEventCatcherTint* fullScrTint;

	void OnClick_back ();

	void ResetLevelList (std::string levListPath);

	void CalcLevelViewOffsetY ();

	void StartLevel (std::string levelPath);

public:
	MenuState_PlayLevelLoad ();

	~MenuState_PlayLevelLoad ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);
};

#endif // MENUSTATE_PLAYLEVELLOAD_HPP
