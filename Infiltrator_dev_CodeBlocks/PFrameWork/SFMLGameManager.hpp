#ifndef SFMLGAMEMANAGER_HPP
#define SFMLGAMEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <list>

#include "WorldCamera2D.hpp"

class GameState;
template<class T> class StateMachine;

class SFMLGameManager {
private:
	float deltaTime;
	float windowFakeScale;

	StateMachine<GameState>* mGameStateMachine;

	sf::RenderWindow win;

	WorldCamera2D mWorldCamera2D;

	std::list<sf::Event> sfEvents;

	bool fullScreen;

	bool reset;

	bool bClear;
	sf::Color clearColor;

	int sfVideoModesCount;

	SFMLGameManager ();

	~SFMLGameManager ();

	SFMLGameManager (SFMLGameManager const&);
	SFMLGameManager& operator= (SFMLGameManager const&);

	void OnResize ();

public:
	static SFMLGameManager* Instance ();

	void Create (unsigned int width, unsigned int height, bool enable);
	void Create (unsigned int width, unsigned int height);

	int Run ();

	void SetGameStateMachine (StateMachine<GameState>* newGameStateMachine);

	StateMachine<GameState>* GetGameStateMachine ();

	void SetFullScreen (bool enable) { Create(win.GetWidth(), win.GetHeight(), enable); }
	void ToggleFullScreen () { Create(win.GetWidth(), win.GetHeight(), !fullScreen); }
	bool IsFullScreen () { return fullScreen; }

	void SetClearColor (const sf::Color& clearColor) { this->clearColor = clearColor; }
		//sf::Color(clearColor.r, clearColor.g, clearColor.b, clearColor.a); }
	void SetClear (bool enable) { bClear = enable; }

	sf::RenderWindow* GetRenderWindow () { return &win; }

	WorldCamera2D* GetWorldCamera2D () { return &mWorldCamera2D; }

	sf::Vector2f ConvertCoordsWindowToWorld (sf::Vector2f windowSpaceCoords);
	sf::Vector2f ConvertCoordsWindowToWorld (float windowSpaceX, float windowSpaceY);

	float GetDeltaTime () { return deltaTime; }
	float GetWindowFakeScale () { return windowFakeScale; }
	int GetSFVideoModesCount () { return sfVideoModesCount; }
};

#define Game SFMLGameManager::Instance()

#endif // SFMLGAMEMANAGER_HPP
