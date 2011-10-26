#ifndef SFMLGAMEMANAGER_HPP
#define SFMLGAMEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <list>

#include "GameStateMachine.hpp"
#include "Camera2D.hpp"

class SFMLGameManager {
private:
	GameStateMachine* mGameStateMachine;

	sf::RenderWindow win;

	Camera2D worldCamera2D;

	std::list<sf::Event> sfEvents;

	bool fullScreen;

	bool reset;

	bool bClear;
	sf::Color clearColor;

	SFMLGameManager ();

	~SFMLGameManager () { delete mGameStateMachine; }

	SFMLGameManager (SFMLGameManager const&);
	SFMLGameManager& operator= (SFMLGameManager const&);

public:
	static SFMLGameManager* Instance ();

	void Create (unsigned int width, unsigned int height, bool enable);
	void Create (unsigned int width, unsigned int height);

	int Run ();

	void SetGameStateMachine (GameStateMachine* newGameStateMachine) { mGameStateMachine = newGameStateMachine; }

	GameStateMachine* GetGameStateMachine () { return mGameStateMachine; }

	void SetFullScreen (bool enable) { Create(win.GetWidth(), win.GetHeight(), enable); }
	void ToggleFullScreen () { Create(win.GetWidth(), win.GetHeight(), !fullScreen); }
	bool IsFullScreen () { return fullScreen; }

	void SetClearColor (const sf::Color& clearColor) { this->clearColor = clearColor; }
		//sf::Color(clearColor.r, clearColor.g, clearColor.b, clearColor.a); }
	void SetClear (bool enable) { bClear = enable; }

	sf::RenderWindow* GetRenderWindow () { return &win; }

	Camera2D* GetWorldCamera2D () { return &worldCamera2D; }
};

#endif // SFMLGAMEMANAGER_HPP
