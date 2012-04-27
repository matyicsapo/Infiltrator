#ifndef ONESHOTSFX_HPP
#define ONESHOTSFX_HPP

#include "SoundEffect.hpp"

class OneShotSFX : public SoundEffect {
public:
	OneShotSFX (std::string soundFile = "", unsigned int entityType = 0);
    OneShotSFX (OneShotSFX const& xOneShotSFX);

    virtual ~OneShotSFX ();
};

#endif // ONESHOTSFX_HPP
