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
	AnimatedSprite (const SpriteAnimation* anim, float layerDepth);
    AnimatedSprite (AnimatedSprite const& aniSprite) : DrawableBase(layerDepth), Sprite(aniSprite)
		{ sfSprite = new sf::Sprite( *(aniSprite.sfSprite) ); }

	virtual void SetImage (std::string textureFile) {} // doesn't make sense here

	void SetAnimation (const SpriteAnimation* anim);

	void ChangeFrame (int change = 1);

	virtual sf::Vector2f GetScreenSize ();
};

#endif // ANIMATEDSPRITE_HPP
