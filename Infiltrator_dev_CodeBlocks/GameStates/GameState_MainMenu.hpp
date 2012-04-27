#ifndef GAMESTATE_MAINMENU_HPP
#define GAMESTATE_MAINMENU_HPP

#include "../PFrameWork/State.hpp"

class MenuStateMachine;
class DarkFader;
class GroundSprite;
template<class T> class StateMachine;

class GameState_MainMenu : public GameState {
private:
	StateMachine<MenuState>* mMenuStateMachine;
	MenuState* plusMenuStateToPush;

	DarkFader* inoutFader;

	GroundSprite* groundSprite;

	void OnFinishFadeLight ();

	void DoChangeGameState (GameState* gameState);

public:
	GameState_MainMenu (MenuState* plusMenuStateToPush = 0);

	~GameState_MainMenu ();

	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents);

	virtual void Init ();

	virtual void Update (float dT);

	virtual void DrawScreenSpace (sf::RenderWindow& rwin);

	virtual void OnLanguageChanged (CfgContents& contents_lang);

	void ScheduleChangeGameState (GameState* state);
	StateMachine<MenuState>* GetMenuStateMachine ();

	void Login ();
};

#endif // GAMESTATE_MAINMENU_HPP
