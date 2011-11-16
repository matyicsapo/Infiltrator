#include "TimeAnimatedSprite.hpp"

#include "../SpriteAnimationManager.hpp"
#include "../DrawManager.hpp"
#include "WorldDrawable.hpp"
#include "ScreenSpaceDrawable.hpp"

TimeAnimatedSprite::TimeAnimatedSprite (AnimatedSprite* animSprite) : animSprite(animSprite),
													timePerFrame(1.0f / animSprite->GetAnimation()->FPS),
													timeSpentCurrentFrame(0),
													playing(true) {

	WorldDrawable* wd = dynamic_cast<WorldDrawable*>(animSprite);
	if (wd != 0) {
		Drawables->AddWorldSpace(wd);
	}
	else {
		ScreenSpaceDrawable* sd = dynamic_cast<ScreenSpaceDrawable*>(animSprite);
		Drawables->AddScreenSpace(sd);
	}
}

TimeAnimatedSprite::~TimeAnimatedSprite () {
	WorldDrawable* wd = dynamic_cast<WorldDrawable*>(animSprite);
	if (wd != 0) {
		Drawables->PopWorldSpace(wd);
	}
	else {
		ScreenSpaceDrawable* sd = dynamic_cast<ScreenSpaceDrawable*>(animSprite);
		Drawables->PopScreenSpace(sd);
	}

	delete animSprite;
}

void TimeAnimatedSprite::Update (float dT) {
	if (playing) {
		timeSpentCurrentFrame += dT;
		if (timeSpentCurrentFrame > timePerFrame) {
			timeSpentCurrentFrame -= timePerFrame;

			animSprite->ChangeFrame();
		}
	}
}

void TimeAnimatedSprite::SetAnimation (const SpriteAnimation* anim) {
	animSprite->SetAnimation(anim);
	ResetTimeAnim();
}
