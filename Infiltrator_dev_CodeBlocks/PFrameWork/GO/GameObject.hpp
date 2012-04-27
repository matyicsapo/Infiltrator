#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <SFML/System/Vector2.hpp>

class GameObject {
protected:
	float windowFakeScale;

	sf::Vector2f baseScale;
	sf::Vector2f basePos;

	// pretty pointless this function is - still..don't delete!
	sf::Vector2f CalcFakeScale () { return baseScale * windowFakeScale; }

	virtual sf::Vector2f CalcFakePos () = 0;

	virtual void SetFakeScale (sf::Vector2f fakeScale) = 0;
	virtual void SetFakePos (sf::Vector2f fakePos) = 0;

public:
	const unsigned int ID;
	const unsigned int entityType;

	GameObject (unsigned int entityType = 0);
	GameObject (GameObject const& xGameObject);

	virtual ~GameObject ();

	void SetFakeStuff (float windowFakeScale);

	virtual void ReleaseResources () {};

	virtual void SetPosition (sf::Vector2f pos);
	sf::Vector2f GetPosition ();
	virtual void Move (sf::Vector2f offset);

	virtual void SetScale (float scale);
	virtual void SetScale (sf::Vector2f scale);
	sf::Vector2f GetScale ();

	virtual void SetRotation (float rotation) = 0;
	virtual float GetRotation () = 0;
	virtual void Rotate (float angle) = 0;
};

#endif // GAMEOBJECT_HPP
