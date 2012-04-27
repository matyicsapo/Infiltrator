#include "TimeAnimatedSprite.hpp"

void TimeAnimatedSprite::Update (float dT) {
	if (playing) {
		timeSpentCurrentFrame += dT;
		if (timeSpentCurrentFrame > timePerFrame) {
			timeSpentCurrentFrame -= timePerFrame;

			ChangeFrame();
		}
	}
}

void TimeAnimatedSprite::SetAnimation (const SpriteAnimation* anim) {
	SetAnimation(anim);
	Reset();
}
