#ifndef BASESOUND_HPP
#define BASESOUND_HPP

#include "GameObject.hpp"
#include <SFML/Audio.hpp>
#include <string>

//#include "../SoundManager.hpp"

class BaseSound : virtual public GameObject {
protected:
	float volume_base;

	virtual sf::Vector2f CalcFakePos () { return sf::Vector2f(0, 0); }

	virtual void SetFakeScale (sf::Vector2f fakeScale) {}
	virtual void SetFakePos (sf::Vector2f fakePos) {}

public:
	BaseSound () : volume_base(100) {}
//	virtual ~BaseSound () { Sounds->Pop(this); }

	virtual void SetSound (std::string soundFile) = 0;

	virtual void Play () = 0;
	virtual void Pause () = 0;
	virtual void Stop () = 0;

	virtual void SetLoop (bool loop) = 0;
	virtual bool GetLoop () = 0;

	virtual void SetVolume (float volume) = 0;
	virtual void SetBaseVolume (float volume_base) { this->volume_base = volume_base; };
	virtual float GetVolume () = 0;
	float GetBaseVolume () { return volume_base; };

	virtual sf::Sound::Status GetStatus () = 0;

	virtual void SetRotation (float rotation) {}
	virtual float GetRotation () { return 0; }
	virtual void Rotate (float angle) {}
};

#endif // BASESOUND_HPP
