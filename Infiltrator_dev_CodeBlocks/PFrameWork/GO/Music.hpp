#ifndef MUSIC_HPP
#define MUSIC_HPP

#include "BaseSound.hpp"

#include <SFML/Audio/Music.hpp>

class Music : public BaseSound {
protected:
	sf::Music sfMusic;

public:
	const sf::Music& GetSFMusic () { return sfMusic; }

	Music (std::string soundFile = "", unsigned int entityType = 0, std::size_t BufferSize = 44100);

	virtual ~Music ();

    virtual void ReleaseResources () {}

    virtual void SetSound (std::string soundFile);

    virtual void Play ();
    virtual void Pause ();
    virtual void Stop ();

    virtual void SetLoop (bool loop);
	virtual bool GetLoop ();

	virtual void SetVolume (float volume);
	virtual void SetBaseVolume (float volume_base);
	virtual float GetVolume ();

    virtual sf::Sound::Status GetStatus ();
};

#endif // MUSIC_HPP
