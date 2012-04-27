#include "SpriteAnimationManager.hpp"

#include "ConfigParser.hpp"

SpriteAnimationManager* SpriteAnimationManager::instance = 0;

SpriteAnimationManager* SpriteAnimationManager::Instance () {
	if (instance == 0)
		instance = new SpriteAnimationManager();

	return instance;
}

void SpriteAnimationManager::Release () {
	delete instance;
	instance = 0;
}

SpriteAnimationManager::~SpriteAnimationManager () {
	for (std::map<std::string, SpriteAnimation*>::iterator itAnim = animations.begin(); itAnim != animations.end();
			itAnim++) {
		delete itAnim->second;
	}
}

void SpriteAnimationManager::LoadAnimations (std::string cfgFile) {
	CfgContents contents;
	CfgParser->GetContents(cfgFile, contents);

	for (CfgSectionMap::iterator itSection = contents.GetIteratorBegin();
			itSection != contents.GetIteratorEnd(); itSection++) {
		SpriteAnimation* anim = new SpriteAnimation();
		anim->fileName = contents[(*itSection)->first]["file"];
		anim->FPS = Convert->ToNum<int>(contents[(*itSection)->first]["FPS"]);
		anim->frameCnt = Convert->ToNum<int>(contents[(*itSection)->first]["frameCnt"]);
		anim->startX = Convert->ToNum<int>(contents[(*itSection)->first]["startX"]);
		anim->startY = Convert->ToNum<int>(contents[(*itSection)->first]["startY"]);
		anim->width = Convert->ToNum<int>(contents[(*itSection)->first]["width"]);
		anim->height = Convert->ToNum<int>(contents[(*itSection)->first]["height"]);

		animations.insert( std::pair<std::string, SpriteAnimation*>((*itSection)->first, anim) );
	}
}

void SpriteAnimationManager::UnloadAnimation (std::string animName) {
	animations.erase(animName);
}

const SpriteAnimation* SpriteAnimationManager::operator[] (std::string animName) {
	return animations[animName];
}
