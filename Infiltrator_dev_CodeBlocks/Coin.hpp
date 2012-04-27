#ifndef COIN_HPP
#define COIN_HPP

#include "WallCircle.hpp"

class Coin : public WallCircle {
public:
	Coin (int layerDepth);

	virtual void OnIntersect (ColliderBase* other);
};

#endif // COIN_HPP
