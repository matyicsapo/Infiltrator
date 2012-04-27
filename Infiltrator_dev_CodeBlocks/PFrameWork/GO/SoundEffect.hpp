#ifndef SOUNDEFFECT_HPP
#define SOUNDEFFECT_HPP

#include "BaseSound.hpp"

namespace sf {
	class Sound;
};

class SoundEffect : public BaseSound {
protected:
	sf::Sound* sfSound;

public:
	bool oneShot;

	SoundEffect (std::string soundFile = "", unsigned int entityType = 0);
    SoundEffect (SoundEffect const& xSoundEffect);

    virtual ~SoundEffect ();

    virtual void ReleaseResources ();

    virtual void SetSound (std::string soundFile);

    virtual void Play ();
    virtual void Pause ();
    virtual void Stop ();

    void SetPlayingOffset (float TimeOffset);

    virtual void SetLoop (bool loop);
	virtual bool GetLoop ();

	virtual void SetVolume (float volume);
	virtual void SetBaseVolume (float volume_base);
	virtual float GetVolume ();

    virtual sf::Sound::Status GetStatus ();
};

#endif // SOUNDEFFECT_HPP
