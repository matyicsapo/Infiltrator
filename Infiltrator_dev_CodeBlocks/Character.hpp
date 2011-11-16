#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <string>

#include "PFrameWork/GO/WorldAnimatedSprite.hpp"
#include "PFrameWork/GO/Colliders.hpp"

class WorldShape;

class Character : public WorldAnimatedSprite, public CircleCollider {
private:
	WorldShape* mCollDbgShape;

	// unused ****************************************************************
	//const sf::Vector2i baseLook;
	//sf::Vector2i look;

	inline float TargetAngle (sf::Vector2f normalizedVToTarget);

protected:
	float walkSpd;
	float turnSpd;

	void LookAt (sf::Vector2f target);
	void WalkInDir (float dT, sf::Vector2f direction, bool rotate = true);
	void WalkTowards (float dT, sf::Vector2f targetWorldPos, bool rotate = true);

public:
	Character (float walkSpd, float turnSpd, std::string animFile, std::string startAnim, int layerDepth);

	~Character ();

	virtual void SetPosition (sf::Vector2f position);

	//virtual void Rotate (float angle);

	virtual void Update (float dT) = 0;
};

#endif // CHARACTER_HPP
