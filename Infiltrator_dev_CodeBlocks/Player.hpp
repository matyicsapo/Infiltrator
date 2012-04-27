#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "PFrameWork/SFMLEventHandler.hpp"

class WorldShape;

class Player : public Character, public SFMLEventHandler {
protected:
	WorldShape* visionTriangle;

public:
	Player (DrawManager* const drawManager, int layerDepth = 0);

	~Player ();

	virtual void SetPosition (sf::Vector2f pos);
	virtual void Move (sf::Vector2f offset);

	virtual void Rotate (float angle);

	bool CanInteractWith (ColliderBase* object);

	virtual void Update (float dT);

	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents);
};

#endif // PLAYER_HPP
