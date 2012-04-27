#include "Character.hpp"

#include "PFrameWork/SpriteAnimationManager.hpp"
#include "PFrameWork/Utilities.hpp"
#include "PFrameWork/PFWConstants.hpp"
#include "PFrameWork/DrawManager.hpp"
#include "PFrameWork/GO/SoundEffect.hpp"
#include "PFrameWork/SoundManager.hpp"

//#include "PFrameWork/DrawManager.hpp"
//#include "PFrameWork/GO/WorldShape.hpp"
//#include "PFrameWork/SFMLGameManager.hpp"
//#include "PFrameWork/StateMachine.hpp"
//#include "PFrameWork/State.hpp"
//#include <iostream>

Character::Character (DrawManager* const drawManager,
						float walkSpd, float turnSpd,
						std::string animFile, std::string startAnim,
						int layerDepth,
						unsigned int entityType)
		: DrawableBase(layerDepth, entityType),
		WorldAnimatedSprite(layerDepth),
		CircleCollider(0, Collision::DYNAMIC),
		drawManager(drawManager),
		//timePerFrame(0),
		timeSpentCurrentFrame(0),
		walkSpd(walkSpd),
		turnSpd(turnSpd) {

	//look = baseLook;
	SpriteAnimationManager::Instance()->LoadAnimations(animFile);
	const SpriteAnimation* anim = (*SpriteAnimationManager::Instance())[startAnim];
	SetAnimation( anim );

	timePerFrame = 1.0f / anim->FPS;

	radius = GetScreenSize().x / 2;

//	mCollDbgShape = new WorldShape(new sf::Shape(
//		sf::Shape::Circle(basePos + colliderOffset, radius, sf::Color::Yellow, 3, sf::Color::Blue)),
//		0);
//	drawManager->AddWorldSpace(mCollDbgShape);
//	mCollDbgShape->EnableFill(false);

	curSpd = 0;

	drawManager->AddWorldSpace(this);

//	footsteps = new SoundEffect("Content/Audio/foot_16_slow_long.wav");
//	footsteps->SetLoop(true);
//	footsteps->SetBaseVolume(10);
//	Sounds->RefreshVol_Effect_Of(footsteps);
}

Character::~Character () {
//	drawManager->PopWorldSpace(mCollDbgShape);
//	delete mCollDbgShape;

	drawManager->PopWorldSpace(this);

	delete footsteps;
}

void Character::ReleaseResources () {
	CircleCollider::ReleaseResources();
	WorldAnimatedSprite::ReleaseResources();
}

float Character::TargetAngle (sf::Vector2f normalizedVToTarget) {
	return std::atan2(normalizedVToTarget.x, normalizedVToTarget.y) * (180.0 / PFWConstants::PI);
}

void Character::LookAt (sf::Vector2f target) {
	sf::Vector2f normalizedVToTarget = Utils->Normalized(target - basePos);

	//***********************************************************************************************

	// baseLook or look ? :O
	//float angle = std::acos(baseLook.x * normalizedVToTarget.x + baseLook.y * normalizedVToTarget.y) * 180.0 / PI;

	SetRotation(TargetAngle(-normalizedVToTarget));
	//Rotate(angle);
	//baseLook = normalizedVToTarget;
	//look = normalizedVToTarget;
}

void Character::WalkInDir (float dT, sf::Vector2f direction, bool rotate) {
	if (direction.x != 0 || direction.y != 0) {
		sf::Vector2f motion(direction * walkSpd * dT);

		if (rotate) {
			float trgtAng = TargetAngle(-motion);
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

		curSpd = Utils->Length(motion);
	}
}

void Character::WalkTowards (float dT, sf::Vector2f target, bool rotate) {
//std::cout << "WalkTowards" << std::endl;

	if (target.x != basePos.x || target.y != basePos.y) {

//std::cout << "CanwWalk" << std::endl;

//std::cout << basePos.x << " " << basePos.y << std::endl;
//std::cout << target.x << " " << target.y << std::endl;

		sf::Vector2f vToTarget = target - basePos;

//std::cout << vToTarget.x << " " << vToTarget.y << std::endl;

		sf::Vector2f dirToTarget = Utils->Normalized(vToTarget);

		sf::Vector2f motion(dirToTarget * walkSpd * dT);

		if (rotate) {
			float trgtAng = TargetAngle(-motion);
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
//if (Utils->Length(vToTarget) < 33) return;

		Utils->ClampLength(motion, walkSpd * dT);

		if (std::abs(motion.x) > std::abs(vToTarget.x))
			motion.x = vToTarget.x;
		if (std::abs(motion.y) > std::abs(vToTarget.y))
			motion.y = vToTarget.y;

		Colliders->Move(this, motion);

		curSpd = Utils->Length(motion);
	}
}

void Character::PauseAudio () {
	footsteps->Pause();
}

void Character::LateUpdate (float dT) {
	if (curSpd != 0) {
		timeSpentCurrentFrame += dT;
		if (timeSpentCurrentFrame > timePerFrame) {
			timeSpentCurrentFrame -= timePerFrame;

			ChangeFrame();
		}

		if (footsteps->GetStatus() != sf::Sound::Playing) {
			footsteps->Play();
			footsteps->SetPlayingOffset(sf::Randomizer::Random(0.0f, 12.0f));
		}
	}
	else {
		ResetAnim();
		ChangeFrame(0);

		footsteps->Pause();
	}

	curSpd = 0;
}
