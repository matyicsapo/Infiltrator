#ifndef MENUSTATE_EDITORLEVELLOAD_HPP
#define MENUSTATE_EDITORLEVELLOAD_HPP

#include "../PFrameWork/State.hpp"

#include <boost/function.hpp>

class TilingButton;
class FullScreenEventCatcherTint;
class GUIButton;

class MenuState_EditorLevelLoad : public MenuState {
private:
	float levelViewOffsetY;

	TilingButton* btnMain;
	TilingButton* btnUser;
	TilingButton* btnBack;

	std::vector<TilingButton*> btnsLevels;
	std::vector<GUIButton*> btnsDelete;
	std::vector<std::string> justDeletedLevelNames;

	FullScreenEventCatcherTint* fullScrTint;

	void OnClick_back ();

	void ResetLevelList (std::string levListPath, bool usermaps);

	void CalcLevelViewOffsetY ();

	void InvokeDeleteLevel (std::string levelPath);

public:
	MenuState_EditorLevelLoad ();

	~MenuState_EditorLevelLoad ();

	virtual void Init ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Update(float dT);
};

#endif // MENUSTATE_EDITORLEVELLOAD_HPP
