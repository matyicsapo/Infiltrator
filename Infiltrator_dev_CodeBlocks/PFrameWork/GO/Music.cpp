#include "Music.hpp"

#include "../SoundManager.hpp"
#include "../Utilities.hpp"

Music::Music (std::string soundFile, unsigned int entityType, std::size_t BufferSize)
		: GameObject(entityType),
		BaseSound(),
		sfMusic(BufferSize) {

	Sounds->AddMusic(this);

	// sound spatialization intentionally disabled until everything is properly implemented
	sfMusic.SetRelativeToListener(true);
	sfMusic.SetPosition(0, 0, 0);

	if (soundFile != "") {
		sfMusic.OpenFromFile(soundFile);
	}
}

Music::~Music () {
	Sounds->PopMusic(this);
}

void Music::SetSound (std::string soundFile) {
	sfMusic.OpenFromFile(soundFile);
}

void Music::Play () {
	sfMusic.Play();
}

void Music::Pause () {
	sfMusic.Pause();
}

void Music::Stop () {
	sfMusic.Stop();
}

void Music::SetLoop (bool loop) {
	sfMusic.SetLoop(loop);
}
bool Music::GetLoop () {
	return sfMusic.GetLoop();
}

void Music::SetVolume (float volume) {
	sfMusic.SetVolume( volume * volume_base );
}
void Music::SetBaseVolume (float volume_base) {
	sfMusic.SetVolume( (sfMusic.GetVolume() / this->volume_base) * volume_base );

	BaseSound::SetBaseVolume(volume_base);

	Sounds->RefreshVol_Music_Of(this);
}
float Music::GetVolume () {
	return sfMusic.GetVolume();
}

sf::Sound::Status Music::GetStatus () {
	return sfMusic.GetStatus();
}
