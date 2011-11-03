#include "AnimatedSprite.hpp"

#include "SpriteAnimationManager.hpp"

AnimatedSprite::AnimatedSprite (const SpriteAnimation* anim, float layerDepth) : DrawableBase(layerDepth),
		Sprite(anim->fileName, layerDepth),
		curAnim(anim), curFrame(0) {
	sfSprite->SetCenter(curAnim->width / 2, curAnim->height / 2);
	SetSubRect();
}

void AnimatedSprite::SetSubRect () {
	int left = curFrame * curAnim->width;
	int right = left + curAnim->width;

	int l = int(right / sfSprite->GetImage()->GetWidth());
	if (right % sfSprite->GetImage()->GetWidth() == 0) {
		l--;

		if (l > 0) {
			left %= sfSprite->GetImage()->GetWidth();
			right = right - sfSprite->GetImage()->GetWidth() * l;
		}
	}
	else {
		left %= sfSprite->GetImage()->GetWidth();
		right %= sfSprite->GetImage()->GetWidth();
	}

	int top = l * curAnim->height;
	int bottom = top + curAnim->height;

	sf::Rect<int> intRect(left, top, right, bottom);
	sfSprite->SetSubRect(intRect);
}

void AnimatedSprite::SetAnimation (const SpriteAnimation* anim) {
	curAnim = anim;
	sfSprite->SetCenter(curAnim->width / 2, curAnim->height / 2);
	SetSubRect();
}

void AnimatedSprite::ChangeFrame (int change) {
	curFrame = (curFrame + change) % (curAnim->frameCnt);

    SetSubRect();

}

sf::Vector2f AnimatedSprite::GetScreenSize () {
	return sf::Vector2f(curAnim->width * sfSprite->GetScale().x, curAnim->height * sfSprite->GetScale().y);
}
