#ifndef GAMESTATE_BASEPLAYABLE_HPP
#define GAMESTATE_BASEPLAYABLE_HPP

#include "../PFrameWork/State.hpp"
#include "../AI/Navigation.hpp"
#include "../Objective.hpp"
#include "../proto/InfiltratorLevel.pb.h"

template<class T> class StateMachine;
class InvisibleRectCollider;
class DarkFader;
class ScreenTimeAnimatedSprite;
class ScreenString;
class TilingButton;
class GroundSprite;
class WorldShape;
class Player;
class Princess;
class Goal;
class Coin;
class Chest;
class WallRectangle;
class GameplayHUD;
class WorldSprite;

class GameState_BasePlayable : public GameState {
private:
	sf::Thread* navMapGenThread;
	ThreadState_GenerateNavMap navMapGenThreadState;

	ScreenTimeAnimatedSprite* loadingIcon;
	ScreenString* loadingText;
	TilingButton* cancelNavMapGenBtn;

	GroundSprite* groundSprite;
	WorldShape* levelBoundsDebug;
	InvisibleRectCollider** levelBoundColliders;

	void DeleteAll (unsigned int entityType);
	Infiltrator::Level_WorldObjType ConvertType (unsigned int typeID);
	void SaveStuff (unsigned int entityType, Infiltrator::Level& level);
	void SaveStuff (GameObject* entity, Infiltrator::Level& level);
	void OnObjectiveCompleted (unsigned int objectiveIndex);

protected:
	bool finished;

	GameplayHUD* HUD;
	WorldSprite* interacteeHighlight;

	int goldPickedUp;

	BaseObjective* objectives[2];

	Player* player;
	Princess* princess;
//	Goal* goal;
	Chest* chest;

	std::string levelPath;
	std::string levelName;
	sf::Vector2f levelSize;

	StateMachine<MenuState>* mMenuStateMachine;

	DarkFader* inoutFader;

	void OnFinishFadeLight ();
	void UpdateGamePlay (float dT);

	virtual void OnPause (bool pause);
	virtual void OnFinishedNavMapGen (bool successful);
	virtual void OnLevelLoaded (bool successful) = 0;
	void ClearMap ();
	void DoGenerateNavMap ();
	void StartPlaying ();
	virtual void OnGameFinished (bool successful);

public:
	GameState_BasePlayable (std::string levelPath);
	virtual ~GameState_BasePlayable ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);
	virtual void Init ();
	virtual void Update (float dT);
	virtual void DrawScreenSpace (sf::RenderWindow& rwin);
	StateMachine<MenuState>* GetMenuStateMachine ();
	InvisibleRectCollider** GetLevelBoundColliders ();
	virtual void LoadLevel (std::string levelPath);
	virtual void SetLevelSize (sf::Vector2f levelSize);
	void PickedUpGold (int gold, WorldDrawable* thing);
	virtual void SaveLevel (std::string levelPath, std::string levlistFilePath);
	virtual void OnLanguageChanged (CfgContents& contents_lang);
	void EnteredGoal (Goal* goal);
	int GetGoldPickedUp () { return goldPickedUp; }
};

#endif // GAMESTATE_BASEPLAYABLE_HPP
