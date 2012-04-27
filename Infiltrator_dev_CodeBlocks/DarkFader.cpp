#include "DarkFader.hpp"

DarkFader::DarkFader (float duration
						, bool toDark
						, boost::function<void (void)> onFinished
						, int layerDepth)
		: DrawableBase(layerDepth)
		, FullScreenEventCatcherTint(layerDepth,
									toDark ? sf::Color(0, 0, 0, 0) : sf::Color(0, 0, 0, 255))
		, finished(false)
		, duration(duration)
		, timeLeft(duration)
		, toDark(toDark)
		, alpha(toDark ? 0 : 255)
		, onFinished(onFinished) {
}

void DarkFader::Update (float dT) {
	if (finished)
		return;

	timeLeft -= dT;

	if (timeLeft <= 0) {
		alpha = toDark ? 255 : 0;
		sfSprite->SetColor(sf::Color(0, 0, 0, alpha));

		finished = true;

		if (onFinished) {
			onFinished();
		}
	}
	else {
		alpha += (toDark ? 1 : -1) * (255 / duration) * dT;
		sfSprite->SetColor(sf::Color(0, 0, 0, alpha));
	}
}
