#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "DrawableBase.hpp"

class Sprite : virtual public DrawableBase {
protected:
	sf::Sprite* sfSprite;

	virtual void SetFakeScale (sf::Vector2f fakeScale) { sfSprite->SetScale(fakeScale); }
	virtual void SetFakePos (sf::Vector2f fakePos) { sfSprite->SetPosition(fakePos); }

public:
	Sprite (int layerDepth, unsigned int entityType = 0) : DrawableBase(layerDepth), sfSprite(0) {}
	Sprite (std::string textureFile, int layerDepth, unsigned int entityType = 0);
    Sprite (Sprite const& xSprite);

    virtual ~Sprite ();

    virtual void ReleaseResources ();

    virtual void Draw (sf::RenderWindow& rwin) { rwin.Draw(*sfSprite); }

    virtual void SetColor (sf::Color color) { sfSprite->SetColor(color); }
    virtual sf::Color GetColor () { return sfSprite->GetColor(); }

	virtual void SetImage (std::string textureFile);

	virtual void SetRotation (float rotation) { sfSprite->SetRotation(rotation); }
	virtual float GetRotation () { return sfSprite->GetRotation(); }
	virtual void Rotate (float angle) { sfSprite->Rotate(angle); }

	virtual sf::Vector2f GetScreenSize ();

	virtual bool Contains (sf::Vector2f globalPosition);
	virtual bool IsOpaque (sf::Vector2f globalPosition);
};

#endif // SPRITE_HPP
