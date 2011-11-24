#ifndef DRAWABLEBASE_HPP
#define DRAWABLEBASE_HPP

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"

class DrawableBase : virtual public GameObject {
public:
	int layerDepth;

	DrawableBase (int layerDepth) : GameObject(), layerDepth(layerDepth) {}

	virtual void Draw (sf::RenderWindow& rwin) = 0;

	virtual void SetColor (sf::Color color) = 0;
	virtual sf::Color GetColor () = 0;
};

#endif // DRAWABLEBASE_HPP
