#ifndef GUITEXTFIELD_HPP
#define GUITEXTFIELD_HPP

#include "ScreenSpaceDrawable.hpp"
#include "Sprite.hpp"
#include "DrawableString.hpp"
#include "SFMLEventHandler.hpp"
#include "MyConstants.hpp"
#include "ScreenSprite.hpp"
#include "DrawManager.hpp"

class GUITextfield : public ScreenSpaceDrawable, public Sprite, public DrawableString, public SFMLEventHandler {
protected:
	bool active;
	//bool hover;

	ScreenSprite* textCursor;
	unsigned int iTextCursor;

	std::string txInactiveNormal;
	std::string txActiveNormal;

	virtual void SetFakeScale (sf::Vector2f fakeScale) {
		sfSprite->SetScale(fakeScale);
		sfString->SetScale(fakeScale);
	}
	virtual void SetFakePos (sf::Vector2f fakePos) {
		sfSprite->SetPosition(fakePos);
		sfString->SetPosition(fakePos);
	}

public:
	GUITextfield (std::string txInactiveNormal,
				std::string txActiveNormal,
				std::string text = "",
				std::string fontFile = MConst::defaultFontFile,
				float fontSize = MConst::fontSize,
				AlignH alignH = LEFT,
				AlignV alignV = TOP,
				float layerDepth = 0)
		: DrawableBase(layerDepth),
			ScreenSpaceDrawable(alignH, alignV, layerDepth),
			Sprite(txInactiveNormal, layerDepth),
			DrawableString(text, fontFile, fontSize, layerDepth),
			active(false),
			//hover(false),
			textCursor(0),
			iTextCursor(0),
			txInactiveNormal(txInactiveNormal),
			txActiveNormal(txActiveNormal)
		{}

	~GUITextfield () {
		if (active) {
			DrawManager::Instance()->PopScreenSpace(textCursor);
		}

		delete textCursor;
	}

	virtual void Draw (sf::RenderWindow& rwin) {
		rwin.Draw(*sfSprite);
		rwin.Draw(*sfString);
	}

	virtual void SetColor (sf::Color color) {
		sfSprite->SetColor(color);
		sfString->SetColor(color);
	}

	virtual void SetRotation (float rotation) {
		sfSprite->SetRotation(rotation);
		sfString->SetRotation(rotation);
	}
	virtual float GetRotation () { return sfSprite->GetRotation(); }
	virtual void Rotate (float angle) {
		sfSprite->Rotate(angle);
		sfString->Rotate(angle);
	}

	virtual sf::Vector2f GetScreenSize () {
		//sf::Vector2f imgSize(sfSprite->GetImage()->GetWidth(), sfSprite->GetImage()->GetHeight());
		//return sf::Vector2f(imgSize.x * sfSprite->GetScale().x, imgSize.y * sfSprite->GetScale().y);

		//return sf::Vector2f( (sfString->GetRect().Right - sfString->GetRect().Left),
							//(sfString->GetRect().Bottom - sfString->GetRect().Top) );

		//*****************************************************************************************************************************************
		return sf::Vector2f( (sfString->GetRect().Right - sfString->GetRect().Left) + 50,
							(sfString->GetRect().Bottom - sfString->GetRect().Top) );
	}

	virtual void HandleEvents(std::list<sf::Event>& sfEvents);
};

#endif // GUITEXTFIELD_HPP
