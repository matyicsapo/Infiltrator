#ifndef WALLCIRCLE_HPP
#define WALLCIRCLE_HPP

#include "PFrameWork/GO/WorldSprite.hpp"
#include "PFrameWork/GO/Colliders.hpp"

class WallCircle : public WorldSprite, public CircleCollider {
public:
	WallCircle (std::string textureFile,
				int collisionType = Collision::STATIC,
				int layerDepth = 0,
				unsigned int entityType = 0);

    WallCircle (WallCircle const& xWallCircle);

	virtual void ReleaseResources ();
};


#endif // WALLCIRCLE_HPP
