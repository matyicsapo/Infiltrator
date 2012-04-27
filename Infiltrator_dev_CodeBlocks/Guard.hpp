#ifndef GUARD_HPP
#define GUARD_HPP

#include <deque>

#include "Character.hpp"

class WorldShape;
class Player;

class Guard : public Character {
protected:
	WorldShape* visionTriangle;
	WorldShape* visionLine;

	std::deque<sf::Vector2i>* path;
	std::deque<sf::Vector2i>::iterator itNode;

	std::deque<sf::Vector2i*> waypoints;

	enum PathFindState { IDLE = 0, NEW = 1, FOLLOW = 2 };
	int pfs;
	int direction;
	int indexCurrWP;

public:
	Guard (DrawManager* const drawManager, int layerDepth = 0);

	~Guard ();

	virtual void SetPosition (sf::Vector2f pos);
	virtual void Move (sf::Vector2f offset);

	virtual void Rotate (float angle);

	void SetBlind (bool blind);

	bool CanSeePlayer (Player* player);

	virtual void Update (float dT);

	void StartFollowingWayPoints ();

	void AssignPath (std::deque<sf::Vector2i>* path);

	void WPushBack (sf::Vector2i* wp);
	void WRemove (sf::Vector2i wp);

	std::deque<sf::Vector2i*>& GetWayPoints ();
};

#endif // GUARD_HPP
