#ifndef GUISLIDER_HPP
#define GUISLIDER_HPP

#include "PFrameWork/SFMLEventHandler.hpp"
#include "PFrameWork/GO/ScreenSprite.hpp"
#include "PFrameWork/GO/ScreenSpaceDrawable.hpp"
#include "PFrameWork/GO/Sprite.hpp"

#include <boost/function.hpp>

class GUISlider : public ScreenSpaceDrawable, public Sprite, public SFMLEventHandler {
protected:
	DrawManager* const drawManager;

	float width;

	ScreenSprite* edgeLeft;
	ScreenSprite* edgeRight;

	ScreenSprite* sliderThingy;

	float value;

	bool hover;

	bool dragging;

	float lastMousePressedX;

	boost::function<void (float)> onChanged;

public:
	GUISlider (DrawManager* drawManager, float value,
				float width,
				std::string txEdgeLeft,
				std::string txMiddle,
				std::string txEdgeRight,
				std::string txSliderThingy,
				boost::function<void (float)> onChanged = 0,
				AlignH alignH = LEFT,
				AlignV alignV = TOP,
				int layerDepth = 0)
		: DrawableBase(layerDepth),
			ScreenSpaceDrawable(alignH, alignV, layerDepth),
			Sprite(txMiddle, layerDepth),
			drawManager(drawManager),
			width(width),
			value(value),
			hover(false),
			dragging(false),
			onChanged(onChanged)
			{
		edgeLeft = new ScreenSprite(txEdgeLeft, alignH, alignV, layerDepth);
		edgeRight = new ScreenSprite(txEdgeRight, alignH, alignV, layerDepth);

		sliderThingy = new ScreenSprite(txSliderThingy, alignH, alignV, layerDepth + 1);

		drawManager->AddScreenSpace(edgeLeft);
		drawManager->AddScreenSpace(edgeRight);
		drawManager->AddScreenSpace(sliderThingy);

		drawManager->AddScreenSpace(this);

		SetValue(value);
	}

	~GUISlider () {
		drawManager->PopScreenSpace(edgeLeft);
		drawManager->PopScreenSpace(edgeRight);
		drawManager->PopScreenSpace(sliderThingy);

		delete edgeLeft;
		delete edgeRight;

		delete sliderThingy;

		drawManager->PopScreenSpace(this);
	}

	void SetValue (float value);
	float GetValue () { return value; }

	virtual void Draw (sf::RenderWindow& rwin);

	virtual void SetScale (float scale) {
		Sprite::SetScale(scale);

		edgeLeft->SetScale(scale);
		edgeRight->SetScale(scale);

		sliderThingy->SetScale(scale);
	}
	virtual void SetScale (sf::Vector2f scale) {
		Sprite::SetScale(scale);

		edgeLeft->SetScale(scale);
		edgeRight->SetScale(scale);

		sliderThingy->SetScale(scale);
	}

	virtual void SetPosition (sf::Vector2f position);

	virtual void SetColor (sf::Color color) {
		Sprite::SetColor(color);

		edgeLeft->SetColor(color);
		edgeRight->SetColor(color);

		sliderThingy->SetColor(color);
	}
	virtual sf::Color GetColor () { return sfSprite->GetColor(); }

	virtual void SetRotation (float rotation) {
		Sprite::SetRotation(rotation);

		edgeLeft->SetRotation(rotation);
		edgeRight->SetRotation(rotation);

		sliderThingy->SetRotation(rotation);
	}
	virtual float GetRotation () { return sfSprite->GetRotation(); }
	virtual void Rotate (float angle) {
		Sprite::Rotate(angle);

		edgeLeft->Rotate(angle);
		edgeRight->Rotate(angle);

		sliderThingy->Rotate(angle);
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
};

#endif // GUISLIDER_HPP
