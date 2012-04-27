#include "GameObject.hpp"

#include "../GameObjectManager.hpp"

GameObject::GameObject (unsigned int entityType)
		: windowFakeScale(1),
		baseScale(1, 1),
		ID(GameObjects->GetValidID()),
		entityType(entityType) {
	GameObjects->Add(this);
}

GameObject::GameObject (GameObject const& xGameObject)
		: windowFakeScale(xGameObject.windowFakeScale),
		baseScale(xGameObject.baseScale),
		basePos(xGameObject.basePos),
		ID(GameObjects->GetValidID()),
		entityType(xGameObject.entityType) {
	windowFakeScale = xGameObject.windowFakeScale;
	baseScale = xGameObject.baseScale;
	basePos = xGameObject.basePos;

	GameObjects->Add(this);
}

GameObject::~GameObject () {
	GameObjects->Pop(this);
}

void GameObject::SetPosition (sf::Vector2f pos) {
	basePos = pos;
	SetFakePos(CalcFakePos());
}

sf::Vector2f GameObject::GetPosition () {
	return basePos;
}

void GameObject::Move (sf::Vector2f offset) {
	basePos += offset;
	SetFakePos(CalcFakePos());
}

void GameObject::SetScale (float scale) {
	baseScale = sf::Vector2f(scale, scale);
	SetFakeScale(CalcFakeScale());
}

void GameObject::SetScale (sf::Vector2f scale) {
	baseScale = scale;
	SetFakeScale(CalcFakeScale());
}

sf::Vector2f GameObject::GetScale () {
	return baseScale;
}

void GameObject::SetFakeStuff (float windowFakeScale) {
	this->windowFakeScale = windowFakeScale;
	SetFakeScale(CalcFakeScale());
	SetFakePos(CalcFakePos());
}
