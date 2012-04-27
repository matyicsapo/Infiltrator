#include "Goal.hpp"

#include "MyMagicNumbers.hpp"
//#include <iostream>
#include "GameStates/GameState_BasePlayable.hpp"
#include "PFrameWork/SFMLGameManager.hpp"
#include "PFrameWork/StateMachine.hpp"

Goal::Goal (int layerDepth)
		: GameObject(ID_GOAL),
		DrawableBase(layerDepth),
		WallCircle(TEXTURE_GOAL,
			Collision::TRIGGER,
			layerDepth,
			ID_GOAL) {
}

void Goal::OnIntersect (ColliderBase* other) {
	GameState_BasePlayable* playableState = dynamic_cast<GameState_BasePlayable*>(Game->GetGameStateMachine()->Top());

	if (playableState && other->entityType == ID_PLAYER) {
//std::cout << "no. this is not where we hid the treasure." << std::endl;
		playableState->EnteredGoal(this);
	}
}
