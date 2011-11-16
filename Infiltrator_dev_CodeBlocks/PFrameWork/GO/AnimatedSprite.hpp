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
    AnimatedSprite (AnimatedSprite const& aniSprite) : DrawableBase(layerDepth), Sprite(aniSprite)
		{ sfSprite = new sf::Sprite( *(aniSprite.sfSprite) ); }

	virtual void SetImage (std::string textureFile) {} // doesn't make sense here

	void SetAnimation (const SpriteAnimation* anim);
	const SpriteAnimation* GetAnimation () { return curAnim; }

	void ChangeFrame (int change = 1);
	void ResetAnim () { curFrame = 0; }

	virtual sf::Vector2f GetScreenSize ();
};

#endif // ANIMATEDSPRITE_HPP
