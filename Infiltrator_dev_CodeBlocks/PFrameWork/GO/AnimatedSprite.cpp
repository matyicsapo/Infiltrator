#include "AnimatedSprite.hpp"

#include <cmath>
#include "../SpriteAnimationManager.hpp"
//#include <iostream>

AnimatedSprite::AnimatedSprite (int layerDepth) : DrawableBase(layerDepth), Sprite(layerDepth), curAnim(0), curFrame(0) {}

AnimatedSprite::AnimatedSprite (const SpriteAnimation* anim, int layerDepth) : DrawableBase(layerDepth),
		Sprite(anim->fileName, layerDepth),
		curAnim(anim), curFrame(0) {
	sfSprite->SetCenter(curAnim->width / 2, curAnim->height / 2);
	SetSubRect();
}

void AnimatedSprite::SetSubRect () {
/* attempt at vertical..not sure if failed
int top = curFrame * curAnim->height;
int bottom = top + curAnim->height;

int c = int(bottom / sfSprite->GetImage()->GetHeight());
if (bottom % sfSprite->GetImage()->GetHeight() == 0) {
	c--;

	if (c > 0) {
		top %= sfSprite->GetImage()->GetHeight();
		bottom = bottom - sfSprite->GetImage()->GetHeight() * c;
	}
}
else {
	top %= sfSprite->GetImage()->GetHeight();
	bottom %= sfSprite->GetImage()->GetHeight();
}

int left = c * curAnim->width;
int right = left + curAnim->width;
*/

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
	Sprite::SetImage(anim->fileName);
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

bool AnimatedSprite::Contains (sf::Vector2f globalPosition) {
	sf::Vector2f size(curAnim->width * GetScale().x,
						curAnim->width * GetScale().y);

	return std::abs(globalPosition.x - basePos.x) <= size.x / 2
			&&
		std::abs(globalPosition.y - basePos.y) <= size.y / 2;
}

bool AnimatedSprite::IsOpaque (sf::Vector2f globalPosition) {
	// a 2 megoldás azonos

	sf::Matrix3 matrixToGlobal;
	matrixToGlobal.SetFromTransformations(sf::Vector2f(0, 0), GetPosition(), GetRotation(), GetScale());
	sf::Matrix3 matrixToLocal = matrixToGlobal.GetInverse();
	sf::Vector2f localPosition = matrixToLocal.Transform(globalPosition) / windowFakeScale;

//	sf::Vector2f localPosition = sfSprite->TransformToLocal(globalPosition);

	// a plusz értékek nem biztos, hogy jók - topleft 0,0 origin-ú 1. képkockával tesztelve csak
	return sfSprite->GetPixel(localPosition.x + curAnim->width / 2, localPosition.y + curAnim->height / 2).a != 0;
}
