#include "GroundSprite.hpp"

#include "PFrameWork/SFMLGameManager.hpp"

void GroundSprite::Draw (sf::RenderWindow& rwin) {
	sf::FloatRect viewRect(Game->GetWorldCamera2D()->GetViewRect());

	int w = sfSprite->GetImage()->GetWidth() * sfSprite->GetScale().x;
	int h = sfSprite->GetImage()->GetHeight() * sfSprite->GetScale().y;

	float lowTop = viewRect.Top < -.5f ? viewRect.Top : -.5f;

	int itX = (viewRect.Left < -.5f ? viewRect.Left : -.5f) / w - 1;
	int limitX = (viewRect.Right > .5f ? viewRect.Right : .5f) / w + 1;
	int itY = lowTop / h - 1;
	int limitY = (viewRect.Bottom > .5f ? viewRect.Bottom : .5f) / h + 1;

	sf::Vector2f offset(itX * w, itY * h);

	for ( ; itX <= limitX; itX++) {
		for ( ; itY <= limitY; itY++) {
			sfSprite->SetPosition(offset);
			rwin.Draw(*sfSprite);

			offset.y += h;
		}

		itY = lowTop / h - 1;

		offset.x += w;
		offset.y = itY * h;
	}
}
