#include "AnimatedSprite.hpp"

#include "CfgParser.hpp"
#include "Convert.hpp"

AnimatedSprite::AnimatedSprite (std::string fileName) : Sprite(fileName) {
	CfgParser::CfgContents contents;
	bool found = CfgParser::Instance()->GetContents("Content/Textures/Animated/animated.ini", contents);

	if (found) {
		for (CfgParser::CfgContents::CfgSectionMap::iterator itSection = contents.GetIteratorBegin();
				itSection != contents.GetIteratorEnd(); itSection++) {

			if (contents[(*itSection)->first]["path"] == fileName) {
				frameWidth = Convert::Instance()->ToNum<unsigned int>(contents[(*itSection)->first]["frameWidth"]);
				FPS = Convert::Instance()->ToNum<unsigned int>(contents[(*itSection)->first]["FPS"]);

				if (FPS == 0)
					FPS = 1;

				sfSprite->SetCenter(frameWidth / 2, sfSprite->GetImage()->GetHeight() / 2);

				numFrames = sfSprite->GetImage()->GetWidth() / frameWidth;
				currentFrame = 0;
				SetSubRect();

				found = true;
				break;
			}
		}
	}

	if (!found) {
		frameWidth = 1;
		numFrames = 1;
		currentFrame = 0;
		FPS = 1;
	}
}

void AnimatedSprite::ChangeFrame (int change) {
    currentFrame += change;
    if (currentFrame >= numFrames) {
        currentFrame = 0;
    }

    SetSubRect();
}

void AnimatedSprite::SetSubRect () {
    int left = frameWidth * currentFrame;
    int top = 0;
    int right = left + frameWidth;
    int bottom = 0 + sfSprite->GetImage()->GetHeight();
    sf::Rect<int> intRect(left, top, right, bottom);
    sfSprite->SetSubRect(intRect);
}
