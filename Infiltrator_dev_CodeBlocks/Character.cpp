#include "Character.hpp"

#include "PFrameWork/SpriteAnimationManager.hpp"
#include "PFrameWork/Utilities.hpp"
#include "PFrameWork/PFWConstants.hpp"

#include "PFrameWork/DrawManager.hpp"
#include "PFrameWork/GO/WorldShape.hpp"

Character::Character (float walkSpd, float turnSpd, std::string animFile, std::string startAnim, int layerDepth)
		: DrawableBase(layerDepth),
		WorldAnimatedSprite(layerDepth),
		CircleCollider(),
		walkSpd(walkSpd),
		turnSpd(turnSpd) {

	//look = baseLook;
	SpriteAnimationManager::Instance()->LoadAnimations(animFile);
	SetAnimation( (*SpriteAnimationManager::Instance())[startAnim] );

	radius = GetScreenSize().x / 2;

	mCollDbgShape = new WorldShape(new sf::Shape(
		sf::Shape::Circle(basePos, radius, sf::Color::Yellow, 3, sf::Color::Blue)),
		0);
	mCollDbgShape->EnableFill(false);
}

Character::~Character () {
	Drawables->PopWorldSpace(mCollDbgShape);
	delete mCollDbgShape;
}

float Character::TargetAngle (sf::Vector2f normalizedVToTarget) {
	return std::atan2(normalizedVToTarget.x, normalizedVToTarget.y) * (180.0 / PFWConstants::PI);
}

void Character::LookAt (sf::Vector2f target) {
	sf::Vector2f normalizedVToTarget = Utils->Normalized(target - basePos);

	//***********************************************************************************************

	// baseLook or look ? :O
	//float angle = std::acos(baseLook.x * normalizedVToTarget.x + baseLook.y * normalizedVToTarget.y) * 180.0 / PI;

	SetRotation(TargetAngle(normalizedVToTarget));
	//Rotate(angle);
	//baseLook = normalizedVToTarget;
	//look = normalizedVToTarget;
}

void Character::WalkInDir (float dT, sf::Vector2f direction, bool rotate) {
	if (direction.x != 0 || direction.y != 0) {
		sf::Vector2f motion(direction * walkSpd * dT);

		if (rotate) {
			float trgtAng = TargetAngle(motion);
			if (trgtAng < 0)
				trgtAng = 360 + trgtAng;

			float mAng = GetRotation();

			float rotRem = trgtAng - mAng;
			if (rotRem > 180)
				rotRem = rotRem - 360;
			else if (rotRem < -180)
				rotRem = rotRem + 360;

			float r = 0;

			if (rotRem > 0) {
				r = turnSpd * dT;
				if (rotRem - r < 0) {
					r += rotRem - r;
				}
			}
			else
			//if (rotRem < 0)
			{
				r = -turnSpd * dT;
				if (rotRem - r > 0) {
					r += rotRem - r;
				}
			}

			Rotate(r);
		}

		// setting the maximum lenght so that transversal movement isn't faster
		Utils->ClampLength(motion, walkSpd * dT);

		Colliders->Move(this, motion);
		mCollDbgShape->Move(motion);
	}
}

void Character::WalkTowards (float dT, sf::Vector2f target, bool rotate) {
	if (target.x != basePos.x || target.y != basePos.y) {
		sf::Vector2f vToTarget = target - basePos;

		sf::Vector2f dirToTarget = Utils->Normalized(vToTarget);

		sf::Vector2f motion(dirToTarget * walkSpd * dT);

		if (rotate) {
			float trgtAng = TargetAngle(motion);
			if (trgtAng < 0)
				trgtAng = 360 + trgtAng;

			float mAng = GetRotation();

			float rotRem = trgtAng - mAng;
			if (rotRem > 180)
				rotRem = rotRem - 360;
			else if (rotRem < -180)
				rotRem = rotRem + 360;

			float r = 0;

			if (rotRem > 0) {
				r = turnSpd * dT;
				if (rotRem - r < 0) {
					r += rotRem - r;
				}
			}
			else
			//if (rotRem < 0)
			{
				r = -turnSpd * dT;
				if (rotRem - r > 0) {
					r += rotRem - r;
				}
			}

			Rotate(r);
		}

		// rotation done
		// don't move if target is too close - would jitter
		if (Utils->Length(vToTarget) < 33) return;

		Utils->ClampLength(motion, walkSpd * dT);

		if (std::abs(motion.x) > std::abs(vToTarget.x))
			motion.x = vToTarget.x;
		if (std::abs(motion.y) > std::abs(vToTarget.y))
			motion.y = vToTarget.y;

		Colliders->Move(this, motion);
		mCollDbgShape->Move(motion);
	}
}

void Character::SetPosition (sf::Vector2f position) {
	GameObject::SetPosition(position);
	mCollDbgShape->SetPosition(position);
}
