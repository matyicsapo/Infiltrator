#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "DrawableBase.hpp"

class Shape : virtual public DrawableBase {
protected:
	sf::Shape* sfShape;

	virtual void SetFakeScale (sf::Vector2f fakeScale);
	virtual void SetFakePos (sf::Vector2f fakePos);

public:
	Shape (sf::Shape* s, int layerDepth = 0);
    Shape (Shape const& shape);

    virtual ~Shape ();

    virtual void ReleaseResources ();

    virtual void Draw (sf::RenderWindow& rwin);

    virtual void SetColor (sf::Color color);
    virtual sf::Color GetColor ();

	virtual void SetRotation (float rotation);
	virtual float GetRotation ();
	virtual void Rotate (float angle);

	void AddPoint (const sf::Vector2f& Position,
					const sf::Color& Col = sf::Color(255, 255, 255),
					const sf::Color& OutlineCol = sf::Color(0, 0, 0));
	unsigned int GetNbPoints () const;
	void EnableFill (bool enable);
	void EnableOutline (bool enable);
	void SetPointPosition (unsigned int Index, const sf::Vector2f& Position);
	void SetOutlineWidth (float Width);
	sf::Vector2f GetPointPosition (unsigned int Index) const;
	sf::Vector2f GetPointPositionLocal (unsigned int Index) const;
	sf::Vector2f GetPointPositionTransformed (unsigned int Index) const;
	void SetCenter (const sf::Vector2f& Center);

	virtual sf::Vector2f TransformToLocal (const sf::Vector2f& Point) const;
	virtual sf::Vector2f TransformToGlobal (const sf::Vector2f& Point) const;

	virtual sf::Vector2f GetScreenSize ();
	virtual bool Contains (sf::Vector2f worldPosition);
	virtual bool IsOpaque (sf::Vector2f worldPosition);
};

#endif // SHAPE_HPP
