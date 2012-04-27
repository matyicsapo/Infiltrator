#ifndef OBJECTIVE_HPP
#define OBJECTIVE_HPP

#include "PFrameWork/GO/GameObject.hpp"

enum ObjectiveState {
	ACTIVE = 1,
	COMPLETED = 2,
	INACTIVE = 3
//	, FAILED = 4
};

class BaseObjective : public virtual GameObject {
public:
	ObjectiveState objectiveState;

	BaseObjective () : objectiveState(INACTIVE) {}
};

#endif // OBJECTIVE_HPP
