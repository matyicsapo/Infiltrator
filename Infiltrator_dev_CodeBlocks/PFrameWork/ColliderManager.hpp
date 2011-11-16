#ifndef COLLIDERMANAGER_HPP
#define COLLIDERMANAGER_HPP

#include <SFML/System/Vector2.hpp>
#include <list>

class ColliderBase;

class ColliderManager {
private:
	std::list<ColliderBase*> mColliders;

    ColliderManager () {}

	~ColliderManager ();

	ColliderManager (ColliderManager const&);
	ColliderManager& operator= (ColliderManager const&);

public:
	static ColliderManager* Instance ();

	void Add (ColliderBase* c) { mColliders.push_back(c); }
	void Pop (ColliderBase* c) { mColliders.remove(c); }

	void Move (ColliderBase* c, sf::Vector2f& motion);
};

#define Colliders ColliderManager::Instance()

#endif // COLLIDERMANAGER_HPP
