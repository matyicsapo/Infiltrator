#ifndef PRINCESS_HPP
#define PRINCESS_HPP

#include "Character.hpp"
#include "Objective.hpp"

class Princess : public Character, public BaseObjective {
private:
	Character* followed;

public:
	Princess (DrawManager* const drawManager, int layerDepth = 0);

	~Princess ();

	virtual void Update (float dT);

	void StartFollowing (Character* character);
	bool IsFollowingAnyone ();
};

#endif // PRINCESS_HPP
