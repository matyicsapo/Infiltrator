#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../SFMLEventHandler.hpp"

class GameState : public SFMLEventHandler {
public:
	virtual ~GameState () {}

	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents);

	virtual void Enter () = 0;

	virtual void Update (float dT) = 0;

	virtual void Exit () = 0;
};

#endif // GAMESTATE_HPP
