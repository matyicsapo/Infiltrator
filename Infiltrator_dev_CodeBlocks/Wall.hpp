#ifndef WALL_HPP
#define WALL_HPP

#include "PFrameWork/GO/WorldSprite.hpp"
#include "PFrameWork/GO/Colliders.hpp"

class WorldShape;

class Wall : public WorldSprite, public RectCollider {
private:
	WorldShape* mCollDbgShape;

public:
	Wall (std::string textureFile, int layerDepth);

	~Wall ();

	virtual void SetPosition (sf::Vector2f position);
	virtual void SetScale (sf::Vector2f scale);
};

#endif // WALL_HPP
