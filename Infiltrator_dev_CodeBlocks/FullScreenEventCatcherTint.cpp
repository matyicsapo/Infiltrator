#include "FullScreenEventCatcherTint.hpp"

#include "PFrameWork/SFMLGameManager.hpp"
//		#include "PFrameWork/StateMachine.hpp"

#include "PFrameWork/PFWConstants.hpp"

// sizes matching exactly can leave artifacts at the edges so we multiply it by a small amount
#define MUL_OVERSIZE 1.1f

FullScreenEventCatcherTint::FullScreenEventCatcherTint (int layerDepth,  sf::Color color)
		: DrawableBase(layerDepth),
		ScreenSpaceDrawable(ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE, layerDepth),
		Sprite("Content/Textures/one.png", layerDepth) {
	sfSprite->SetColor(color);

	OnResize(Game->GetRenderWindow()->GetWidth(), Game->GetRenderWindow()->GetHeight());
}

void FullScreenEventCatcherTint::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::Resized:
				OnResize(itSfEvent->Size.Width, itSfEvent->Size.Height);
				sfEvents.clear();
				return;
			break;
			default:
			break;
		}

		itSfEvent++;
	}

	sfEvents.clear();
}

void FullScreenEventCatcherTint::OnResize (int width, int height) {
	// / 100 <= "Content/Textures/one.png" is 100 pixels wide AND tall
	sf::Vector2f b(PFWConstants::baseResolution.x / 2, PFWConstants::baseResolution.y / 2);

	sf::Vector2f d((width / PFWConstants::baseResolution.x),
					(height / PFWConstants::baseResolution.y));

	SetScale(sf::Vector2f(
						b.x / (d.y < 1 ? d.y : 1),
						b.y / (d.x < 1 ? d.x : 1)
						) * MUL_OVERSIZE
			);
}
