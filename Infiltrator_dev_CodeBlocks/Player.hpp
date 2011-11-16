#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "PFrameWork/SFMLEventHandler.hpp"

class Player : public Character, public SFMLEventHandler {
public:
	Player (int layerDepth = 0);

	~Player ();

	virtual void Update (float dT);

	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents);
};

#endif // PLAYER_HPP
