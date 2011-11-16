#ifndef WORLDCAMERA2D_HPP
#define WORLDCAMERA2D_HPP

#include <SFML/Graphics.hpp>

class WorldCamera2D {
private:
	sf::View sfView;

	sf::Vector2f wCenter;

	float zoom;

	const float zoomLow;
	const float zoomHigh;

	float fakeScale;

public:
	WorldCamera2D (sf::FloatRect floatRect, float zoomLow, float zoomHigh);

	void Reset (sf::RenderWindow& win);

	void SetFakeScale (float fakeScale) { if (fakeScale != 0) this->fakeScale = fakeScale; }

	void SetCenter (sf::Vector2f pos);

	void Move (sf::Vector2f offset);

	void Zoom (float factor);

	sf::View& GetSfView () { return sfView; }
};

#endif // WORLDCAMERA2D_HPP
