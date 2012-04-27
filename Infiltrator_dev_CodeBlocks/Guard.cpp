#include "Guard.hpp"

#include "PFrameWork/SpriteAnimationManager.hpp"
#include "MyMagicNumbers.hpp"
#include "PFrameWork/Utilities.hpp"
#include "AI/Navigation.hpp"
#include "PFrameWork/GO/WorldShape.hpp"
#include "Player.hpp"
#include "PFrameWork/ColliderManager.hpp"
#include "PFrameWork/SFMLGameManager.hpp"
#include "PFrameWork/StateMachine.hpp"
#include "GameStates/GameState_Play.hpp"
#include "GameStates/GameState_Editor.hpp"
#include "PFrameWork/GO/SoundEffect.hpp"
#include "PFrameWork/SoundManager.hpp"

//#include <iostream>

Guard::Guard (DrawManager* const drawManager, int layerDepth)
		: GameObject(ID_GUARD),
		DrawableBase(layerDepth, ID_GUARD),
		Character(drawManager, 200, 720, "Content/Textures/Animated/guard.anim", "guard_walk", layerDepth),
		visionTriangle(0), visionLine(0), path(0), pfs(0) {

//	colliderOffset = sf::Vector2f(0, -30);
//	SetPosition(basePos);

	SetBlind(false);

	footsteps = new SoundEffect("Content/Audio/foot.wav");
	footsteps->SetLoop(true);
	footsteps->SetBaseVolume(5);
	Sounds->RefreshVol_Effect_Of(footsteps);
}

Guard::~Guard () {
//	SpriteAnimationManager::Instance()->UnloadAnimation("guard_walk");

	drawManager->PopWorldSpace(visionTriangle);
	delete visionTriangle;

	drawManager->PopWorldSpace(visionLine);
	delete visionLine;

	drawManager->PopWorldSpace(this);

	delete path;
}

void Guard::SetPosition (sf::Vector2f pos) {
	Character::SetPosition(pos);

	if (visionTriangle)
		visionTriangle->SetPosition(basePos);

	if (visionLine)
		visionLine->SetPosition(basePos);
}

void Guard::Move (sf::Vector2f offset) {
	Character::Move(offset);

	if (visionTriangle)
		visionTriangle->Move(offset);

	if (visionLine)
		visionLine->Move(offset);
}

void Guard::Rotate (float angle) {
	Character::Rotate(angle);

	if (visionTriangle)
		visionTriangle->Rotate(angle);

	if (visionLine)
		visionLine->Rotate(angle);
}

void Guard::SetBlind (bool blind) {
	if (blind && visionTriangle != 0) {
		drawManager->PopWorldSpace(visionTriangle);
		delete visionTriangle;
		visionTriangle = 0;

		drawManager->PopWorldSpace(visionLine);
		delete visionLine;
		visionLine = 0;
	}
	else if (!blind && visionTriangle == 0) {
		visionTriangle = new WorldShape(new sf::Shape());
		sf::Vector2f A(000, 0);
		sf::Vector2f B(250, 0);
		sf::Vector2f C(125, 300);
		visionTriangle->AddPoint(A);
		visionTriangle->AddPoint(B);
		visionTriangle->AddPoint(C);
		visionTriangle->SetCenter(C);
		visionTriangle->SetColor(sf::Color(128, 128, 128, 16));
		drawManager->AddWorldSpace(visionTriangle);
		visionTriangle->SetLayerDepth(LAYER_VISIONTRIANGLE);

		sf::Vector2f U(125, 0);
		sf::Vector2f V(125, 300);
		visionLine = new WorldShape(new sf::Shape(
			sf::Shape::Line(U, V, 5, sf::Color::Cyan)));
		visionLine->SetCenter(V);
//		drawManager->AddWorldSpace(visionLine);
		visionLine->SetLayerDepth(LAYER_VISIONTRIANGLE);

//		visionLine->SetPointPosition(3, B);
////		visionLine->SetPointPosition(2, player->GetPosition());

		SetPosition(GetPosition());

		drawManager->SortAscendingWorld();
	}
}

bool Guard::CanSeePlayer (Player* player) {
	if (!visionTriangle)
		return false;

//	this->radius *= 1.5f;
	if (Colliders->IsColliding(this, player)) {
//		this->radius /= 1.5f;
		return true;
	}
//	this->radius /= 1.5f;

	sf::Vector2f T[visionTriangle->GetNbPoints()];
	for (unsigned int i = 0; i != visionTriangle->GetNbPoints(); ++i) {
		T[i] = visionTriangle->GetPointPositionTransformed(i);
	}

	if (Colliders->IsColliding(T, visionTriangle->GetNbPoints(), player)) {
		std::set<ColliderBase*> exceptionColliders;

		exceptionColliders.insert(player);
		exceptionColliders.insert(this);

		InvisibleRectCollider** levelBoundColliders = 0;
		GameState_BasePlayable* gs_basePlayable = dynamic_cast<GameState_BasePlayable*>(Game->GetGameStateMachine()->Top());
		levelBoundColliders = gs_basePlayable->GetLevelBoundColliders();
		if (levelBoundColliders != 0) {
			for (int i = 0; i != 4; ++i) {
				exceptionColliders.insert(levelBoundColliders[i]);
			}
		}

		sf::Vector2f L[2] { player->GetPosition(), visionTriangle->GetPosition() };

		visionLine->SetPointPosition(0, visionLine->TransformToLocal(player->GetPosition()));
		visionLine->SetPointPosition(3, visionLine->TransformToLocal(player->GetPosition()));

		if (!Colliders->IsColliding(L, 2, &exceptionColliders))
			return true;
	}

	return false;
}

void Guard::StartFollowingWayPoints () {
	if (waypoints.empty()) {
		path = 0;
		return;
	}

	pfs = IDLE;

	NavMap* navMap = Nav->GetNavMap();

	if (navMap == 0) {
//std::cout << __FILE__ << " " << __LINE__ << " nomap" << std::endl;
		path = 0;
		return;
	}

	// search for the 2 closest WAYPOINTS
	int index2ndClosestWP = 0;
	indexCurrWP = 0; // closest wp
	float minDst2nd = 99999;
	float minDst = 99998;
	float dst;
	for (unsigned int i = 0; i != waypoints.size(); ++i) {
		dst = Utils->Dst( sf::Vector2i(basePos.x, basePos.y), *waypoints[i] );
		if (dst < minDst) {
			index2ndClosestWP = indexCurrWP;
			indexCurrWP = i;
			minDst2nd = minDst;
			minDst = dst;
		}
		else if (dst < minDst2nd) {
			index2ndClosestWP = i;
			minDst2nd = dst;
		}
	}

	// start following the waypoints the direction the closer wp is
	direction = indexCurrWP > index2ndClosestWP ? 1 : -1;

	// search for the closest NODE (to get on the nodemap/navmap)
	// search for the node closest to the goal WP
	// 	this will be the goal of the pathfinding algorithm
	sf::Vector2i closestNode(0, 0);
	sf::Vector2i closestNodeToWP(0, 0);
	minDst = 99999;
	minDst2nd = 99999;
	for (NavMap::iterator itN = navMap->begin(); itN != navMap->end(); itN++) {
		dst = Utils->Dst( basePos, sf::Vector2f(itN->first.x, itN->first.y) );
		if (dst < minDst) {
			minDst = dst;
			closestNode = itN->first;
		}

		dst = Utils->Dst( *waypoints[indexCurrWP], itN->first );
		if (dst < minDst2nd) {
			minDst2nd = dst;
			closestNodeToWP = itN->first;
		}
	}

	path = Nav->Path(closestNode, closestNodeToWP);
	if (path) {
		itNode = path->begin();

		pfs = NEW;
	}
}

void Guard::Update (float dT) {
	if (path != 0) {
		switch (pfs) {
			case NEW:
				if (Utils->Dst(sf::Vector2i(basePos.x, basePos.y), *(path->begin())) <= MyMagicNumbers::guardNodeFollowDst) {
					pfs = FOLLOW;
					break;
				}

				WalkTowards( dT, sf::Vector2f( (*(path->begin())).x, (*(path->begin())).y ) );
			break;
			case FOLLOW:
				if (Utils->Dst(basePos, *itNode) <= MyMagicNumbers::guardNodeFollowDst) {
					++itNode;

					if (itNode == path->end()) {
						if (direction == 1) {
							++indexCurrWP;
						}
						else {
							--indexCurrWP;
						}

						if ((unsigned int)indexCurrWP == waypoints.size()) {
							indexCurrWP = waypoints.size() - 1;
							direction = -1;
						}
						else if (indexCurrWP < 0) {
							indexCurrWP = 0;
							direction = 1;
						}

						if (waypoints.size() == 1) {
							pfs = IDLE;
						}

						NavMap* navMap = Nav->GetNavMap();
						sf::Vector2i closestNodeToWP(0, 0);
						float minDst2nd = 99999;
						float dst;
						for (NavMap::iterator itN = navMap->begin(); itN != navMap->end(); itN++) {
							dst = Utils->Dst( *waypoints[indexCurrWP], itN->first );
							if (dst < minDst2nd) {
								minDst2nd = dst;
								closestNodeToWP = itN->first;
//std::cout << "  " << closestNodeToWP.x <<  " " << closestNodeToWP.y << std::endl;
							}
						}
//std::cout << "HELLO " << closestNodeToWP.x <<  " " << closestNodeToWP.y << std::endl;
//std::cout << "HELLO " << (path->back()).x <<  " " << (path->back()).y << std::endl;
						path = Nav->Path(path->back(), closestNodeToWP);
						if (path) {
							itNode = path->begin();
						}
					}
				}

				WalkTowards( dT, sf::Vector2f((*itNode).x, (*itNode).y) );
			break;
		}

		LateUpdate(dT);
	}












//	if (path != 0) {
//		switch (pfs) {
//			case NEW:
//				if (basePos == *(path->begin()) ) {
//					pfs = FOLLOW;
//					break;
//				}
//
//				if ( path != 0) {
//					WalkTowards( dT, *(path->begin()) );
//				}
//			break;
//			case FOLLOW:
////std::cout << basePos.x << " " << basePos.y << std::endl;
////std::cout << (*itNode).x << " " << (*itNode).y << std::endl;
//				//if (basePos == *itNode) {
//				if (Utils->Dst(basePos, *itNode) <= 10) {
////std::cout << "new" << std::endl;
//					itNode++;
//					if (itNode == path->end())
//						pfs = 0;
//				}
//
//				if (pfs == FOLLOW)
//					WalkTowards( dT, *itNode );
//
////std::cout << std::endl;
//			break;
//		}
//
//		if (curSpd != 0) {
//			timeSpentCurrentFrame += dT;
//			if (timeSpentCurrentFrame > timePerFrame) {
//				timeSpentCurrentFrame -= timePerFrame;
//
//				ChangeFrame();
//			}
//		}
//		else {
//			ResetAnim();
//			ChangeFrame(0);
//		}
//	}
}

void Guard::AssignPath (std::deque<sf::Vector2i>* path) {
//	delete this->path;
//	this->path = path;
//	pfs = FOLLOW;
//	itNode = path->begin();
}

void Guard::WPushBack (sf::Vector2i* wp) {
	waypoints.push_back(wp);
}

void Guard::WRemove (sf::Vector2i wp) {
	for (std::deque<sf::Vector2i*>::iterator itWP = waypoints.begin(); itWP != waypoints.end(); itWP++) {
		if (**itWP == wp) {
			waypoints.erase(itWP);
			break;
		}
	}
}

std::deque<sf::Vector2i*>& Guard::GetWayPoints () {
	return waypoints;
}
