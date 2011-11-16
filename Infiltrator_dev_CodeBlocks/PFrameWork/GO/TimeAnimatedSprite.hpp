#ifndef TIMEANIMATEDWORLDSPRITE_HPP
#define TIMEANIMATEDWORLDSPRITE_HPP

#include "AnimatedSprite.hpp"

class TimeAnimatedSprite {
private:
	AnimatedSprite* animSprite;

	float timePerFrame;
	float timeSpentCurrentFrame;

	bool playing;

public:
	TimeAnimatedSprite (AnimatedSprite* animSprite);

	~TimeAnimatedSprite ();

	void Update (float dT);

	void SetAnimation (const SpriteAnimation* anim);

	void ResetTimeAnim () { animSprite->ResetAnim(); timeSpentCurrentFrame = 0; }
	void Play () { ResetTimeAnim(); playing = true; }
	void Stop () { ResetTimeAnim(); playing = false; }
	void Continue () { playing = true; }
	void Pause () { playing = false; }
};

#endif // TIMEANIMATEDWORLDSPRITE_HPP
