#ifndef GOAL_HPP
#define GOAL_HPP

#include "WallCircle.hpp"
#include "Objective.hpp"

class Goal : public WallCircle, public BaseObjective {
public:
	Goal (int layerDepth);

	virtual void OnIntersect (ColliderBase* other);
};

#endif // GOAL_HPP
