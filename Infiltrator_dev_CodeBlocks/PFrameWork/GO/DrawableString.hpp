#ifndef DRAWABLESTRING_HPP
#define DRAWABLESTRING_HPP

#include "DrawableBase.hpp"

class DrawableString : virtual public DrawableBase {
protected:
	sf::String* sfString;

	virtual void SetFakeScale (sf::Vector2f fakeScale) { sfString->SetScale(fakeScale); }
	virtual void SetFakePos (sf::Vector2f fakePos) { sfString->SetPosition(fakePos); }

public:
	DrawableString (int layerDepth) : DrawableBase(layerDepth) { sfString = new sf::String(); }
	DrawableString (std::string text, std::string fontFile, float size, int layerDepth);
	DrawableString (std::string text, float size, int layerDepth);
    DrawableString (DrawableString const& xDrawableString) : DrawableBase(xDrawableString)
		{ sfString = new sf::String( *(xDrawableString.sfString) ); }

	virtual ~DrawableString () { delete sfString; }

	virtual void Draw (sf::RenderWindow& rwin) { rwin.Draw(*sfString); }

	virtual void SetColor (sf::Color color) { sfString->SetColor(color); }

	void SetStyle (unsigned long textStyle) { sfString->SetStyle(textStyle); }

	void SetText (std::string text);
	std::string GetText () { return sfString->GetText(); }

	virtual void SetRotation (float rotation) { sfString->SetRotation(rotation); }
	virtual float GetRotation () { return sfString->GetRotation(); }
	virtual void Rotate (float angle) { sfString->Rotate(angle); }

	virtual sf::Vector2f GetScreenSize () {
		return sf::Vector2f( (sfString->GetRect().Right - sfString->GetRect().Left),
							(sfString->GetRect().Bottom - sfString->GetRect().Top) );
	}
};

#endif // DRAWABLESTRING_HPP
