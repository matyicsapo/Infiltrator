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
	static SpriteAnimationManager* instance;

	std::map<std::string, SpriteAnimation*> animations;

    SpriteAnimationManager () {}

	~SpriteAnimationManager ();

	SpriteAnimationManager (SpriteAnimationManager const&);
	SpriteAnimationManager& operator= (SpriteAnimationManager const&);

	SpriteAnimationManager* mInstance;

public:
	static SpriteAnimationManager* Instance ();

	static void Release ();

	void LoadAnimations (std::string cfgFile);
	void UnloadAnimation (std::string animName);

	const SpriteAnimation* operator[] (std::string animName);
};

#endif // SPRITEANIMATIONMANAGER_HPP
