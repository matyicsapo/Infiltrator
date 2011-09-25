#ifndef SFMLGAMEMANAGER_HPP
#define SFMLGAMEMANAGER_HPP

#include <SFML/Graphics.hpp>

#include "GameStateMachine.hpp"

class SFMLGameManager {
	GameStateMachine* pGameStateMachine;

	sf::RenderWindow mainWindow;

	bool bClear;
	sf::Color clearColor;

	// constructor is private
	SFMLGameManager ();

	// strictly speaking, the destructor of a singleton should be private
	// but some compilers have problems with this
	~SFMLGameManager () {
		delete pGameStateMachine; pGameStateMachine = 0;
	}

	//copy ctor and assignment should be private
	SFMLGameManager (SFMLGameManager const&);
	SFMLGameManager& operator= (SFMLGameManager const&);

public:
	static SFMLGameManager* Instance ();

	void Create ();

	int Run ();

	void SetGameStateMachine (GameStateMachine* pNewGameStateMachine) {
		pGameStateMachine = pNewGameStateMachine;
	}

	void SetClearColor (const sf::Color& clearColor) { this->clearColor = sf::Color(clearColor.r, clearColor.g, clearColor.b,
																					clearColor.a); }
	void SetClear (bool enable) { bClear = enable; }

	sf::RenderWindow* GetRenderWindow () { return &mainWindow; }
};

#endif
