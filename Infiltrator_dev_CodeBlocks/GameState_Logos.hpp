#ifndef GAMESTATE_LOGOS_HPP
#define GAMESTATE_LOGOS_HPP

#include "PFrameWork/State/GameState.hpp"

class WorldSprite;
class WorldString;
class ScreenString;
class ScreenSprite;

class GUIButton;
class GUIToggleButton;
class GUITextfield;

class TimeAnimatedSprite;

class Player;
class Wall;

class GameState_Logos : public GameState {
private:
	WorldSprite* mWorldSprite1;
	WorldSprite* mWorldSprite2;

	WorldString* mWorldString1;

	ScreenString* mScreenString1;

	ScreenSprite* mScreenSprite1;

	ScreenSprite* mCrosshair;

	GUIButton* btn1;
	GUIToggleButton* toggleBtn1;
	GUITextfield* textField1;

	TimeAnimatedSprite* fireClip;

	Player* player;
Player* player_collisiontest;

	Wall* wall1;

	unsigned int itSfVideoMode;

	void OnClick_btn1 ();

public:
	GameState_Logos ();

	~GameState_Logos ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	void Enter ();

	void Update (float dT);

	void Exit ();
};

#endif // GAMESTATE_LOGOS_HPP
