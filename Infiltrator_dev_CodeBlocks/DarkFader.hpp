#ifndef DARKFADER_HPP
#define DARKFADER_HPP

#include "FullScreenEventCatcherTint.hpp"
#include "boost/function.hpp"

class DarkFader : public FullScreenEventCatcherTint {
protected:
	bool finished;

	float duration;
	float timeLeft;

	bool toDark;

	float alpha;

	boost::function<void (void)> onFinished;

public:
	DarkFader (float duration, bool toDark, boost::function<void (void)> onFinished, int layerDepth);

	void Update (float dT);

	bool ToDark () { return toDark; }
	bool Finished () { return finished; }
	void SetFinished (bool finished) { this->finished = finished; }
};

#endif // DARKFADER_HPP
