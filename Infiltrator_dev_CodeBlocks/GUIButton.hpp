#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

#include <boost/function.hpp>

#include "ScreenSpaceDrawable.hpp"
#include "Sprite.hpp"
#include "DrawableString.hpp"
#include "SFMLEventHandler.hpp"
#include "MyConstants.hpp"

class GUIButton : public ScreenSpaceDrawable, public Sprite, public DrawableString, public SFMLEventHandler {
protected:
	bool active;
	bool hover;

	std::string txInactiveNormal;
	std::string txInactiveHover;
	std::string txActiveHover;

	boost::function<void (void)> onActivate;

	virtual void SetFakeScale (sf::Vector2f fakeScale) {
		sfSprite->SetScale(fakeScale);
		sfString->SetScale(fakeScale);
	}
	virtual void SetFakePos (sf::Vector2f fakePos) {
		sfSprite->SetPosition(fakePos);
		sfString->SetPosition(fakePos);
	}

public:
	GUIButton (std::string txInactiveNormal,
				std::string txInactiveHover,
				std::string txActiveHover,
				boost::function<void (void)> onActivate,
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
			hover(false),
			txInactiveNormal(txInactiveNormal),
			txInactiveHover(txInactiveHover),
			txActiveHover(txActiveHover),
			onActivate(onActivate)
		{}

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

	//**********************************************************************************************************************************************************************************ó
	virtual sf::Vector2f GetScreenSize () {
		sf::Vector2f imgSize(sfSprite->GetImage()->GetWidth(), sfSprite->GetImage()->GetHeight());
		return sf::Vector2f(imgSize.x * sfSprite->GetScale().x, imgSize.y * sfSprite->GetScale().y);
	}

	virtual void HandleEvents(std::list<sf::Event>& sfEvents);
};

#endif // GUIBUTTON_HPP
