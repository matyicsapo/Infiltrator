#include "WallRectangle.hpp"

#include "PFrameWork/DrawManager.hpp"
#include "PFrameWork/GO/WorldShape.hpp"

WallRectangle::WallRectangle (std::string textureFile, int collisionType, int layerDepth, unsigned int entityType)
		: GameObject(entityType),
		DrawableBase(layerDepth, entityType),
		WorldSprite(textureFile, layerDepth, entityType),
		RectCollider(sf::Vector2f(0, 0), collisionType, entityType) {

	size = GetScreenSize();
}

WallRectangle::WallRectangle (WallRectangle const& xWallRectangle)
		: GameObject(xWallRectangle),
		DrawableBase(xWallRectangle),
		WorldSprite(xWallRectangle) {}

void WallRectangle::ReleaseResources () {
	RectCollider::ReleaseResources();
	WorldSprite::ReleaseResources();
}
