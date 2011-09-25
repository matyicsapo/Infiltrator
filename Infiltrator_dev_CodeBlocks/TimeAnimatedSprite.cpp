#include "TimeAnimatedSprite.hpp"

void TimeAnimatedSprite::Update () {
	timeSpentCurrentFrame += sfWin->GetFrameTime();
	if (timeSpentCurrentFrame > timePerFrame) {
		timeSpentCurrentFrame -= timePerFrame;

		ChangeFrame();
	}
}
