#include "OneShotSFX.hpp"

#include "../SoundManager.hpp"

OneShotSFX::OneShotSFX (std::string soundFile, unsigned int entityType)
		: SoundEffect(soundFile, entityType) {
}
OneShotSFX::OneShotSFX (OneShotSFX const& xOneShotSFX) : SoundEffect(xOneShotSFX) {
}

OneShotSFX::~OneShotSFX () {
}
