#ifndef WALLRECTANGLE_HPP
#define WALLRECTANGLE_HPP

#include "PFrameWork/GO/WorldSprite.hpp"
#include "PFrameWork/GO/Colliders.hpp"

class WallRectangle : public WorldSprite, public RectCollider {
public:
	WallRectangle (std::string textureFile,
					int collisionType = Collision::STATIC,
					int layerDepth = 0,
					unsigned int entityType = 0);

    WallRectangle (WallRectangle const& xWallRectangle);

	virtual void ReleaseResources ();
};

#endif // WALLRECTANGLE_HPP
