#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <list>

class Music;
class SoundEffect;

class SoundManager {
private:
	float vol_master;
	float vol_music;
	float vol_effect;

	static SoundManager* instance;

	std::list<Music*> mMusic;
	std::list<SoundEffect*> mEffects;

    SoundManager ();

	~SoundManager ();

	SoundManager (SoundManager const&);
	SoundManager& operator= (SoundManager const&);

public:
	static SoundManager* Instance ();

	static void Release ();

	void Update ();

	void AddMusic (Music* s);
	void AddEffect (SoundEffect* s);
	void PopMusic (Music* s);
	void PopEffect (SoundEffect* s);

	void ReleaseAllMusic ();
	void ReleaseAllEffects ();

	void SetVol_Master (float vol);
	void SetVol_Music (float vol);
	void SetVol_Effect (float vol);
	float GetVol_Master ();
	float GetVol_Music ();
	float GetVol_Effect ();

	void RefreshVol_Music_Of (Music* s);
	void RefreshVol_Effect_Of (SoundEffect* s);
};

#define Sounds SoundManager::Instance()

#endif // SOUNDMANAGER_HPP
