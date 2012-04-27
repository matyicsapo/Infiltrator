#ifndef GAMESTATE_EDITOR_HPP
#define GAMESTATE_EDITOR_HPP

#include "GameState_BasePlayable.hpp"
#include "../AI/Navigation.hpp"

class EditorToolbar;
class EditorMenubar;
class EditorLevelInfobar;
class WorldSprite;
class WorldShape;
class ScreenSprite;
class Guard;

class GameState_Editor : public GameState_BasePlayable {
public:
	enum NavMapState { NONE = 1, VALID = 2, CHANGED = 3 };

	GameState_Editor (std::string levelPath);
	virtual ~GameState_Editor ();

	virtual void Init ();
	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);
	virtual void Update (float dT);
	virtual void DrawScreenSpace (sf::RenderWindow& rwin);

	virtual void SetLevelSize (sf::Vector2f levelSize);
	sf::Vector2f GetLevelSize ();
	void ShowNavMap (bool show);
	void InvokeGenerateNavMap ();
	void SetTesting (bool testing);
	virtual void LoadLevel (std::string levelPath);
	void UpdateLevelName ();
	void SetObjective (unsigned int index, BaseObjective* obj);
	virtual void SaveLevel (std::string levelPath, std::string levlistFilePath);
	void InvokSaveLevel ();
	virtual void OnLanguageChanged (CfgContents& contents_lang);
	void DeleteLevel (std::string levelPath);

private:
	NavMap* testtempNavMap;

	std::string originalLevelName;

	EditorToolbar* toolBar;
	EditorMenubar* menuBar;
	EditorLevelInfobar* levelInfobar;

	WorldSprite* placeElement;
	WorldDrawable* dragee;
	ScreenSprite* screenDragee;
	sf::Vector2f originalDrageePos;
	sf::Vector2f lastMousePosInWorld;
	WorldDrawable* selected;
	WorldDrawable* selectedWayPoint;

	int activeToolID;

	bool testing;

	std::deque<WorldShape*> navMapDebugNodes;
	NavMapState navMapState;

	virtual void OnPause (bool pause);
	virtual void OnFinishedNavMapGen (bool successful);
	virtual void OnLevelLoaded (bool successful);
	bool IsInLevelBounds (const sf::Vector2f& position);
	void OnNavMapStateChanged (NavMapState navMapState);
	void OnGuardSelected (Guard* guard);
	void OnToolActivated (int ID);
	void OnMenuActivated (int ID);
	void OnClick_menuButtonWalls ();
	void OnClick_menuButtonGround ();
	void OnClick_menuButtonCharacters ();
	void OnClick_menuButtonLevelInfo ();
	bool TryDeleteSelected (unsigned int entityType);
	virtual void OnGameFinished (bool successful);
};

#endif // GAMESTATE_EDITOR_HPP
