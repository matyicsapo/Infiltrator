#ifndef EDITORLEVELINFOBAR_HPP
#define EDITORLEVELINFOBAR_HPP

#include "PFrameWork/SFMLEventHandler.hpp"
#include "PFrameWork/GO/GameObject.hpp"
#include <SFML/Graphics.hpp>
#include "PFrameWork/DrawManager.hpp"
#include "PFrameWork/ConfigParser.hpp"
#include "GameStates/GameState_Editor.hpp"

class TilingScreenSprite;
class ScreenString;
class GUITextfield;
class ScreenSprite;
class VisualItemSlot;
class TilingButton;
class GUIToggleButton;
class FullScreenEventCatcherTint;
class GUIButton;

class EditorLevelInfobar : public GameObject, public SFMLEventHandler, public DrawManager {
protected:
	ScreenString* levelNameLabel;
	GUITextfield* levelNameTextField;

	ScreenString* labelLevelSize;
	ScreenString* labelLevelSizeX;
	ScreenString* labelLevelSizeY;
	GUITextfield* levelSizeXTextField;
	GUITextfield* levelSizeYTextField;

	ScreenString* labelObjectives;
	ScreenString* label1stObjective;
	ScreenString* label2ndObjective;

	VisualItemSlot* slot1stObjective;
	VisualItemSlot* slot2ndObjective;

	ScreenString* labelNavMap;
	ScreenString* labelNavMapState;
	TilingButton* generateNavMapBtn;
	ScreenString* labelToggleShowNavMap;
	GUIToggleButton* toggleShowNavMap;

	GUIButton* hideBtn;

	TilingScreenSprite* background;

	FullScreenEventCatcherTint* fullScrTint;

	ScreenSprite* objectiveDragee;
	unsigned int objectiveID;

	bool hover;

	int layerDepth;

	bool hidden;

	GameState_Editor::NavMapState navMapState;

	void OnClick_GenerateNavMap ();
	void OnToggleShowNavMap (bool show);

	virtual sf::Vector2f CalcFakePos () { return sf::Vector2f(0, 0); }

	virtual void SetFakeScale (sf::Vector2f fakeScale) {}
	virtual void SetFakePos (sf::Vector2f fakePos) {}

public:
	EditorLevelInfobar (int layerDepth);

	~EditorLevelInfobar ();

	void Update (float dT);

	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents);

	virtual void OnLanguageChanged (CfgContents& contents_lang);

	std::string GetLevelName ();
	void SetLevelName (std::string levelName);

	sf::FloatRect GetScreenBounds ();

	void SetTextLevelSize (sf::Vector2f newLevelSize);
	sf::Vector2f GetLevelSize ();
	void ApplyLevelSize ();

	void SetNavMapState (GameState_Editor::NavMapState navMapState);
	bool IsShowNavMap ();

	void ResetGUIElements ();
	bool IsAnyTextFieldActive ();

	void SetObjectiveDragee (unsigned int objectiveType, unsigned int ID, sf::Vector2f pos);
	VisualItemSlot* Get1stObjectiveSlot () {
		return slot1stObjective;
	}
	VisualItemSlot* Get2ndObjectiveSlot () {
		return slot2ndObjective;
	}

	virtual void Move (sf::Vector2f offset);

	void SetHidden (bool hidden);
	bool IsHidden ();
	void ToggleHidden ();

	virtual void SetRotation (float rotation) {}
	virtual float GetRotation () { return 0; }
	virtual void Rotate (float angle) {}
};

#endif // EDITORLEVELINFOBAR_HPP
