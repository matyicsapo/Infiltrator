#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "DrawableBase.hpp"

class Shape : virtual public DrawableBase {
protected:
	sf::Shape* sfShape;

	virtual void SetFakeScale (sf::Vector2f fakeScale) { sfShape->SetScale(fakeScale); }
	virtual void SetFakePos (sf::Vector2f fakePos) { sfShape->SetPosition(fakePos); }

public:
	Shape (sf::Shape* s, int layerDepth) : DrawableBase(layerDepth), sfShape(s) {}
    Shape (Shape const& shape) : DrawableBase(shape)
		{ sfShape = new sf::Shape( *(shape.sfShape) ); }

    virtual ~Shape () { delete sfShape; }

    virtual void Draw (sf::RenderWindow& rwin) { rwin.Draw(*sfShape); }

    virtual void SetColor (sf::Color color) { sfShape->SetColor(color); }

	virtual void SetRotation (float rotation) { sfShape->SetRotation(rotation); }
	virtual float GetRotation () { return sfShape->GetRotation(); }
	virtual void Rotate (float angle) { sfShape->Rotate(angle); }

	void EnableFill (bool enable) { sfShape->EnableFill(enable); }
	void EnableOutline (bool enable) { sfShape->EnableOutline(enable); }

// not implemented *********************************************************************
	virtual sf::Vector2f GetScreenSize () {
		return sf::Vector2f(0, 0);
	}
};

#endif // SHAPE_HPP
