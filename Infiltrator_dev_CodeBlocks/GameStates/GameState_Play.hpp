#ifndef GAMESTATE_PLAY_HPP
#define GAMESTATE_PLAY_HPP

#include "GameState_BasePlayable.hpp"

class GameState_Play : public GameState_BasePlayable {
private:
	virtual void OnPause (bool pause);
	virtual void OnFinishedNavMapGen (bool successful);
	virtual void OnLevelLoaded (bool successful);
	virtual void OnGameFinished (bool successful);

public:
	GameState_Play (std::string levelPath);

	virtual void Init ();
	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);
	virtual void Update (float dT);
	std::string GetLevelPath ();
};

#endif // GAMESTATE_PLAY_HPP
