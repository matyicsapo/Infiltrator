#ifndef DRAWABLEBASE_HPP
#define DRAWABLEBASE_HPP

#include <SFML/Graphics.hpp>

class DrawableBase {
private:
	void Reset (float windowFakeScale) {
		this->windowFakeScale = windowFakeScale;
		SetFakeScale(CalcFakeScale());
		SetFakePos(CalcFakePos());
	}

protected:
	float layerDepth;

	float windowFakeScale;

	sf::Vector2f baseScale;
	sf::Vector2f basePos;

	sf::Vector2f CalcFakeScale () { return baseScale * windowFakeScale; }

	virtual sf::Vector2f CalcFakePos () = 0;

	virtual void SetFakeScale (sf::Vector2f fakeScale) = 0;
	virtual void SetFakePos (sf::Vector2f fakePos) = 0;

public:
	DrawableBase (float layerDepth) : layerDepth(layerDepth), windowFakeScale(1), baseScale(1, 1) {}

	void SetLayerDepth (float layerDepth) { this->layerDepth = layerDepth; }

	virtual void Draw (sf::RenderWindow& rwin) = 0;

	void SetPosition (sf::Vector2f pos) {
		basePos = pos;
		SetFakePos(CalcFakePos());
	}
	sf::Vector2f GetPosition () { return basePos; }
	void Move (sf::Vector2f offset) {
		basePos += offset;
		SetFakePos(CalcFakePos());
	}

	void SetScale (sf::Vector2f scale) {
		baseScale = scale;
		SetFakeScale(CalcFakeScale());
	}
	sf::Vector2f GetScale () { return baseScale; }

	virtual void SetRotation (float rotation) = 0;
	virtual float GetRotation () = 0;
	virtual void Rotate (float angle) = 0;

	virtual void SetColor (sf::Color color) = 0;

	virtual sf::Vector2f GetScreenSize () = 0;

	friend class DrawManager;
};

#endif // DRAWABLEBASE_HPP
