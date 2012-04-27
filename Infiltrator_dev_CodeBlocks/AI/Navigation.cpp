#include "Navigation.hpp"

#include <boost/bind.hpp>
#include <cmath>

#include "../MyMagicNumbers.hpp"

#include "../PFrameWork/ColliderManager.hpp"
#include "../PFrameWork/GO/Colliders.hpp"
#include "../PFrameWork/PFWConstants.hpp"
#include "../PFrameWork/Utilities.hpp"

//#include <iostream>

class Node {
//private:
//	static int lastID;

//	int mID;

public:
	Node () : /*mID(lastID++),*/ parent(0), pos(0, 0), H(0), G(0), F(0) {}

//	int ID () const { return mID; }

	Node* parent;

	sf::Vector2i pos;

	int H;
	int G;
	int F;
};

//int Node::lastID;

typedef std::map<sf::Vector2i, Node*, CompareSFVector2i > AStarNodeMap;

Navigation* Navigation::Instance () {
	static Navigation instance;

	return &instance;
}

Navigation::~Navigation () {
	delete navMap;

	delete currDummyCollider;
}

void Navigation::NavGraphGen_ProcessAdjacent (sf::Vector2i* offset) {
	sf::Vector2f fOffset((*offset).x, (*offset).y);

	Colliders->ClampMotion(currDummyCollider, fOffset, 0, Collision::STATIC);
	sf::Vector2i newPos(currBasePos + sf::Vector2i(fOffset.x, fOffset.y));
//	adjacentNodes.push_back( newPos );

//std::cout << newPos.x << " " << newPos.y << std::endl;

	float l1 = Utils->Length(*offset);
	float l2 = Utils->Length(fOffset);

	delete offset;

	if (l1 == l2) {
		adjacentNodes.push_back( newPos );

		if (navMap->count(newPos) == 0 && openNodes.count(newPos) == 0) {
			newNodes.push_back(newPos);
		}
	}
	else if (l2 > currMinStep) {
		adjacentNodes.push_back( newPos );

		if (extraNodes.count(newPos) == 0) {
			extraNodes.insert(newPos);

//std::cout << newPos.x << " " << newPos.y << std::endl;
		}

	}
}

NavMap* Navigation::GenerateNavMap (sf::Vector2i levelSize, float minStep, float step) {
	cancelledGenerateNavMap = false;

	delete navMap;
	navMap = new NavMap();

	currBasePos = sf::Vector2i(step, step);

	currDummyCollider = new InvisibleCircleCollider(MyMagicNumbers::characterRadius);
	currDummyCollider->SetPosition(sf::Vector2f(currBasePos.x, currBasePos.y));

	// looking for a position to start off the flood fill algorithm from
	do {
		if (!Colliders->IsColliding(currDummyCollider, 0, Collision::STATIC))
			break;

		currBasePos.x += step;
		if (currBasePos.x + step > levelSize.x) {
			currBasePos.x = step;

			currBasePos.y += step;
			if (currBasePos.y + step > levelSize.y) {
				delete currDummyCollider;
				currDummyCollider = 0;
				return 0;
			}
		}

		currDummyCollider->SetPosition(sf::Vector2f(currBasePos.x, currBasePos.y));
	} while (true);

	currMinStep = minStep;

	openNodes.clear();
	extraNodes.clear();

	openNodes.insert(currBasePos);

	while ( ! openNodes.empty() ) {
		if (cancelledGenerateNavMap)
			return 0;

		newNodes.clear();

		// iterate all newly found nodes
		for (std::set<sf::Vector2i>::iterator itNN = openNodes.begin(); itNN != openNodes.end(); itNN++) {
			if (cancelledGenerateNavMap)
				return 0;

			adjacentNodes.clear();

			currBasePos = *itNN;
			currDummyCollider->SetPosition(sf::Vector2f(currBasePos.x, currBasePos.y));

			NavGraphGen_ProcessAdjacent(new sf::Vector2i(-step, 0)); // left
			NavGraphGen_ProcessAdjacent(new sf::Vector2i(step, 0)); // right
			NavGraphGen_ProcessAdjacent(new sf::Vector2i(0, -step)); // top
			NavGraphGen_ProcessAdjacent(new sf::Vector2i(0, step)); // bottom
			NavGraphGen_ProcessAdjacent(new sf::Vector2i(-step, -step)); // top-left
			NavGraphGen_ProcessAdjacent(new sf::Vector2i(-step, step)); // bottom-left
			NavGraphGen_ProcessAdjacent(new sf::Vector2i(step, -step)); // top-right
			NavGraphGen_ProcessAdjacent(new sf::Vector2i(step, step)); // bottom-right

			// add a new element to the NavGraph with this node's position as the key and the deque of the adjacent ones
			navMap->insert(std::pair<sf::Vector2i, std::deque<sf::Vector2i> >(currBasePos, adjacentNodes));

//std::cout << currBasePos.x << " " << currBasePos.y << std::endl;
		}

		// clear the list of new nodes..
		openNodes.clear();

		// ..and add the adjacent
		openNodes.insert(newNodes.begin(), newNodes.end());
	}

	for (std::set<sf::Vector2i>::iterator itEN = extraNodes.begin();
			itEN != extraNodes.end(); itEN++) {
		if (cancelledGenerateNavMap)
			return 0;

		adjacentNodes.clear();

//std::cout << std::endl;

		for (std::set<sf::Vector2i>::iterator itEN_2 = extraNodes.begin();
				itEN_2 != extraNodes.end(); itEN_2++) {
			if (cancelledGenerateNavMap)
				return 0;

//std::cout << (*itEN_2).x << " " << (*itEN_2).y << std::endl;

			// address based compare
			if (itEN_2 != itEN) {

//				if (Utils->Dst(*itEN, *itEN_2) <= step) {
//wextranodes.push_back(new WorldShape(
//	new sf::Shape(
//	sf::Shape::Line(*itEN, *itEN_2, 10, sf::Color::Black, 2.25f, sf::Color::Blue)),
////	sf::Shape::Circle(newPos, 50, sf::Color(0, 0, 0, 64), 0, sf::Color::Blue)),
//	0));
//				}



				if (Utils->Dst(*itEN, *itEN_2) <= step) {
					adjacentNodes.push_back(*itEN_2);
				}
			}
		}

//std::cout << std::endl;

		for (NavMap::iterator itNM = navMap->begin(); itNM != navMap->end(); itNM++) {
			if (cancelledGenerateNavMap)
				return 0;

			// vector2 based compare
			if (itNM->first != *itEN) {
				if (Utils->Dst(*itEN, itNM->first) <= step) {
					adjacentNodes.push_back(itNM->first);
				}
			}

//std::cout << itNM->first.x << " " << itNM->first.y << std::endl;

		}

		navMap->insert(std::pair<sf::Vector2i, std::deque<sf::Vector2i> >(*itEN, adjacentNodes));

//std::cout << (*itEN).x << " " << (*itEN).y << std::endl;

	}

	delete currDummyCollider;
	currDummyCollider = 0;






//for (NavMap::iterator itN = navMap->begin(); itN != navMap->end(); ++itN) {
//
//std::cout << itN->first.x << " " << itN->first.y << std::endl;
//
//	for (std::deque<sf::Vector2i>::iterator itA = itN->second.begin();
//			itA != itN->second.end(); ++itA) {
//
//std::cout << "  " << (*itA).x << " " << (*itA).y << std::endl;
//
//	}
//}





	return navMap;
}

NavMap* Navigation::GetNavMap () {
	return navMap;
}

void Navigation::SetNavMap (NavMap* navMap) {
	delete this->navMap;

	this->navMap = navMap;
}

std::deque<sf::Vector2i>* Navigation::Path (sf::Vector2i from, sf::Vector2i to) {
	if (!navMap)
		return 0;

	// currNode is the node with the lowest F
	// which, at start, is the goal position since the algorithm works backwards
	Node* currNode = new Node();
	currNode->pos = to;

	// creating the closed list
	AStarNodeMap closedList;
	closedList.insert(std::pair<sf::Vector2i, Node*>(currNode->pos, currNode));

	AStarNodeMap openList;

	// whether the algorithm has to search on or is it finished
	bool unresolved = true;

//std::cout << std::endl;
//std::cout << currNode << std::endl;

	do {
//std::cout << "searching..." << currNode->pos.x << " " << currNode->pos.y << std::endl;
		NavMap::iterator currNavNode = navMap->find(currNode->pos);
//
//		if (currNavNode == navMap->end()) {
//std::cout << "FCUK!!" << std::endl;
////			continue;
//		}
//		else {
//std::cout << "fnd"<< std::endl;
//		}

		for (std::deque<sf::Vector2i>::iterator itAdj = currNavNode->second.begin();
				itAdj != currNavNode->second.end(); itAdj++) {

			if (*itAdj == from) {
				std::deque<sf::Vector2i>* path = new std::deque<sf::Vector2i>();

				path->push_back(currNode->pos);
				do {
					if (currNode->parent != 0) {
						currNode = currNode->parent;
						path->push_back(currNode->pos);
					}
					else {
						unresolved = false;
					}
				} while (unresolved);

				return path;
			}
		}

//std::cout << std::endl;

		// check for the next best node
		for (std::deque<sf::Vector2i>::iterator itAdj = currNavNode->second.begin();
				itAdj != currNavNode->second.end(); itAdj++) {

//std::cout << "    " << (*itAdj).x << " " << (*itAdj).y << std::endl;

			// if not already on closed list
			if (closedList.count(*itAdj) == 0) {
				AStarNodeMap::iterator itAON = openList.find(*itAdj);

				if (itAON != openList.end()) {
					// it's already on the openList

					int newG = currNode->G + Utils->Dst(currNode->pos, (*itAdj));
					if (newG < itAON->second->G) {
						// this is a better path
						itAON->second->G = newG;
						itAON->second->F = newG + itAON->second->H;
						itAON->second->parent = currNode;
					}
				}
				else {
					// a new node
					Node* newOpenNode = new Node();

					newOpenNode->parent = currNode;
					newOpenNode->pos = *itAdj;
					newOpenNode->G = currNode->G + Utils->Dst(currNode->pos, (*itAdj));
					newOpenNode->H = std::abs((float)(from.x - (*itAdj).x)) + std::abs((float)(from.y - (*itAdj).y));
					newOpenNode->F = newOpenNode->G + newOpenNode->H;
					openList.insert(std::pair<sf::Vector2i, Node*>(newOpenNode->pos, newOpenNode));
				}
			}
		}


		if (openList.size() == 0) {
			// there are no more nodes in the openList - no path
			unresolved = false;
		}
		else {
			// the next best node is the one with the lowest F
			// so we do a simple minimum search
			AStarNodeMap::iterator itMinF = openList.begin();
			for (AStarNodeMap::iterator itON = ++openList.begin(); itON != openList.end(); itON++) {
				if (itON->second->F < itMinF->second->F)
					itMinF = itON;
			}

			closedList.insert(std::pair<sf::Vector2i, Node*>(itMinF->first, itMinF->second));
			openList.erase(itMinF);

			currNode = itMinF->second;
		}
	} while (unresolved);

	return 0;
}

void Async_GenerateNavMap (void* UserData) {
	ThreadData_GenerateNavMap* d = static_cast<ThreadData_GenerateNavMap*>(UserData);

	d->navMapGenThreadState = NONE;

	NavMap* navMap = Nav->GenerateNavMap(d->levelSize,
										MyMagicNumbers::characterRadius * .10f,
										MyMagicNumbers::characterRadius * .75f);

	d->navMapGenThreadState = navMap ? SUCCESS : FAILURE;

	delete d;
}
