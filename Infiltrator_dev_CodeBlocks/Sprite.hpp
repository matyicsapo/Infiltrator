#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Drawable.hpp"

class Sprite : virtual public DrawableBase {
protected:
	sf::Sprite* sfSprite;

	virtual void SetFakeScale (sf::Vector2f fakeScale) { sfSprite->SetScale(fakeScale); }
	virtual void SetFakePos (sf::Vector2f fakePos) { sfSprite->SetPosition(fakePos); }

public:
	Sprite (float layerDepth) : DrawableBase(layerDepth), sfSprite(0) {}
	Sprite (std::string textureFile, float layerDepth);
    Sprite (Sprite const& sprite) : DrawableBase(sprite)
		{ sfSprite = new sf::Sprite( *(sprite.sfSprite) ); }

    virtual ~Sprite () { delete sfSprite; }

    virtual void Draw (sf::RenderWindow& rwin) { rwin.Draw(*sfSprite); }

    virtual void SetColor (sf::Color color) { sfSprite->SetColor(color); }

	virtual void SetImage (std::string textureFile);

	virtual void SetRotation (float rotation) { sfSprite->SetRotation(rotation); }
	virtual float GetRotation () { return sfSprite->GetRotation(); }
	virtual void Rotate (float angle) { sfSprite->Rotate(angle); }

	virtual sf::Vector2f GetScreenSize () {
		sf::Vector2f imgSize(sfSprite->GetImage()->GetWidth(), sfSprite->GetImage()->GetHeight());
		return sf::Vector2f(imgSize.x * sfSprite->GetScale().x, imgSize.y * sfSprite->GetScale().y);
	}
};

#endif // SPRITE_HPP
