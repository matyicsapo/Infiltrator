#ifndef COLLIDERS_HPP
#define COLLIDERS_HPP

#include "GameObject.hpp"
#include "../ColliderManager.hpp"

class ColliderBase : virtual public GameObject {
public:
	bool isCircle;

	ColliderBase (bool isCircle) : isCircle(isCircle) { Colliders->Add(this); }

	virtual ~ColliderBase () { Colliders->Pop(this); }
};

class CircleCollider : public ColliderBase {
public:
	float radius;

	CircleCollider () : ColliderBase(true), radius(0) {}
	CircleCollider (float radius) : ColliderBase(true), radius(radius) {}

	virtual void SetScale (sf::Vector2f scale) {
		ColliderBase::SetScale(scale);
		radius = scale.x < scale.y ? scale.x : scale.y;
	}
};

class RectCollider : public ColliderBase {
public:
	sf::Vector2f size;

	RectCollider () : ColliderBase(false), size(0, 0) {}
	RectCollider (sf::Vector2f size) : ColliderBase(false), size(size) {}

	virtual void SetScale (sf::Vector2f scale) {
		ColliderBase::SetScale(scale);
		size = sf::Vector2f(size.x * scale.x, size.y * scale.y);
	}
};

#endif // COLLIDERS_HPP
