#ifndef STATE_HPP
#define STATE_HPP

#include "SFMLEventHandler.hpp"
#include "DrawManager.hpp"
#include "ConfigParser.hpp"

template<class T> class StateMachine;

class State : public SFMLEventHandler, public DrawManager {
public:
	bool pop;
	bool off;

	State () : pop(false), off(true) {}

	virtual ~State () {}

	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents) = 0;

	virtual void Init ();

	virtual void Update (float dT) = 0;
};

class MenuState : public State {
public:
	virtual void OnLanguageChanged (CfgContents& contents_lang) {}
};

class GameState : public State {
public:
	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents);

	virtual void OnLanguageChanged (CfgContents& contents_lang) {}

	virtual StateMachine<MenuState>* GetMenuStateMachine () { return 0; }
};

#endif // STATE_HPP
