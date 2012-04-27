#ifndef DRAWABLESTRING_HPP
#define DRAWABLESTRING_HPP

#include "DrawableBase.hpp"

class DrawableString : virtual public DrawableBase {
protected:
	sf::String* sfString;

	float height;

	virtual void SetFakeScale (sf::Vector2f fakeScale) {
		sfString->SetScale(fakeScale);
		CalcHeight();
	}
	virtual void SetFakePos (sf::Vector2f fakePos) { sfString->SetPosition(fakePos); }

	void CalcHeight ();

public:
	DrawableString (int layerDepth) : DrawableBase(layerDepth) { sfString = new sf::String(); }
	DrawableString (std::string text, std::string fontFile, float size, int layerDepth);
	DrawableString (std::string text, float size, int layerDepth);
    DrawableString (DrawableString const& xDrawableString);

	virtual ~DrawableString ();

	virtual void ReleaseResources ();

	virtual void Draw (sf::RenderWindow& rwin) { rwin.Draw(*sfString); }

	virtual void SetColor (sf::Color color) { sfString->SetColor(color); }
	virtual sf::Color GetColor () { return sfString->GetColor(); }

	void SetStyle (unsigned long textStyle) { sfString->SetStyle(textStyle); }

	virtual void SetText (std::string text);
	std::string GetText () { return sfString->GetText(); }

	virtual void SetRotation (float rotation) { sfString->SetRotation(rotation); }
	virtual float GetRotation () { return sfString->GetRotation(); }
	virtual void Rotate (float angle) { sfString->Rotate(angle); }

	virtual sf::Vector2f GetScreenSize ();

	virtual bool Contains (sf::Vector2f globalPosition) {
		return false;
	}
	virtual bool IsOpaque (sf::Vector2f globalPosition) {
		return false;
	}
};

#endif // DRAWABLESTRING_HPP
