#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include "Sprite.hpp"

#include "Updateable.hpp"

class AnimatedSprite : public Sprite, public Updateable {
protected:
	unsigned int frameWidth;
    unsigned int numFrames;
    unsigned int currentFrame;
    unsigned int FPS;

    void SetSubRect ();

    void ChangeFrame (int change = 1);

public:
    AnimatedSprite () : Sprite(), frameWidth(1), numFrames(1), currentFrame(0), FPS(1) {}

	AnimatedSprite (std::string fileName);

    virtual void Update () = 0;
};

#endif
