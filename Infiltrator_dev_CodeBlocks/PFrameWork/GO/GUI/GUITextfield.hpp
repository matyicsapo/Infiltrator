#ifndef GUITEXTFIELD_HPP
#define GUITEXTFIELD_HPP

#include "../Sprite.hpp"
#include "../DrawableString.hpp"
#include "../ScreenSpaceDrawable.hpp"
#include "../../SFMLEventHandler.hpp"
#include "../../PFWConstants.hpp"
#include "../ScreenSprite.hpp"
#include "../../DrawManager.hpp"

class GUITextfield : public ScreenSpaceDrawable, public Sprite, public DrawableString, public SFMLEventHandler {
protected:
	bool active;
	bool hover;

	std::string txNormal;
	std::string txHover;

	sf::Color colTextInactive;
	sf::Color colTextActive;

	ScreenSprite* textCursor;
	unsigned int iTextCursor;
	float flashTime;
	float timeSinceLastFlash;

	float widthWoBg;
	float height;
	float textCursorY;

	sf::Rect<float> GetScreenBounds ();

	virtual void SetFakeScale (sf::Vector2f fakeScale) {
		if (sfSprite) sfSprite->SetScale(fakeScale);
		sfString->SetScale(fakeScale);

		CalcHeight();
		if (textCursor != 0) CalcTextCursorPosY();
	}
	virtual void SetFakePos (sf::Vector2f fakePos) {
		if (sfSprite) sfSprite->SetPosition(fakePos);
		sfString->SetPosition(fakePos);

		if (textCursor != 0) CalcTextCursorPosY();
	}

	virtual sf::Vector2f CalcFakePos ();

	void CalcHeight ();
	void CalcTextCursorPosY ();
	float GetTextCursorX ();
	float x_GetTextCursorX ();

public:
	GUITextfield (std::string txNormal = "",
				std::string txHover = "",
				sf::Color colTextInactive = sf::Color(128, 128, 128),
				sf::Color colTextActive = sf::Color::White,
				std::string text = "",
				std::string fontFile = PFWConstants::defaultFontFile,
				float fontSize = PFWConstants::fontSize,
				float widthWoBg = 100,
				AlignH alignH = LEFT,
				AlignV alignV = TOP,
				int layerDepth = 0)
		: DrawableBase(layerDepth),
			ScreenSpaceDrawable(alignH, alignV, layerDepth),
			Sprite(txNormal, layerDepth),
			DrawableString(text, fontFile, fontSize, layerDepth),
			active(false),
			hover(false),
			txNormal(txNormal),
			txHover(txHover),
			colTextInactive(colTextInactive),
			colTextActive(colTextActive),
			textCursor(0),
			iTextCursor(0),
			flashTime(.5f),
			timeSinceLastFlash(0),
			widthWoBg(widthWoBg) {
		sfString->SetColor(colTextInactive);

		CalcHeight();

		SetText(text); // lazy
	}

	~GUITextfield () {
		if (active) {
			Drawables->PopScreenSpace(textCursor);
		}

		delete textCursor;
	}

	virtual void SetText (std::string text);

	virtual void Draw (sf::RenderWindow& rwin) {
		if (sfSprite) rwin.Draw(*sfSprite);
		rwin.Draw(*sfString);
	}

	virtual void SetColor (sf::Color color) {
		if (sfSprite) sfSprite->SetColor(color);
		sfString->SetColor(color);
	}
	virtual sf::Color GetColor () { return sfString->GetColor(); }

	virtual void SetRotation (float rotation) {
		if (sfSprite) sfSprite->SetRotation(rotation);
		sfString->SetRotation(rotation);
	}
	virtual float GetRotation () { return sfString->GetRotation(); }
	virtual void Rotate (float angle) {
		if (sfSprite) sfSprite->Rotate(angle);
		sfString->Rotate(angle);
	}

	virtual sf::Vector2f GetScreenSize ();

	void Update (float dT); // updates flashing cursor

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);
};

#endif // GUITEXTFIELD_HPP
