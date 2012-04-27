#include "SoundManager.hpp"

#include "GO/SoundEffect.hpp"
#include "GO/Music.hpp"

SoundManager* SoundManager::instance = 0;

SoundManager::SoundManager () : vol_master(1), vol_music(1), vol_effect(1) {}

SoundManager* SoundManager::Instance () {
	if (instance == 0)
		instance = new SoundManager();

	return instance;
}

void SoundManager::Release () {
	delete instance;
	instance = 0;
}

void SoundManager::Update () {
	for (std::list<SoundEffect*>::iterator itS = mEffects.begin(); itS != mEffects.end(); itS++) {
		if ((*itS)->oneShot && (*itS)->GetStatus() == sf::Sound::Stopped) {
			delete *itS--;
		}
	}
}

SoundManager::~SoundManager () {
	for (std::list<Music*>::iterator itS = mMusic.begin(); itS != mMusic.end(); itS++) {
		delete *itS--;
	}

	for (std::list<SoundEffect*>::iterator itS = mEffects.begin(); itS != mEffects.end(); itS++) {
		delete *itS--;
	}
}

void SoundManager::AddMusic (Music* s) {
	mMusic.push_back(s);
}
void SoundManager::AddEffect (SoundEffect* s) {
	mEffects.push_back(s);
}

void SoundManager::PopMusic (Music* s) {
	mMusic.remove(s);
}
void SoundManager::PopEffect (SoundEffect* s) {
	mEffects.remove(s);
}

void SoundManager::ReleaseAllMusic () {
	for (std::list<Music*>::iterator itS = mMusic.begin(); itS != mMusic.end(); itS++) {
		delete *itS;
	}
}
void SoundManager::ReleaseAllEffects () {
	for (std::list<SoundEffect*>::iterator itS = mEffects.begin(); itS != mEffects.end(); itS++) {
		delete *itS;
	}
}

void SoundManager::SetVol_Master (float vol) {
	vol_master = vol;

	for (std::list<Music*>::iterator itS = mMusic.begin(); itS != mMusic.end(); itS++) {
		(*itS)->SetVolume( vol_music * vol_master );
	}

	for (std::list<SoundEffect*>::iterator itS = mEffects.begin(); itS != mEffects.end(); itS++) {
		(*itS)->SetVolume( vol_effect * vol_master );
	}
}
void SoundManager::SetVol_Music (float vol) {
	vol_music = vol;

	for (std::list<Music*>::iterator itS = mMusic.begin(); itS != mMusic.end(); itS++) {
		(*itS)->SetVolume( vol_music * vol_master );
	}
}
void SoundManager::SetVol_Effect (float vol) {
	vol_effect = vol;

	for (std::list<SoundEffect*>::iterator itS = mEffects.begin(); itS != mEffects.end(); itS++) {
		(*itS)->SetVolume( vol_effect * vol_master );
	}
}
float SoundManager::GetVol_Master () { return vol_master; }
float SoundManager::GetVol_Music () { return vol_music; }
float SoundManager::GetVol_Effect () { return vol_effect; }

void SoundManager::RefreshVol_Music_Of (Music* s) {
	s->SetVolume( vol_music * vol_master );
}
void SoundManager::RefreshVol_Effect_Of (SoundEffect* s) {
	s->SetVolume( vol_effect * vol_master );
}
