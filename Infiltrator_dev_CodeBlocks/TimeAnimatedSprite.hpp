#ifndef TIMEANIMATEDSPRITE_HPP
#define TIMEANIMATEDSPRITE_HPP

#include "AnimatedSprite.hpp"

class TimeAnimatedSprite : public AnimatedSprite {
protected:
	float timePerFrame;
	float timeSpentCurrentFrame;

public:
	TimeAnimatedSprite () : AnimatedSprite(), timePerFrame(1), timeSpentCurrentFrame(0) {}

	TimeAnimatedSprite (std::string fileName) : AnimatedSprite(fileName), timePerFrame(1.0f / FPS), timeSpentCurrentFrame(0) {}

	virtual void Update ();
};

#endif
