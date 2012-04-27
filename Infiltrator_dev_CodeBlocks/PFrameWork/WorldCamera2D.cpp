#include "WorldCamera2D.hpp"


				//#include <iostream>


WorldCamera2D::WorldCamera2D (sf::FloatRect floatRect, float zoomLow, float zoomHigh)
		: zoom(1),
		zoomLow(zoomLow),
		zoomHigh(zoomHigh),
		fakeScale(1),
		worldBoundsHalfSize(0, 0) {

	sfView.SetFromRect(floatRect);

	worldCenter = sf::Vector2f(0, 0);
	camCenterPos = sf::Vector2f(0, 0);
	sfView.SetCenter(camCenterPos);
}

void WorldCamera2D::Reset (sf::RenderWindow& win) {
	sfView.SetFromRect(sf::FloatRect(0, 0, win.GetWidth(), win.GetHeight()));
	sfView.SetCenter(camCenterPos * fakeScale);
	sfView.Zoom(zoom);
}

void WorldCamera2D::SetCenterOfView (sf::Vector2f pos) {
	camCenterPos = OnChangeCenterOfView(pos);
	sfView.SetCenter(camCenterPos * fakeScale);
}

void WorldCamera2D::SetCenterOfView_X (float x) {
	camCenterPos = OnChangeCenterOfView(sf::Vector2f(x, camCenterPos.y));
	sfView.SetCenter(camCenterPos * fakeScale);
}

void WorldCamera2D::SetCenterOfView_Y (float y) {
	camCenterPos = OnChangeCenterOfView(sf::Vector2f(camCenterPos.x, y));
	sfView.SetCenter(camCenterPos * fakeScale);
}

sf::Vector2f WorldCamera2D::GetCenterOfView () {
	return sfView.GetCenter();
}

void WorldCamera2D::Move (sf::Vector2f offset) {
	camCenterPos = OnChangeCenterOfView(camCenterPos + offset);
	sfView.SetCenter(camCenterPos * fakeScale);
}

void WorldCamera2D::SetZoom (float zoom) {
	float targetZoom;
	if (zoom < zoomLow)
		targetZoom = zoomLow;
	else
	if (zoom > zoomHigh)
		targetZoom = zoomHigh;
	else
		targetZoom = zoom;

//	targetZoom = OnChangeZoom(targetZoom);

	float factor = targetZoom / this->zoom;

	sfView.Zoom(factor);

	this->zoom = targetZoom;
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

//	float targetZoom = OnChangeZoom(zoom * factor);
//
//	float finalfactor = targetZoom / zoom;
//
//	sfView.Zoom(finalfactor);

	sfView.Zoom(factor);

//	zoom = targetZoom;

	zoom *= factor;
}

float WorldCamera2D::GetZoom () {
	return zoom;
}

void WorldCamera2D::SetWorldBounds (sf::Vector2f halfSize) {
	worldBoundsHalfSize = halfSize;
}

void WorldCamera2D::SetWorldCenter (sf::Vector2f worldCenter) {
	this->worldCenter = worldCenter;
}

sf::View& WorldCamera2D::GetSFView () {
	return sfView;
}

//sf::Vector2f WorldCamera2D::OnChangeCenterOfView (sf::Vector2f newCenterOfView) {
//	if (worldBoundsHalfSize == sf::Vector2f(0, 0))
//		return newCenterOfView;
//
//	if (newCenterOfView.x - sfView.GetRect().GetWidth() / 2 < -worldBoundsHalfSize.x) {
//		newCenterOfView.x = -worldBoundsHalfSize.x + sfView.GetRect().GetWidth() / 2;
//	}
//	else
//	if (newCenterOfView.x + sfView.GetRect().GetWidth() / 2 > worldBoundsHalfSize.x) {
//		newCenterOfView.x = worldBoundsHalfSize.x - sfView.GetRect().GetWidth() / 2;
//	}
//
//	if (newCenterOfView.y - sfView.GetRect().GetHeight() / 2 < -worldBoundsHalfSize.y) {
//		newCenterOfView.y = -worldBoundsHalfSize.y + sfView.GetRect().GetHeight() / 2;
//	}
//	else
//	if (newCenterOfView.y + sfView.GetRect().GetHeight() / 2 > worldBoundsHalfSize.y) {
//		newCenterOfView.y = worldBoundsHalfSize.y - sfView.GetRect().GetHeight() / 2;
//	}
//
//	return newCenterOfView;
//}

sf::Vector2f WorldCamera2D::OnChangeCenterOfView (sf::Vector2f newCenterOfView) {
	if (worldBoundsHalfSize == sf::Vector2f(0, 0))
		return newCenterOfView;

	if (newCenterOfView.x < worldCenter.x / fakeScale - worldBoundsHalfSize.x / fakeScale) {
		newCenterOfView.x = worldCenter.x / fakeScale - worldBoundsHalfSize.x / fakeScale;
	}
	else
	if (newCenterOfView.x > worldCenter.x / fakeScale + worldBoundsHalfSize.x / fakeScale) {
		newCenterOfView.x = worldCenter.x / fakeScale + worldBoundsHalfSize.x / fakeScale;
	}

	if (newCenterOfView.y < worldCenter.y / fakeScale - worldBoundsHalfSize.y / fakeScale) {
		newCenterOfView.y = worldCenter.y / fakeScale - worldBoundsHalfSize.y / fakeScale;
	}
	else
	if (newCenterOfView.y > worldCenter.y / fakeScale + worldBoundsHalfSize.y / fakeScale) {
		newCenterOfView.y = worldCenter.y / fakeScale + worldBoundsHalfSize.y / fakeScale;
	}

	return newCenterOfView;
}

//float WorldCamera2D::OnChangeZoom (float newZoom) {
//	if (worldBoundsHalfSize == sf::Vector2f(0, 0) || newZoom > zoom)
//		return newZoom;
//
//	return newZoom;
//}

//float WorldCamera2D::OnChangeZoom (float newZoom) {
//	// TICK
//	sf::Vector2f windowSize = sf::Vector2f(sfView.GetRect().GetWidth(), sfView.GetRect().GetHeight()) * zoom;
//
//	// TICK
//	sf::Vector2f newViewDimensions = windowSize / newZoom;
//
////std::cout << "view: " << sfView.GetRect().GetWidth() << " " << sfView.GetRect().GetHeight() << std::endl;
//////std::cout << Game->GetRenderWindow()->GetWidth() << " " << Game->GetRenderWindow()->GetHeight() << std::endl;
////std::cout << "window: " << windowSize.x << " " <<  windowSize.y << std::endl;
////std::cout << "zoom: " << zoom << std::endl;
////std::cout << "newZoom: " << newZoom << std::endl;
////std::cout << "newViewDimensions: " << newViewDimensions.x << " " <<  newViewDimensions.y << std::endl;
//
////	if (worldBoundsHalfSize == sf::Vector2f(0, 0) || newZoom > zoom)
////		return newZoom;
//
////	sf::Vector2f windowSize = sf::Vector2f(sfView.GetRect().GetWidth(), sfView.GetRect().GetHeight()) * zoom;
////	sf::Vector2f newViewDimensions = windowSize / newZoom; // => zoom = windowSize / viewDimensions
//
//	if (wCenter.x - newViewDimensions.x / 2 < -worldBoundsHalfSize.x) {
//		wCenter = sf::Vector2f(-worldBoundsHalfSize.x + newViewDimensions.x / 2, wCenter.y);
//		sfView.SetCenter(wCenter * fakeScale);
//
//		if (wCenter.x + newViewDimensions.x / 2 > worldBoundsHalfSize.x) {
//			std::cout << "a c" << std::endl;
//
//			// dimensions.x = 2 * (worldBoundsHalfSize.x - wCenter.x)
//			// zoom = windowSize / dimensions
//
//			float limitDimensionsX = 2 * (wCenter.x - worldBoundsHalfSize.x);
//			newZoom = std::abs(windowSize.x / limitDimensionsX);
////			newZoom *= 1.1f;
//
////std::cout << newZoom << std::endl;
//		}
//	}
//	else
//	if (wCenter.x + newViewDimensions.x / 2 > worldBoundsHalfSize.x) {
//		wCenter = sf::Vector2f(worldBoundsHalfSize.x - newViewDimensions.x / 2, wCenter.y);
//		sfView.SetCenter(wCenter * fakeScale);
//
//		if (wCenter.x - newViewDimensions.x / 2 < -worldBoundsHalfSize.x) {
//std::cout << "b c" << std::endl;
//
////std::cout << (wCenter.x - newViewDimensions.x / 2) << std::endl;
////std::cout << (-worldBoundsHalfSize.x) << std::endl;
//
//			float limitDimensionsX = 2 * (worldBoundsHalfSize.x + wCenter.x);
//
////			if (wCenter.x - limitDimensionsX / 2 < -worldBoundsHalfSize.x) {
////				std::cout << "asd" << std::endl;
////			}
////			else
////			if (wCenter.x + limitDimensionsX / 2 > worldBoundsHalfSize.x) {
////				std::cout << "bsd" << std::endl;
////			}
//
//			newZoom = std::abs(windowSize.x / limitDimensionsX);
//
//std::cout << newZoom << std::endl;
////			newZoom *= 1.1f;
//		}
//	}
//
//std::cout << std::endl;
//
//	return newZoom;
//}

sf::Vector2f WorldCamera2D::GetViewSize () {
	return sf::Vector2f(sfView.GetRect().GetWidth(), sfView.GetRect().GetHeight());
}

sf::FloatRect WorldCamera2D::GetViewRect () {
	return sfView.GetRect();
}
