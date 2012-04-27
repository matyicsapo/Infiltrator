#ifndef FULLSCREENEVENTCATCHERTINT_HPP
#define FULLSCREENEVENTCATCHERTINT_HPP

#include "PFrameWork/GO/Sprite.hpp"
#include "PFrameWork/GO/ScreenSpaceDrawable.hpp"
#include "PFrameWork/SFMLEventHandler.hpp"

class FullScreenEventCatcherTint : public ScreenSpaceDrawable, public Sprite, public SFMLEventHandler {
private:
	void OnResize (int width, int height);

public:
	FullScreenEventCatcherTint (int layerDepth, sf::Color color = sf::Color(0, 0, 0, 128));

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);
};

#endif // FULLSCREENEVENTCATCHERTINT_HPP
