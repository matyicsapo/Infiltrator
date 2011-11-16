#include "WorldCamera2D.hpp"

WorldCamera2D::WorldCamera2D (sf::FloatRect floatRect, float zoomLow, float zoomHigh)
		: zoom(1),
		zoomLow(zoomLow),
		zoomHigh(zoomHigh),
		fakeScale(1) {

	sfView.SetFromRect(floatRect);

	wCenter = sf::Vector2f(0, 0);
	sfView.SetCenter(wCenter);
}

void WorldCamera2D::Reset (sf::RenderWindow& win) {
	sfView.SetFromRect(sf::FloatRect(0, 0, win.GetWidth(), win.GetHeight()));
	sfView.SetCenter(wCenter * fakeScale);
	sfView.Zoom(zoom);
}

void WorldCamera2D::SetCenter (sf::Vector2f pos) {
	wCenter = pos;
	sfView.SetCenter(wCenter * fakeScale);
}

void WorldCamera2D::Move (sf::Vector2f offset) {
	sfView.Move(offset * fakeScale);
	wCenter += offset;
	//sfView.SetCenter(wCenter * fakeScale); // same effect
}

void WorldCamera2D::Zoom (float factor) {
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
