#include "Player.hpp"

#include <cmath>

#include "WorldAnimatedSprite.hpp"
#include "SpriteAnimationManager.hpp"
#include "DrawManager.hpp"

#include "MyConstants.hpp"
#include "Utilities.hpp"

#include "SFMLGameManager.hpp"

Player::Player () : baseLook(0, 1), // looking down since that's how the animation is at 0 rotation
					walkSpd(350),
					turnSpd(720) {
	look = baseLook;
	SpriteAnimationManager::Instance()->LoadAnimations("Content/Textures/Animated/player.anim");
	was = new WorldAnimatedSprite((*SpriteAnimationManager::Instance())["player_walk"]);
	DrawManager::Instance()->Add(was);
}

Player::Player (Player const& xPlayer) {
	was = new WorldAnimatedSprite( *(xPlayer.was) );
	DrawManager::Instance()->Add(was);
}

Player::~Player () {
	DrawManager::Instance()->Pop(was);
	delete was;
}

float Player::TargetAngle (sf::Vector2f normalizedVToTarget) {
	return std::atan2(normalizedVToTarget.x, normalizedVToTarget.y) * 180.0 / MConst::PI;
}

float Player::AngleBetween (sf::Vector2f nV1, sf::Vector2f nV2) {
	return std::acos(nV1.x * nV2.x + nV1.y * nV2.y) * 180.0 / MConst::PI;
}

void Player::LookAt (sf::Vector2f target) {
	sf::Vector2f normalizedVToTarget = Utilities::Instance()->Normalized(target - was->GetPosition());

	// baseLook or look ? :O
	//float angle = std::acos(baseLook.x * normalizedVToTarget.x + baseLook.y * normalizedVToTarget.y) * 180.0 / PI;

	was->SetRotation(TargetAngle(normalizedVToTarget));
	//was->Rotate(angle);
	//baseLook = normalizedVToTarget;
	look = normalizedVToTarget;
}

void Player::SetPosition (sf::Vector2f position) {
	was->SetPosition(position);
}

sf::Vector2f Player::GetPosition () {
	return was->GetPosition();
}

void Player::WalkInDir (float dT, sf::Vector2f direction, bool rotate) {
	if (direction.x != 0 || direction.y != 0) {
		Utilities::Instance()->Normalize(direction);

		sf::Vector2f motion(direction * walkSpd * dT);

		if (rotate) {
			float trgtAng = TargetAngle(motion);
			if (trgtAng < 0)
				trgtAng = 360 + trgtAng;

			float mAng = was->GetRotation();

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

			was->Rotate(r);
		}

		was->Move(motion);
	}
}

void Player::WalkTowards (float dT, sf::Vector2f target, bool rotate) {
	if (target.x != was->GetPosition().x || target.y != was->GetPosition().y) {
		sf::Vector2f vToTarget = target - was->GetPosition();

		sf::Vector2f dirToTarget = Utilities::Instance()->Normalized(vToTarget);

		sf::Vector2f motion(dirToTarget * walkSpd * dT);

		if (rotate) {
			float trgtAng = TargetAngle(motion);
			if (trgtAng < 0)
				trgtAng = 360 + trgtAng;

			float mAng = was->GetRotation();

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

			was->Rotate(r);
		}

		// rotation done
		// don't move if target is too close - would jitter
		if (Utilities::Instance()->Length(vToTarget) < 33) return;

		if (std::abs(motion.x) > std::abs(vToTarget.x))
			motion.x = vToTarget.x;
		if (std::abs(motion.y) > std::abs(vToTarget.y))
			motion.y = vToTarget.y;

		was->Move(motion);
	}
}
