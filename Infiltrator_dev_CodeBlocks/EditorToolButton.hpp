#ifndef EDITORTOOLBUTTON_HPP
#define EDITORTOOLBUTTON_HPP

#include <boost/function.hpp>

#include "PFrameWork/GO/ScreenSprite.hpp"
#include "PFrameWork/GO/DrawableString.hpp"
#include "PFrameWork/SFMLEventHandler.hpp"
#include "PFrameWork/PFWConstants.hpp"
#include "PFrameWork/DrawManager.hpp"

class EditorToolButton : public ScreenSpaceDrawable, public Sprite, public DrawableString, public SFMLEventHandler {
protected:
	DrawManager* const drawManager;

	int ID;

	ScreenSprite* spriteAddActive;

	bool hover;

	std::string texture;
	std::string textureAddActive;

	boost::function<void (int)> onActivate;

	sf::Vector2f textOffset;

	sf::Vector2i fixedSize;

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
	EditorToolButton (DrawManager* drawManager, int ID,
				std::string texture,
				std::string textureAddActive,
				boost::function<void (int)> onActivate,
				std::string text = "",
				std::string fontFile = PFWConstants::defaultFontFile,
				float fontSize = PFWConstants::defaultFontSize,
				sf::Vector2f textOffset = sf::Vector2f(0, 0),
				AlignH alignH = LEFT,
				AlignV alignV = TOP,
				int layerDepth = 0,
				sf::Vector2i fixedSize = sf::Vector2i(-1, -1))
		: DrawableBase(layerDepth),
			ScreenSpaceDrawable(alignH, alignV, layerDepth),
			Sprite(texture, layerDepth),
			DrawableString(text, fontFile, fontSize, layerDepth),
			drawManager(drawManager),
			ID(ID),
			hover(false),
			texture(texture),
			textureAddActive(textureAddActive),
			onActivate(onActivate),
			textOffset(textOffset),
			fixedSize(fixedSize) {
		sfString->SetPosition(textOffset);
		sfString->SetColor(sf::Color(128, 128, 128));

		spriteAddActive = 0;

		drawManager->AddScreenSpace(this);
	}

	~EditorToolButton ();

	virtual void ReleaseResources ();

	void SetActive (bool active);
	int GetID () { return ID; }

	virtual void Draw (sf::RenderWindow& rwin) {
		rwin.Draw(*sfSprite);
		rwin.Draw(*sfString);
	}

	virtual void SetColor (sf::Color color) {
		sfSprite->SetColor(color);
		sfString->SetColor(color);

		if (spriteAddActive != 0) spriteAddActive->SetColor(color);
	}
	virtual sf::Color GetColor () { return sfSprite->GetColor(); }

	virtual void SetRotation (float rotation) {
		sfSprite->SetRotation(rotation);
		sfString->SetRotation(rotation);

		if (spriteAddActive != 0) spriteAddActive->SetRotation(rotation);
	}
	virtual float GetRotation () { return sfSprite->GetRotation(); }
	virtual void Rotate (float angle) {
		sfSprite->Rotate(angle);
		sfString->Rotate(angle);

		if (spriteAddActive != 0) spriteAddActive->Rotate(angle);
	}

	virtual void SetPosition (sf::Vector2f pos) {
		basePos = pos;
		SetFakePos(CalcFakePos());

		if (spriteAddActive != 0) spriteAddActive->SetPosition(pos);
	}
	virtual void Move (sf::Vector2f offset) {
		basePos += offset;
		SetFakePos(CalcFakePos());

		if (spriteAddActive != 0) spriteAddActive->Move(offset);
	}

	virtual sf::Vector2f GetScreenSize () {
		return sf::Vector2f(fixedSize.x * sfSprite->GetScale().x,
							fixedSize.y * sfSprite->GetScale().y);
	}

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual bool Contains (sf::Vector2f globalPosition) {
		return false;
	}
	virtual bool IsOpaque (sf::Vector2f globalPosition) {
		return false;
	}
};

#endif // EDITORTOOLBUTTON_HPP
