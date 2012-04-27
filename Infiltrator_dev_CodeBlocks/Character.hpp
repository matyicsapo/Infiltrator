#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <string>

#include "PFrameWork/GO/WorldAnimatedSprite.hpp"
#include "PFrameWork/GO/Colliders.hpp"

//class WorldShape;

class SoundEffect;

class Character : public WorldAnimatedSprite, public CircleCollider {
private:
	// unused ****************************************************************
	//const sf::Vector2i baseLook;
	//sf::Vector2i look;

	inline float TargetAngle (sf::Vector2f normalizedVToTarget);

protected:
	SoundEffect* footsteps;

	DrawManager* const drawManager;

//	WorldShape* mCollDbgShape;

	float timePerFrame;
	float timeSpentCurrentFrame;

	float walkSpd;
	float turnSpd;

	float curSpd;

	void LookAt (sf::Vector2f target);
	void WalkInDir (float dT, sf::Vector2f direction, bool rotate = true);
	void WalkTowards (float dT, sf::Vector2f targetWorldPos, bool rotate = true);
	void LateUpdate (float dT);

public:
	Character (DrawManager* const drawManager,
		float walkSpd, float turnSpd,
		std::string animFile, std::string startAnim,
		int layerDepth,
		unsigned int entityType = 0);

	virtual void ReleaseResources ();

	~Character ();

//	virtual void SetPosition (sf::Vector2f position);
//	virtual void Move (sf::Vector2f offset);

//	virtual void Rotate (float angle);

	void PauseAudio ();

	virtual void Update (float dT) = 0;
};

#endif // CHARACTER_HPP
