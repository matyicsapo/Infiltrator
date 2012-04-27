#include "Princess.hpp"

#include "PFrameWork/SpriteAnimationManager.hpp"
#include "MyMagicNumbers.hpp"
#include "PFrameWork/Utilities.hpp"
#include "AI/Navigation.hpp"
#include "PFrameWork/GO/SoundEffect.hpp"
#include "PFrameWork/SoundManager.hpp"
//#include <iostream>

Princess::Princess (DrawManager* const drawManager, int layerDepth)
		: GameObject(ID_PRINCESS),
		DrawableBase(layerDepth, ID_PRINCESS),
		Character(drawManager, 350, 720,
					"Content/Textures/Animated/princess.anim", "princess_walk",
					layerDepth, ID_PRINCESS),
		followed(0) {

	footsteps = new SoundEffect("Content/Audio/foot.wav");
	footsteps->SetLoop(true);
	footsteps->SetBaseVolume(7);
	Sounds->RefreshVol_Effect_Of(footsteps);
}

Princess::~Princess () {
	SpriteAnimationManager::Instance()->UnloadAnimation("princess_walk");
}

void Princess::Update (float dT) {
	if (followed) {
		if (Utils->Dst(basePos, followed->GetPosition()) < MyMagicNumbers::princessFollowDst) {
			footsteps->Pause();
			return;
		}

		// CALC PATH ===
		std::deque<sf::Vector2i>* path = 0;

		NavMap* navMap = Nav->GetNavMap();

		if (navMap == 0) {
			footsteps->Pause();
//std::cout << __FILE__ << " " << __LINE__ << " nomap" << std::endl;
			return;
		}

		// search for the closest NODE (to get on the nodemap/navmap)
		// search for the node closest to the goal WP
		// 	this will be the goal of the pathfinding algorithm
		sf::Vector2i closestNode(0, 0);
		sf::Vector2i closestNodeToWP(0, 0);
		float minDst = 99999;
		float minDst2nd = 99999;
		float dst;
		for (NavMap::iterator itN = navMap->begin(); itN != navMap->end(); itN++) {
			dst = Utils->Dst( basePos, itN->first );
			if (dst < minDst) {
				minDst = dst;
				closestNode = itN->first;
			}

			dst = Utils->Dst( followed->GetPosition(), itN->first );
			if (dst < minDst2nd) {
				minDst2nd = dst;
				closestNodeToWP = itN->first;
			}
		}

		path = Nav->Path(closestNode, closestNodeToWP);
		// === CALC PATH

		// MOVE ===
		if (path) {
			sf::Vector2f to( (*(path->begin())).x, (*(path->begin())).y );
			if (Utils->Dst(basePos, to) < MyMagicNumbers::princessFollowDst
					&& (++(path->begin())) != path->end()) {
				to = sf::Vector2f( (*(++(path->begin()))).x, (*(++(path->begin()))).y );
				WalkTowards(dT, to );
			}
			else {
				WalkTowards(dT, to);
			}
		}
		// === MOVE

		LateUpdate(dT);
	}
	else {
		footsteps->Pause();
	}
}

void Princess::StartFollowing (Character* character) {
	followed = character;
}

bool Princess::IsFollowingAnyone () {
	return (followed != 0);
}
