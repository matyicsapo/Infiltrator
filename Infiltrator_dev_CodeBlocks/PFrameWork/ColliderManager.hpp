#ifndef COLLIDERMANAGER_HPP
#define COLLIDERMANAGER_HPP

#include <SFML/System/Vector2.hpp>
#include <list>
#include <set>

class ColliderBase;

namespace Collision {
	enum CollType {
		STATIC = 1,
		DYNAMIC = 2,
		TRIGGER = 4
		// 8, 16, 32 etc.
	};
};

class ColliderManager {
private:
	static ColliderManager* instance;

	std::list<ColliderBase*> mColliders;

    ColliderManager () {}

	~ColliderManager ();

	ColliderManager (ColliderManager const&);
	ColliderManager& operator= (ColliderManager const&);

	void ProjectionRect (sf::Vector2f A[], unsigned int nbPointsA,
						sf::Vector2f projectTo,
						sf::Vector2f& max, sf::Vector2f& min);

	bool DoProjectionsOverlap (sf::Vector2f A[], unsigned int nbPointsA,
								sf::Vector2f B[], unsigned int nbPointsB,
								sf::Vector2f projectTo);
	bool DoProjectionsOverlap (sf::Vector2f A[], unsigned int nbPointsA,
								ColliderBase* B,
								sf::Vector2f projectTo);

public:
	static ColliderManager* Instance ();

	static void Release ();

	void Add (ColliderBase* c);
	void Pop (ColliderBase* c);

	void Move (ColliderBase* c, sf::Vector2f& motion, std::set<ColliderBase*>* exceptions = 0,
		int collisionType = Collision::STATIC | Collision::DYNAMIC | Collision::TRIGGER);

	bool IsColliding (ColliderBase* c, std::set<ColliderBase*>* exceptions = 0,
		int collisionType = Collision::STATIC | Collision::DYNAMIC);
	bool IsColliding (sf::Vector2f A[], unsigned int nbPointsA, std::set<ColliderBase*>* exceptions = 0,
		int collisionType = Collision::STATIC | Collision::DYNAMIC);
	bool IsColliding (ColliderBase* c1, ColliderBase* c2);
	bool IsColliding (sf::Vector2f A[], unsigned int nbPointsA,
						sf::Vector2f B[], unsigned int nbPointsB);
	bool IsColliding (sf::Vector2f A[], unsigned int nbPointsA,
						ColliderBase* B);

	// much like Move with the following differences
	// 	doesn't even try to actually call move for the collider
	// 	doesn't do sliding along colliders
	void ClampMotion (ColliderBase* c, sf::Vector2f& motion, std::set<ColliderBase*>* exceptions = 0,
		int collisionType = Collision::STATIC | Collision::DYNAMIC);
};

#define Colliders ColliderManager::Instance()

#endif // COLLIDERMANAGER_HPP
