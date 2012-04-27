#include "EditorToolButton.hpp"

#include "PFrameWork/SFMLGameManager.hpp"
#include "PFrameWork/StateMachine.hpp"
#include "PFrameWork/State.hpp"

EditorToolButton::~EditorToolButton () {
	drawManager->PopScreenSpace(spriteAddActive);
	delete spriteAddActive;

	drawManager->PopScreenSpace(this);
}

void EditorToolButton::ReleaseResources () {
	Sprite::ReleaseResources();
	DrawableString::ReleaseResources();
//	Resources->ReleaseFont(&sfString->GetFont());
//	delete sfString;
//	sfString = 0;
//
//	Resources->ReleaseImage(sfSprite->GetImage());
//	delete sfSprite;
//	sfSprite = 0;
}

void EditorToolButton::HandleSFEvents (std::list<sf::Event>& sfEvents) {
	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::MouseMoved: {
				sf::Vector2f screenSize(GetScreenSize());
				sf::Rect<float> btnBounds(CalcFakePos().x - (screenSize.x / 2),
											CalcFakePos().y - (screenSize.y / 2 ),
											CalcFakePos().x + (screenSize.x / 2),
											CalcFakePos().y + (screenSize.y / 2 ) );

				if (btnBounds.Contains(itSfEvent->MouseMove.X, itSfEvent->MouseMove.Y)) {
					if (!hover) {
						sfString->SetColor(sf::Color::White);
						hover = true;
					}
				}
				else if (hover) {
					sfString->SetColor(sf::Color(128, 128, 128));
					hover = false;
				}
			} break;
			case sf::Event::MouseButtonPressed:
				if (hover) {
					SetActive(true);

					onActivate(ID);

					itSfEvent = --sfEvents.erase(itSfEvent);
				}
				else {
					SetActive(false);

					onActivate(-1);
				}
			break;
			default:
			break;
		}

		itSfEvent++;
	}
}

void EditorToolButton::SetActive (bool active) {
	if (active) {
		if (spriteAddActive == 0) {
			spriteAddActive = new ScreenSprite(textureAddActive,
				mAlignH, mAlignV,
				layerDepth - 1);
			drawManager->AddScreenSpace(spriteAddActive);
			spriteAddActive->SetPosition(GetPosition());
			spriteAddActive->SetRotation(GetRotation());
			spriteAddActive->SetScale(GetScale());
			spriteAddActive->SetColor(GetColor());

			drawManager->SortAscendingScreen();
		}
	}
	else if (spriteAddActive != 0) {
		drawManager->PopScreenSpace(spriteAddActive);
		delete spriteAddActive;
		spriteAddActive = 0;
	}
}

