#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <SFML/System/Vector2.hpp>

#include "../GameObjectManager.hpp"

class GameObject {
	friend class GameObjectManager;

private:
	void Reset (float windowFakeScale) {
		this->windowFakeScale = windowFakeScale;
		SetFakeScale(CalcFakeScale());
		SetFakePos(CalcFakePos());
	}

protected:
	float windowFakeScale;

	sf::Vector2f baseScale;
	sf::Vector2f basePos;

	// pretty pointless this function is - still..don't delete
	sf::Vector2f CalcFakeScale () { return baseScale * windowFakeScale; }

	virtual sf::Vector2f CalcFakePos () = 0;

	virtual void SetFakeScale (sf::Vector2f fakeScale) = 0;
	virtual void SetFakePos (sf::Vector2f fakePos) = 0;

public:
	GameObject () : windowFakeScale(1), baseScale(1, 1) { GameObjects->Add(this); }

	virtual ~GameObject () { GameObjects->Pop(this); }

	virtual void SetPosition (sf::Vector2f pos) {
		basePos = pos;
		SetFakePos(CalcFakePos());
	}
	sf::Vector2f GetPosition () { return basePos; }
	virtual void Move (sf::Vector2f offset) {
		basePos += offset;
		SetFakePos(CalcFakePos());
	}

	virtual void SetScale (sf::Vector2f scale) {
		baseScale = scale;
		SetFakeScale(CalcFakeScale());
	}
	sf::Vector2f GetScale () { return baseScale; }

	virtual void SetRotation (float rotation) = 0;
	virtual float GetRotation () = 0;
	virtual void Rotate (float angle) = 0;

	virtual sf::Vector2f GetScreenSize () = 0;
};

#endif // GAMEOBJECT_HPP
