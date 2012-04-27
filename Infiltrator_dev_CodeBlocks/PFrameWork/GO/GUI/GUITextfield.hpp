#ifndef GUITEXTFIELD_HPP
#define GUITEXTFIELD_HPP

#include "../Sprite.hpp"
#include "../DrawableString.hpp"
#include "../ScreenSpaceDrawable.hpp"
#include "../../SFMLEventHandler.hpp"
#include "../../PFWConstants.hpp"
#include "../ScreenSprite.hpp"
#include "../../DrawManager.hpp"
#include <boost/regex.hpp>
#include <boost/function.hpp>

#include "../../SFMLGameManager.hpp"
#include "../../State.hpp"
#include "../../StateMachine.hpp"

class GUITextfield : public ScreenSpaceDrawable, public Sprite, public DrawableString, public SFMLEventHandler {
protected:
	DrawManager* const drawManager;

	boost::regex regexPattern;

	boost::function<void (void)> onApply;

	bool active;
	bool hover;

	std::string txNormal;
	std::string txHover;

	sf::Color colTextInactive;
	sf::Color colTextActive;

	ScreenSprite* textCursor;
	int iTextCursor;
	float flashTime;
	float timeSinceLastFlash;

	float widthWoBg;
	float height;
	float textCursorY;

	sf::Rect<float> GetScreenBounds ();

	virtual void SetFakeScale (sf::Vector2f fakeScale);
	virtual void SetFakePos (sf::Vector2f fakePos);

	virtual sf::Vector2f CalcFakePos ();

	void CalcHeight ();
	void CalcTextCursorPosY ();
	float GetTextCursorX ();
	float x_GetTextCursorX (); // used when there's no text

public:
	GUITextfield (DrawManager* drawManager,
				std::string txNormal = "",
				std::string txHover = "",
				sf::Color colTextInactive = sf::Color(128, 128, 128),
				sf::Color colTextActive = sf::Color::White,
				std::string text = "",
				std::string regexPattern = ".",
				boost::function<void (void)> onApply = 0,
				std::string fontFile = PFWConstants::defaultFontFile,
				float fontSize = PFWConstants::defaultFontSize,
				float widthWoBg = 100,
				AlignH alignH = LEFT,
				AlignV alignV = TOP,
				int layerDepth = 0)
		: DrawableBase(layerDepth),
			ScreenSpaceDrawable(alignH, alignV, layerDepth),
			Sprite(txNormal, layerDepth),
			DrawableString(text, fontFile, fontSize, layerDepth),
			drawManager(drawManager),
			regexPattern(regexPattern),
			onApply(onApply),
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

		drawManager->AddScreenSpace(this);
	}

	~GUITextfield () {
		drawManager->PopScreenSpace(textCursor);
		delete textCursor;

		drawManager->PopScreenSpace(this);
	}

	virtual void ReleaseResources ();

	virtual void SetText (std::string text, bool resetCursorToEnd = false);

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

	virtual bool Contains (sf::Vector2f globalPosition) {
		return false;
	}
	virtual bool IsOpaque (sf::Vector2f globalPosition) {
		return false;
	}

	void SetRegexPattern (std::string regexPattern) {
		this->regexPattern = boost::regex(regexPattern);
	}

	void SetActive (bool active);
	bool GetActive () { return active; }
};

#endif // GUITEXTFIELD_HPP
