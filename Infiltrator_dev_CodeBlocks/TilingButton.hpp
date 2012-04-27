#ifndef TILINGBUTTON_HPP
#define TILINGBUTTON_HPP

#include <boost/function.hpp>

#include "PFrameWork/GO/ScreenSpaceDrawable.hpp"
#include "PFrameWork/GO/Sprite.hpp"
#include "PFrameWork/GO/DrawableString.hpp"
#include "PFrameWork/SFMLEventHandler.hpp"
#include "PFrameWork/PFWConstants.hpp"
#include "PFrameWork/GO/ScreenSprite.hpp"

#include <SFML/Graphics/Color.hpp>

#include "PFrameWork/ResourceManager.hpp"

class TilingButton : public ScreenSpaceDrawable, public Sprite, public DrawableString, public SFMLEventHandler {
protected:
	float minWidth;

	DrawManager* const drawManager;

	float width;

	ScreenSprite* edgeLeft;
	ScreenSprite* edgeRight;

	bool active;
	bool hover;

	std::string txInactiveNormal[3];
	std::string txInactiveHover[3];
	std::string txActiveHover[3];

	boost::function<void (void)> onActivate;

	sf::Vector2f textOffset;

	virtual void SetFakeScale (sf::Vector2f fakeScale) {
		sfSprite->SetScale(fakeScale);
		sfString->SetScale(fakeScale);
	}
	virtual void SetFakePos (sf::Vector2f fakePos) {
		sfSprite->SetPosition(fakePos);
		sfString->SetPosition(fakePos
			+ sf::Vector2f(textOffset.x * baseScale.x,
							textOffset.y * baseScale.y) * windowFakeScale);
	}

public:
	TilingButton (DrawManager* const drawManager,
				std::string txInactiveNormal[3],
				std::string txInactiveHover[3],
				std::string txActiveHover[3],
				boost::function<void (void)> onActivate,
				std::string text = "",
				std::string fontFile = PFWConstants::defaultFontFile,
				float fontSize = PFWConstants::defaultFontSize,
				sf::Vector2f textOffset = sf::Vector2f(0, 0),
				float minWidth = -1,
				AlignH alignH = LEFT,
				AlignV alignV = TOP,
				int layerDepth = 0)
		: DrawableBase(layerDepth),
			ScreenSpaceDrawable(alignH, alignV, layerDepth),
			Sprite(txInactiveNormal[1], layerDepth),
			DrawableString(text, fontFile, fontSize, layerDepth),
			minWidth(minWidth),
			drawManager(drawManager),
			width(width),
			active(false),
			hover(false),
			onActivate(onActivate),
			textOffset(textOffset)
			{
		sfString->SetPosition(textOffset);
		sfString->SetColor(sf::Color(128, 128, 128));

		for (int i = 0; i < 3; i++) {
			this->txInactiveNormal[i] = txInactiveNormal[i];
			this->txInactiveHover[i] = txInactiveHover[i];
			this->txActiveHover[i] = txActiveHover[i];
		}

		width = sfString->GetRect().Right - sfString->GetRect().Left;
		width += Resources->GetImage(txInactiveNormal[0])->GetWidth() * 2;

		if (minWidth > 0) {
			if (width < minWidth) {
				width = minWidth;
			}
		}

		edgeLeft = new ScreenSprite(txInactiveNormal[0], alignH, alignV, layerDepth - 1);
		drawManager->AddScreenSpace(edgeLeft);
		edgeRight = new ScreenSprite(txInactiveNormal[2], alignH, alignV, layerDepth - 1);
		drawManager->AddScreenSpace(edgeRight);

		drawManager->AddScreenSpace(this);

		SetPosition(sf::Vector2f(0, 0));
	}

	~TilingButton () {
		drawManager->PopScreenSpace(edgeLeft);
		delete edgeLeft;
		drawManager->PopScreenSpace(edgeRight);
		delete edgeRight;

		drawManager->PopScreenSpace(this);
	}

	virtual void SetText (std::string text);

	virtual void ReleaseResources ();

	virtual void Draw (sf::RenderWindow& rwin);

	virtual void SetScale (float scale) {
		Sprite::SetScale(scale);
		DrawableString::SetScale(scale);

		edgeLeft->SetScale(scale);
		edgeRight->SetScale(scale);

		SetPosition(GetPosition());
	}
	virtual void SetScale (sf::Vector2f scale) {
		Sprite::SetScale(scale);
		DrawableString::SetScale(scale);

		edgeLeft->SetScale(scale);
		edgeRight->SetScale(scale);

		SetPosition(GetPosition());
	}

	virtual void SetPosition (sf::Vector2f position);
	virtual void Move (sf::Vector2f offset);

	virtual void SetColor (sf::Color color) {
		Sprite::SetColor(color);
		DrawableString::SetColor(color);

		edgeLeft->SetColor(color);
		edgeRight->SetColor(color);
	}
	virtual sf::Color GetColor () { return sfSprite->GetColor(); }

	virtual void SetRotation (float rotation) {
		Sprite::SetRotation(rotation);
		DrawableString::SetRotation(rotation);

		edgeLeft->SetRotation(rotation);
		edgeRight->SetRotation(rotation);
	}
	virtual float GetRotation () { return sfSprite->GetRotation(); }
	virtual void Rotate (float angle) {
		Sprite::Rotate(angle);
		DrawableString::Rotate(angle);

		edgeLeft->Rotate(angle);
		edgeRight->Rotate(angle);
	}

	virtual sf::Vector2f GetScreenSize () {
		return sf::Vector2f(width * sfSprite->GetScale().x,
							sfSprite->GetImage()->GetHeight() * sfSprite->GetScale().y);
	}

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual bool Contains (sf::Vector2f globalPosition) {
		return false;
	}
	virtual bool IsOpaque (sf::Vector2f globalPosition) {
		return false;
	}

	void SetActive (bool active);
};

#endif // TILINGBUTTON_HPP
