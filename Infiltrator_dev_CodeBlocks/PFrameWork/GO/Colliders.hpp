#ifndef COLLIDERS_HPP
#define COLLIDERS_HPP

#include "GameObject.hpp"
#include "../ColliderManager.hpp"

class ColliderBase : virtual public GameObject {
public:
	sf::Vector2f colliderOffset;

	const bool isCircle;
	int collisionType;

	ColliderBase (bool isCircle, int collisionType = Collision::STATIC, unsigned int entityType = 0)
			: GameObject(entityType),
			colliderOffset(0, 0),
			isCircle(isCircle),
			collisionType(collisionType) {
		Colliders->Add(this);
	}

	virtual ~ColliderBase () { Colliders->Pop(this); }

	virtual void ReleaseResources () {}

	virtual void OnIntersect (ColliderBase* other) {}

	//void SetColliderOffset (sf::Vector2f colliderOffset) { this->colliderOffset = colliderOffset; }
};

class CircleCollider : public ColliderBase {
public:
	float radius;

	CircleCollider (unsigned int entityType = 0) : ColliderBase(true, Collision::STATIC, entityType), radius(0) {}
	CircleCollider (float radius, unsigned int entityType = 0) : ColliderBase(true, Collision::STATIC, entityType), radius(radius) {}
	CircleCollider (float radius, int collisionType, unsigned int entityType = 0) : ColliderBase(true, collisionType, entityType), radius(radius) {}

	virtual void SetScale (sf::Vector2f scale) {
		ColliderBase::SetScale(scale);
		radius = scale.x < scale.y ? scale.x : scale.y;
	}
};

class RectCollider : public ColliderBase {
public:
	sf::Vector2f size;

	RectCollider (unsigned int entityType = 0) : ColliderBase(false, Collision::STATIC, entityType), size(0, 0) {}
	RectCollider (sf::Vector2f size, unsigned int entityType = 0) : ColliderBase(false, Collision::STATIC, entityType), size(size) {}
	RectCollider (sf::Vector2f size, int collisionType, unsigned int entityType = 0) : ColliderBase(false, collisionType, entityType), size(size) {}

	virtual void SetScale (sf::Vector2f scale) {
		ColliderBase::SetScale(scale);
		size = sf::Vector2f(size.x * scale.x, size.y * scale.y);
	}
};

class InvisibleCircleCollider : public CircleCollider {
protected:
	virtual sf::Vector2f CalcFakePos () { return basePos * windowFakeScale; }

	virtual void SetFakeScale (sf::Vector2f fakeScale) {}
	virtual void SetFakePos (sf::Vector2f fakePos) {}

public:
	InvisibleCircleCollider () : CircleCollider() {}
	InvisibleCircleCollider (float radius) : CircleCollider(radius) {}
	InvisibleCircleCollider (float radius, int collisionType) : CircleCollider(radius, collisionType) {}

	virtual void SetRotation (float rotation) {}
	virtual float GetRotation () { return 0; }
	virtual void Rotate (float angle) {}
};

class InvisibleRectCollider : public RectCollider {
protected:
	virtual sf::Vector2f CalcFakePos () { return basePos * windowFakeScale; }

	virtual void SetFakeScale (sf::Vector2f fakeScale) {}
	virtual void SetFakePos (sf::Vector2f fakePos) {}

public:
	InvisibleRectCollider () : RectCollider() {}
	InvisibleRectCollider (sf::Vector2f size) : RectCollider(size) {}
	InvisibleRectCollider (sf::Vector2f size, int collisionType) : RectCollider(size, collisionType) {}

	virtual void SetRotation (float rotation) {}
	virtual float GetRotation () { return 0; }
	virtual void Rotate (float angle) {}
};

#endif // COLLIDERS_HPP
