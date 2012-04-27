#ifndef WORLDCAMERA2D_HPP
#define WORLDCAMERA2D_HPP

#include <SFML/Graphics.hpp>

class WorldCamera2D {
private:
	sf::View sfView;

	sf::Vector2f camCenterPos;
	sf::Vector2f worldCenter;

	float zoom;

	const float zoomLow;
	const float zoomHigh;

	float fakeScale;

	sf::Vector2f worldBoundsHalfSize;

	sf::Vector2f OnChangeCenterOfView (sf::Vector2f newCenterOfView);
//	float OnChangeZoom (float newZoom);

public:
	WorldCamera2D (sf::FloatRect floatRect, float zoomLow, float zoomHigh);

	void Reset (sf::RenderWindow& win);

	void SetFakeScale (float fakeScale) { this->fakeScale = fakeScale; }

	void SetCenterOfView (sf::Vector2f pos);
	void SetCenterOfView_X (float x);
	void SetCenterOfView_Y (float y);
	sf::Vector2f GetCenterOfView ();
	void Move (sf::Vector2f offset);

	void SetZoom (float zoom);
	void Zoom (float factor);
	float GetZoom ();

	void SetWorldBounds (sf::Vector2f halfSize);
	void SetWorldCenter (sf::Vector2f worldCenter);

	sf::Vector2f GetViewSize ();
	sf::FloatRect GetViewRect ();

	sf::View& GetSFView ();
};

#endif // WORLDCAMERA2D_HPP
