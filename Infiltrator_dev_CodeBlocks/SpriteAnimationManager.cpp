#include "SpriteAnimationManager.hpp"

#include "CfgParser.hpp"

SpriteAnimationManager* SpriteAnimationManager::Instance () {
	static SpriteAnimationManager instance;

	return &instance;
}

SpriteAnimationManager::~SpriteAnimationManager () {
	for (std::map<std::string, SpriteAnimation*>::iterator itAnim = animations.begin(); itAnim != animations.end();
			itAnim++) {
		delete itAnim->second;
	}
}

void SpriteAnimationManager::LoadAnimations (std::string cfgFile) {
	CfgParser::CfgContents contents;
	CfgParser::Instance()->GetContents(cfgFile, contents);

	for (CfgParser::CfgContents::CfgSectionMap::iterator itSection = contents.GetIteratorBegin();
			itSection != contents.GetIteratorEnd(); itSection++) {

		SpriteAnimation* anim = new SpriteAnimation();
		anim->fileName = contents[(*itSection)->first]["file"];
		anim->FPS = Convert::Instance()->ToNum<int>(contents[(*itSection)->first]["FPS"]);
		anim->frameCnt = Convert::Instance()->ToNum<int>(contents[(*itSection)->first]["frameCnt"]);
		anim->startX = Convert::Instance()->ToNum<int>(contents[(*itSection)->first]["startX"]);
		anim->startY = Convert::Instance()->ToNum<int>(contents[(*itSection)->first]["startY"]);
		anim->width = Convert::Instance()->ToNum<int>(contents[(*itSection)->first]["width"]);
		anim->height = Convert::Instance()->ToNum<int>(contents[(*itSection)->first]["height"]);

		animations.insert( std::pair<std::string, SpriteAnimation*>((*itSection)->first, anim) );
	}
}

void SpriteAnimationManager::UnloadAnimation (std::string animName) {
	animations.erase(animName);
}

const SpriteAnimation* SpriteAnimationManager::operator[] (std::string animName) {
	return animations[animName];
}
