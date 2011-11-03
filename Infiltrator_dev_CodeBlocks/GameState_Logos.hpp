#ifndef GAMESTATE_LOGOS_HPP
#define GAMESTATE_LOGOS_HPP

#include "GameState.hpp"

#include "WorldSprite.hpp"
#include "WorldString.hpp"
#include "ScreenString.hpp"
#include "ScreenSprite.hpp"

#include "GUIButton.hpp"
#include "GUIToggleButton.hpp"
#include "GUITextfield.hpp"

#include "WorldAnimatedSprite.hpp"

class Player;

class GameState_Logos : public GameState {
private:
	WorldSprite* mWorldSprite1;
	WorldSprite* mWorldSprite2;

	WorldString* mWorldString1;

	ScreenString* mScreenString1;

	ScreenSprite* mScreenSprite1;

	GUIButton* btn1;
	GUIToggleButton* toggleBtn1;
	GUITextfield* textField1;

	WorldAnimatedSprite* mWAS;

	Player* player;

	unsigned int itSfVideoMode;

	void OnClick_btn1 ();

public:
	GameState_Logos ();

	~GameState_Logos ();

	virtual void HandleEvents(std::list<sf::Event>& sfEvents);

	void Enter ();

	void Update (float dT);

	void Exit ();
};

#endif // GAMESTATE_LOGOS_HPP
