#include "WallCircle.hpp"

#include "PFrameWork/DrawManager.hpp"
#include "PFrameWork/GO/WorldShape.hpp"

WallCircle::WallCircle (std::string textureFile, int collisionType, int layerDepth, unsigned int entityType)
		: GameObject(entityType),
		DrawableBase(layerDepth, entityType),
		WorldSprite(textureFile, layerDepth, entityType),
		CircleCollider(0, collisionType, entityType) {

	radius = GetScreenSize().x / 2;
}

WallCircle::WallCircle (WallCircle const& xWallCircle)
		: GameObject(xWallCircle),
		DrawableBase(xWallCircle),
		WorldSprite(xWallCircle) {}

void WallCircle::ReleaseResources () {
	CircleCollider::ReleaseResources();
	WorldSprite::ReleaseResources();
}
