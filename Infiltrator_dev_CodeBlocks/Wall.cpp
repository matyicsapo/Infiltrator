#include "Wall.hpp"

#include "PFrameWork/DrawManager.hpp"
#include "PFrameWork/GO/WorldShape.hpp"

Wall::Wall (std::string textureFile, int layerDepth)
		: DrawableBase(layerDepth),
		WorldSprite(textureFile, layerDepth),
		RectCollider() {

	size = GetScreenSize();

	mCollDbgShape = new WorldShape(new sf::Shape(
		sf::Shape::Rectangle(basePos - size / 2.0f, basePos + size / 2.0f,
			sf::Color::Yellow, 3, sf::Color::Blue)),
		0);
	mCollDbgShape->EnableFill(false);
}

Wall::~Wall () {
	Drawables->PopWorldSpace(mCollDbgShape);
	delete mCollDbgShape;
}

void Wall::SetPosition (sf::Vector2f position) {
	GameObject::SetPosition(position);
	mCollDbgShape->SetPosition(position);
}

void Wall::SetScale (sf::Vector2f scale) {
	RectCollider::SetScale(scale);
	mCollDbgShape->SetScale(scale);
}
