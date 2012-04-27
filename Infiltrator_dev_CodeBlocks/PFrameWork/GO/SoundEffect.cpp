#include "SoundEffect.hpp"

#include <SFML/Audio/Sound.hpp>

#include "../ResourceManager.hpp"
#include "../SoundManager.hpp"
#include "../Utilities.hpp"

SoundEffect::SoundEffect (std::string soundFile, unsigned int entityType)
		: sfSound(0),
		oneShot(false),
		GameObject(entityType),
		BaseSound() {

	Sounds->AddEffect(this);

	if (soundFile != "") {
		sf::SoundBuffer* soundBuff = Resources->GetSound(soundFile);

		if (soundBuff != 0) {
			sfSound = new sf::Sound(*soundBuff);

			// sound spatialization intentionally disabled until everything is properly implemented
			sfSound->SetRelativeToListener(true);
			sfSound->SetPosition(0, 0, 0);
		}
	}
	else
		sfSound = 0;
}

SoundEffect::SoundEffect (SoundEffect const& xSoundEffect)
		: GameObject(xSoundEffect.entityType),
		BaseSound() {

	Sounds->AddEffect(this);

	sfSound = new sf::Sound( *(xSoundEffect.sfSound) );
}

SoundEffect::~SoundEffect () {
	Sounds->PopEffect(this);

	delete sfSound;
}

void SoundEffect::ReleaseResources () {
	Resources->ReleaseSound(sfSound->GetBuffer());
	delete sfSound;
	sfSound = 0;
}

void SoundEffect::SetSound (std::string soundFile) {
	sf::SoundBuffer* soundBuff = Resources->GetSound(soundFile);
	if (soundBuff != 0) {
		if (sfSound == 0)
			sfSound = new sf::Sound(*soundBuff);
		else
			sfSound->SetBuffer(*soundBuff);

		// sound spatialization intentionally disabled until everything is properly implemented
		sfSound->SetRelativeToListener(true);
		sfSound->SetPosition(0, 0, 0);
	}
}

void SoundEffect::Play () {
	if (sfSound) {
		sfSound->Play();
	}
}

void SoundEffect::Pause () {
	if (!sfSound)
		return;

	sfSound->Pause();
}

void SoundEffect::Stop () {
	if (!sfSound)
		return;

	sfSound->Stop();
}

void SoundEffect::SetPlayingOffset (float TimeOffset) {
	if (!sfSound)
		return;

	sfSound->SetPlayingOffset(TimeOffset);
}

void SoundEffect::SetLoop (bool loop) {
	if (!sfSound)
		return;

	sfSound->SetLoop(loop);
}
bool SoundEffect::GetLoop () {
	if (!sfSound)
		return false;

	return sfSound->GetLoop();
}

void SoundEffect::SetVolume (float volume) {
	if (!sfSound)
		return;

	sfSound->SetVolume( volume * volume_base );
}
void SoundEffect::SetBaseVolume (float volume_base) {
	if (!sfSound)
		return;

	sfSound->SetVolume( (sfSound->GetVolume() / this->volume_base) * volume_base );

	BaseSound::SetBaseVolume(volume_base);

	Sounds->RefreshVol_Effect_Of(this);
}
float SoundEffect::GetVolume () {
	if (!sfSound)
		return 0;

	return sfSound->GetVolume();
}

sf::Sound::Status SoundEffect::GetStatus () {
	if (!sfSound)
		return sf::Sound::Stopped;

	return sfSound->GetStatus();
}
