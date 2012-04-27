#ifndef CHEST_HPP
#define CHEST_HPP

#include "WallRectangle.hpp"
#include "Objective.hpp"
#include "MyMagicNumbers.hpp"

class Chest : public WallRectangle, public BaseObjective {
public:
	Chest (int layerDepth)
		: GameObject(ID_CHEST),
		DrawableBase(layerDepth),
		WallRectangle(TEXTURE_CHEST,
			Collision::STATIC,
			layerDepth,
			ID_CHEST) {}
};

#endif // CHEST_HPP
