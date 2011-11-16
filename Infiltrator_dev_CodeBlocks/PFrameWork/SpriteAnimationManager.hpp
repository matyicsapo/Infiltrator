#ifndef SPRITEANIMATIONMANAGER_HPP
#define SPRITEANIMATIONMANAGER_HPP

#include <string>
#include <map>

struct SpriteAnimation {
	std::string animName;
	std::string fileName;
	int FPS;
	int frameCnt;
	int startX, startY;
	int width, height;
};

class SpriteAnimationManager {
private:
	std::map<std::string, SpriteAnimation*> animations;

    SpriteAnimationManager () {}

	~SpriteAnimationManager ();

	SpriteAnimationManager (SpriteAnimationManager const&);
	SpriteAnimationManager& operator= (SpriteAnimationManager const&);

public:
	static SpriteAnimationManager* Instance ();

	void LoadAnimations (std::string cfgFile);
	void UnloadAnimation (std::string animName);

	const SpriteAnimation* operator[] (std::string animName);
};

#endif // SPRITEANIMATIONMANAGER_HPP
