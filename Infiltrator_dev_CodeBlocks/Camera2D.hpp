#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

#include <SFML/Graphics.hpp>

class Camera2D {
private:
	sf::View sfView;

	sf::Vector2f center;

	float zoom;

	const float zoomLow;
	const float zoomHigh;

	float fakeScale;

public:
	Camera2D (sf::FloatRect floatRect, float zoomLow, float zoomHigh) : zoom(1),
																		zoomLow(zoomLow),
																		zoomHigh(zoomHigh),
																		fakeScale(1) {
		sfView.SetFromRect(floatRect);
		center = sfView.GetCenter();
	}

	void Reset (sf::RenderWindow& win) {
		sfView.SetFromRect(sf::FloatRect(0, 0, win.GetWidth(), win.GetHeight()));
		sfView.SetCenter(center);
		sfView.Zoom(zoom);
	}

	void SetFakeScale (float fakeScale) { if (fakeScale != 0) this->fakeScale = fakeScale; }

	void Move (sf::Vector2f offset) {
		sfView.Move(offset * fakeScale);
		center = sfView.GetCenter();
	}

	void Zoom (float factor) {
		float borderFactor;
		if (factor > 1) {
			borderFactor = zoomHigh / zoom;
			if (factor > borderFactor)
				factor = borderFactor;
		}
		else {
			borderFactor = zoomLow / zoom;
			if (factor < borderFactor)
				factor = borderFactor;
		}

		zoom *= factor;

		sfView.Zoom(factor);
	}

	sf::View& GetSfView () { return sfView; }
};

#endif // CAMERA2D_HPP
