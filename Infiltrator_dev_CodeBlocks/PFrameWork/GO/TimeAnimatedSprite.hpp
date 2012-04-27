#ifndef TIMEANIMATEDSPRITE_HPP
#define TIMEANIMATEDSPRITE_HPP

#include "AnimatedSprite.hpp"

#include "../SpriteAnimationManager.hpp"

class TimeAnimatedSprite : public AnimatedSprite {
private:
	float timePerFrame;
	float timeSpentCurrentFrame;

	bool playing;

public:
	TimeAnimatedSprite (int layerDepth)
		: DrawableBase(layerDepth), AnimatedSprite(layerDepth),
		timePerFrame(0),
		timeSpentCurrentFrame(0),
		playing(true) {}
	TimeAnimatedSprite (const SpriteAnimation* anim, int layerDepth)
		: DrawableBase(layerDepth), AnimatedSprite(anim, layerDepth),
		timePerFrame(1.0f / anim->FPS),
		timeSpentCurrentFrame(0),
		playing(true) {}
    TimeAnimatedSprite (TimeAnimatedSprite const& tAniSprite) : DrawableBase(layerDepth), AnimatedSprite(tAniSprite) {
		timePerFrame = tAniSprite.timePerFrame;
		timeSpentCurrentFrame = tAniSprite.timeSpentCurrentFrame;
		playing = tAniSprite.playing;
	}

	void Update (float dT);

	void SetAnimation (const SpriteAnimation* anim);

	void Reset () { ResetAnim(); timeSpentCurrentFrame = 0; }
	void Play () { Reset(); playing = true; }
	void Stop () { Reset(); playing = false; }
	void Continue () { playing = true; }
	void Pause () { playing = false; }
};

#endif // TIMEANIMATEDSPRITE_HPP
