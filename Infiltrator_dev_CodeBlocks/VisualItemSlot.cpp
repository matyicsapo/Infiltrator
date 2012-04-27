#include "VisualItemSlot.hpp"

#include "PFrameWork/GO/ScreenSprite.hpp"
#include "PFrameWork/DrawManager.hpp"
//#include <iostream>

VisualItemSlot::VisualItemSlot (DrawManager* drawManager, std::string txNormal,
								std::string txHover,
								AlignH alignH,
								AlignV alignV,
								int layerDepth)
		: DrawableBase(layerDepth),
		ScreenSprite(txNormal, alignH, alignV, layerDepth),
		drawManager(drawManager),
		txNormal(txNormal),
		txHover(txHover),
		containedSprite(0) {
	drawManager->AddScreenSpace(this);
}

VisualItemSlot::~VisualItemSlot () {
	drawManager->PopScreenSpace(containedSprite);
	delete containedSprite;

	drawManager->PopScreenSpace(this);
}

void VisualItemSlot::SetContained(ScreenSprite* sprite, unsigned int typeID, unsigned int ID) {
	drawManager->PopScreenSpace(containedSprite);
	delete containedSprite;

	containedSprite = sprite;

	containedTypeID = typeID;
	containedID = ID;

	if (containedSprite) {
		drawManager->AddScreenSpace(containedSprite);

		containedSprite->SetAlign(mAlignH, mAlignV);

		float x = 0;
		switch (mAlignH) {
			case ScreenSpaceDrawable::LEFT:
				x = GetPosition().x - containedSprite->GetScreenSize().x / 2.0f / windowFakeScale + GetScreenSize().x / 2.0f / windowFakeScale;
			break;
			case ScreenSpaceDrawable::CENTER:
				x = GetPosition().x;
			break;
			case ScreenSpaceDrawable::RIGHT:
				x = GetPosition().x + containedSprite->GetScreenSize().x / 2.0f / windowFakeScale - GetScreenSize().x / 2.0f / windowFakeScale;
			break;
		}

		float y = 0;
		switch (mAlignV) {
			case ScreenSpaceDrawable::TOP:
				y = GetPosition().y - containedSprite->GetScreenSize().y / 2.0f / windowFakeScale + GetScreenSize().y / 2.0f / windowFakeScale;
			break;
			case ScreenSpaceDrawable::MIDDLE:
				y = GetPosition().y;
			break;
			case ScreenSpaceDrawable::BOTTOM:
				y = GetPosition().y + containedSprite->GetScreenSize().y / 2.0f / windowFakeScale - GetScreenSize().y / 2.0f / windowFakeScale;
			break;
		}

		containedSprite->SetPosition(sf::Vector2f(x, y));

		drawManager->SortAscendingScreen();
	}
}

bool VisualItemSlot::GetContained (unsigned int& typeID, unsigned int& ID) const {
	if (!containedSprite)
		return false;

	typeID = containedTypeID;
	ID = containedID;
	return true;
}

void VisualItemSlot::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::MouseMoved: {
//				sf::Vector2f screenSize(GetScreenSize());
//				sf::Rect<float> btnBounds(CalcFakePos().x - (screenSize.x / 2),
//											CalcFakePos().y - (screenSize.y / 2 ),
//											CalcFakePos().x + (screenSize.x / 2),
//											CalcFakePos().y + (screenSize.y / 2 ) );
//
//				if (btnBounds.Contains(itSfEvent->MouseMove.X, itSfEvent->MouseMove.Y)) {
				if (Contains(sf::Vector2f())) {
					if (!hover) {
						// if got in/on
						hover = true;
						SetImage(txHover);
					}
				}
				else if (hover) {
					// if got out/off
					hover = false;
					SetImage(txNormal);
				}
			} break;
			default:
			break;
		}

		itSfEvent++;
	}
}

void VisualItemSlot::ReleaseResources () {
	ScreenSprite::ReleaseResources();
	drawManager->PopScreenSpace(containedSprite);
	delete containedSprite;
	containedSprite = 0;
}
