#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include "Sprite.hpp"

class SpriteAnimation;

class AnimatedSprite : public Sprite {
private:
	const SpriteAnimation* curAnim;

	int curFrame; // 0 indexed

	void SetSubRect ();

public:
	AnimatedSprite (int layerDepth);
	AnimatedSprite (const SpriteAnimation* anim, int layerDepth);
    AnimatedSprite (AnimatedSprite const& aniSprite) : DrawableBase(layerDepth), Sprite(aniSprite) {
    	curAnim = aniSprite.curAnim;
    	curFrame = aniSprite.curFrame;
	}

	virtual void SetImage (std::string textureFile) {} // doesn't make sense here

	void SetAnimation (const SpriteAnimation* anim);
	const SpriteAnimation* GetAnimation () { return curAnim; }

	void ChangeFrame (int change = 1);
	void ResetAnim () { curFrame = 0; }

	virtual sf::Vector2f GetScreenSize ();

	virtual bool Contains (sf::Vector2f globalPosition);
	virtual bool IsOpaque (sf::Vector2f globalPosition);
};

#endif // ANIMATEDSPRITE_HPP
