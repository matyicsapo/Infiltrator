#include "GameState_Logos.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "PFrameWork/DrawManager.hpp"

#include "PFrameWork/SFMLGameManager.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>

#include "PFrameWork/PFWConstants.hpp"

#include "Player.hpp"
#include "Wall.hpp"

#include "PFrameWork/GO/WorldSprite.hpp"
#include "PFrameWork/GO/WorldString.hpp"
#include "PFrameWork/GO/ScreenString.hpp"
#include "PFrameWork/GO/ScreenSprite.hpp"

#include "PFrameWork/GO/GUI/GUIButton.hpp"
#include "PFrameWork/GO/GUI/GUIToggleButton.hpp"
#include "PFrameWork/GO/GUI/GUITextfield.hpp"

#include "PFrameWork/GO/TimeAnimatedSprite.hpp"

#include "PFrameWork/GO/WorldAnimatedSprite.hpp"
#include "PFrameWork/SpriteAnimationManager.hpp"

GameState_Logos::GameState_Logos () {
	wall1 = new Wall("Content/Textures/wall1.png", 10);
	wall1->SetScale(sf::Vector2f(3, 2));
	wall1->SetPosition(sf::Vector2f(-300, 0));

	SpriteAnimationManager::Instance()->LoadAnimations("Content/Textures/Animated/fire.anim");
	fireClip = new TimeAnimatedSprite(
		new WorldAnimatedSprite((*SpriteAnimationManager::Instance())["burn"]));

	mCrosshair = new ScreenSprite("Content/Textures/crosshair.png",
		ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE, 100);

	player = new Player(21);
	player->SetPosition(sf::Vector2f(100, 100));
player_collisiontest = new Player();
player_collisiontest->SetPosition(sf::Vector2f(300, -100));

	mWorldSprite1 = new WorldSprite("Content/Textures/sprite.png");
	mWorldSprite1->SetPosition(sf::Vector2f(600, 100));
	mWorldSprite1->SetRotation(37);

	mWorldSprite2 = new WorldSprite("Content/Textures/background_waterfall.jpg", -1);

	mWorldString1 = new WorldString("Hello World", 36);
	mWorldString1->SetPosition(sf::Vector2f(300, 200));
	mWorldString1->SetStyle(sf::String::Bold | sf::String::Underlined);

	Drawables->SortByLayerDepthAscendingWorldSpace();

	mScreenString1 = new ScreenString("ScreenString1", 42, ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE, 3);
	mScreenString1->SetColor(sf::Color(128, 0, 0, 128));

	mScreenSprite1 = new ScreenSprite("Content/Textures/sprite.png",
		ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::MIDDLE);
	mScreenSprite1->SetRotation(-298);

	boost::function<void (void)> onClick = boost::bind(&GameState_Logos::OnClick_btn1, this);
	btn1 = new GUIButton("Content/Textures/btnNormal.png",
						"Content/Textures/btnHover.png",
						"Content/Textures/btnActive.png",
						onClick,
						"gfx",
						PFWConstants::defaultFontFile,
						PFWConstants::fontSize,
						ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::TOP);
	btn1->SetScale(sf::Vector2f(4, 4));
	btn1->SetPosition(sf::Vector2f(-50, 100));

	toggleBtn1 = new GUIToggleButton("Content/Textures/toggle_inactive_normal.png",
						"Content/Textures/toggle_inactive_hover.png",
						"Content/Textures/toggle_active_normal.png",
						"Content/Textures/toggle_active_hover.png",
						0,
						0,
						"",
						PFWConstants::defaultFontFile,
						PFWConstants::fontSize,
						ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::TOP);
	toggleBtn1->SetScale(sf::Vector2f(3, 3));
	toggleBtn1->SetPosition(sf::Vector2f(50, 150));

	textField1 = new GUITextfield("Content/Textures/toggle_inactive_normal.png",
								"Content/Textures/toggle_active_normal.png",
								"starttext",
								PFWConstants::defaultFontFile,
								24,
								ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::BOTTOM,
								-100);
	textField1->SetScale(sf::Vector2f(2, 2));
	textField1->SetPosition(sf::Vector2f(50, -50));

	Drawables->SortByLayerDepthAscendingScreenSpace();

	itSfVideoMode = -1;
}

void GameState_Logos::OnClick_btn1 () {
}

GameState_Logos::~GameState_Logos () {
	delete fireClip;
	SpriteAnimationManager::Instance()->UnloadAnimation("burn");

	delete mWorldSprite1;
	delete mWorldSprite2;
	delete mWorldString1;

	delete mScreenString1;
	delete mScreenSprite1;

	delete btn1;
	delete toggleBtn1;
	delete textField1;

	delete mCrosshair;

	delete player;
delete player_collisiontest;

	delete wall1;

	Drawables->ClearAll();
}

void GameState_Logos::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	btn1->HandleSFEvents(sfEvents);
	toggleBtn1->HandleSFEvents(sfEvents);
	textField1->HandleSFEvents(sfEvents);

	player->HandleSFEvents(sfEvents);

	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::Closed:
				Game->GetRenderWindow()->Close();
			break;
			case sf::Event::KeyPressed:
				switch (itSfEvent->Key.Code) {
					case sf::Key::Escape:
						Game->GetRenderWindow()->Close();
					break;
					case sf::Key::F:
						Game->ToggleFullScreen();
					break;
					case sf::Key::C: {

						if (Game->IsFullScreen()) {
							// try to skip non monitor aspect ratio fullscreen resolutions
							// base on the "assumption" that the 0th mode as being the best is of the monitor's
								// aspect ratio

							unsigned int initial_itSfVideoMode = itSfVideoMode;

							bool cool = false;

							// Should never run end up in an infinite loop
							do {
								itSfVideoMode++;
								if ( itSfVideoMode > (sf::VideoMode::GetModesCount() - 1) ) {
									itSfVideoMode = 0;
								}

								float a = (float)sf::VideoMode::GetMode(itSfVideoMode).Width
									/ (float)sf::VideoMode::GetMode(itSfVideoMode).Height;
								float b = (float)sf::VideoMode::GetMode(0).Width
										/ (float)sf::VideoMode::GetMode(0).Height;

								if (a == b)
									cool = true;
							} while (!cool);

							if (sf::VideoMode::GetMode(initial_itSfVideoMode).Width
										== sf::VideoMode::GetMode(itSfVideoMode).Width
									&& sf::VideoMode::GetMode(initial_itSfVideoMode).Height
										== sf::VideoMode::GetMode(itSfVideoMode).Height) {
								// Should work
								break;
							}
						}
						else {
							itSfVideoMode++;
							if ( itSfVideoMode > (sf::VideoMode::GetModesCount() - 1) ) {
								itSfVideoMode = 0;
							}
						}

						Game->Create(sf::VideoMode::GetMode(itSfVideoMode).Width,
															sf::VideoMode::GetMode(itSfVideoMode).Height);

					} break;
					default:
						// supress warnings
					break;
				}
			break;
			default:
				// supress warnings
			break;
		}

		itSfEvent++;
	}
}

void GameState_Logos::Enter () {
}

void GameState_Logos::Update (float dT) {
	const sf::Input& sfInput = Game->GetRenderWindow()->GetInput();

	float zoomSpd = 5;
	float zoomFactor = 1;
	if (sfInput.IsKeyDown(sf::Key::Add)) {
		zoomFactor += zoomSpd * dT;
	}
	else if (sfInput.IsKeyDown(sf::Key::Subtract)) {
		zoomFactor -= zoomSpd * dT;
	}
	Game->GetWorldCamera2D()->Zoom(zoomFactor);
	mScreenString1->Rotate(12);

	float camSpd = 350;
	sf::Vector2f worldOffset;
	if (sfInput.IsKeyDown(sf::Key::Left)) {
		worldOffset.x -= camSpd * dT;
	}
	else if (sfInput.IsKeyDown(sf::Key::Right)) {
		worldOffset.x += camSpd * dT;
	}
	if (sfInput.IsKeyDown(sf::Key::Up)) {
		worldOffset.y -= camSpd * dT;
	}
	else if (sfInput.IsKeyDown(sf::Key::Down)) {
		worldOffset.y += camSpd * dT;
	}
	Game->GetWorldCamera2D()->Move(worldOffset);
	//mScreenString1->Move(viewOffset); // no good..offset is in worldviewspace not in screenviewspace

	player->Update(dT);

	fireClip->Update(dT);
}

void GameState_Logos::Exit () {
}
