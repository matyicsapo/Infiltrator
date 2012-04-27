#ifndef DRAWABLEBASE_HPP
#define DRAWABLEBASE_HPP

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"

class DrawableBase : virtual public GameObject {
	friend class DrawManager;

protected:
	int layerDepth;

public:
	DrawableBase (int layerDepth, unsigned int entityType = 0) : GameObject(entityType), layerDepth(layerDepth) {}
	DrawableBase (DrawableBase const& xDrawableBase) : GameObject(xDrawableBase) {
		layerDepth = xDrawableBase.layerDepth;
	}

	virtual void Draw (sf::RenderWindow& rwin) = 0;

	void SetLayerDepth (int layerDepth);
	int GetLayerDepth () { return layerDepth; }

	virtual void SetColor (sf::Color color) = 0;
	virtual sf::Color GetColor () = 0;

	virtual sf::Vector2f GetScreenSize () = 0;

	virtual bool Contains (sf::Vector2f globalPosition) = 0;
	virtual bool IsOpaque (sf::Vector2f globalPosition) = 0;
};

#endif // DRAWABLEBASE_HPP
