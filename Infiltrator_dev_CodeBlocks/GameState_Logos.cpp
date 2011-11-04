#include "GameState_Logos.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "DrawManager.hpp"

#include "SFMLGameManager.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>

#include "MyConstants.hpp"

#include "Player.hpp"

#include "Input.hpp"

GameState_Logos::GameState_Logos () {
	mCrosshair = new ScreenSprite("Content/Textures/crosshair.png",
		ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE, 100);
	DrawManager::Instance()->AddScreenSpace(mCrosshair);

	player = new Player();
	//player->SetPosition(sf::Vector2f(512, 384));

	mWorldSprite1 = new WorldSprite("Content/Textures/sprite.png");
	DrawManager::Instance()->Add(mWorldSprite1);
	mWorldSprite1->SetPosition(sf::Vector2f(600, 100));
	mWorldSprite1->SetRotation(37);

	mWorldSprite2 = new WorldSprite("Content/Textures/background_waterfall.jpg", -1);
	DrawManager::Instance()->Add(mWorldSprite2);

	mWorldString1 = new WorldString("Hello World", 36);
	DrawManager::Instance()->Add(mWorldString1);
	mWorldString1->SetPosition(sf::Vector2f(300, 200));
	mWorldString1->SetStyle(sf::String::Bold | sf::String::Underlined);

	DrawManager::Instance()->SortByLayerDepthAscendingWorld();

	mScreenString1 = new ScreenString("ScreenString1", 42, ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::MIDDLE, 3);
	DrawManager::Instance()->AddScreenSpace(mScreenString1);
	mScreenString1->SetColor(sf::Color(128, 0, 0, 128));

	mScreenSprite1 = new ScreenSprite("Content/Textures/sprite.png",
		ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::MIDDLE);
	DrawManager::Instance()->AddScreenSpace(mScreenSprite1);
	mScreenSprite1->SetRotation(-298);

	boost::function<void (void)> onClick = boost::bind(&GameState_Logos::OnClick_btn1, this);
	btn1 = new GUIButton("Content/Textures/btnNormal.png",
						"Content/Textures/btnHover.png",
						"Content/Textures/btnActive.png",
						onClick,
						"gfx",
						MConst::defaultFontFile,
						MConst::fontSize,
						ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::TOP);
	btn1->SetScale(sf::Vector2f(4, 4));
	DrawManager::Instance()->AddScreenSpace(btn1);
	btn1->SetPosition(sf::Vector2f(-50, 100));

	toggleBtn1 = new GUIToggleButton("Content/Textures/toggle_inactive_normal.png",
						"Content/Textures/toggle_inactive_hover.png",
						"Content/Textures/toggle_active_normal.png",
						"Content/Textures/toggle_active_hover.png",
						0,
						0,
						"",
						MConst::defaultFontFile,
						MConst::fontSize,
						ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::TOP);
	toggleBtn1->SetScale(sf::Vector2f(3, 3));
	DrawManager::Instance()->AddScreenSpace(toggleBtn1);
	toggleBtn1->SetPosition(sf::Vector2f(50, 150));

	textField1 = new GUITextfield("Content/Textures/toggle_inactive_normal.png",
								"Content/Textures/toggle_active_normal.png",
								"starttext",
								MConst::defaultFontFile,
								24,
								ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::BOTTOM,
								-100);
	textField1->SetScale(sf::Vector2f(2, 2));
	textField1->SetPosition(sf::Vector2f(50, -50));
	DrawManager::Instance()->AddScreenSpace(textField1);

	DrawManager::Instance()->SortByLayerDepthAscendingScreenSpace();

	itSfVideoMode = -1;
}

void GameState_Logos::OnClick_btn1 () {
}

GameState_Logos::~GameState_Logos () {
	DrawManager::Instance()->Pop(mWorldSprite1);
	delete mWorldSprite1;
	DrawManager::Instance()->Pop(mWorldSprite2);
	delete mWorldSprite2;
	DrawManager::Instance()->Pop(mWorldString1);
	delete mWorldString1;

	DrawManager::Instance()->PopScreenSpace(mScreenString1);
	delete mScreenString1;
	DrawManager::Instance()->PopScreenSpace(mScreenSprite1);
	delete mScreenSprite1;

	DrawManager::Instance()->PopScreenSpace(btn1);
	delete btn1;
	DrawManager::Instance()->PopScreenSpace(toggleBtn1);
	delete toggleBtn1;
	DrawManager::Instance()->PopScreenSpace(textField1);
	delete textField1;

	DrawManager::Instance()->PopScreenSpace(mCrosshair);
	delete mCrosshair;

	DrawManager::Instance()->ClearAll();
}

void GameState_Logos::HandleEvents(std::list<sf::Event>& sfEvents) {
	btn1->HandleEvents(sfEvents);
	toggleBtn1->HandleEvents(sfEvents);
	textField1->HandleEvents(sfEvents);

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
/*
case sf::Event::MouseButtonPressed:
	mWAS->ChangeFrame();
break;
*/
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
	//mScreenString1->Rotate(zoomFactor);

	float camSpd = 350;
	sf::Vector2f viewOffset;
	if (sfInput.IsKeyDown(sf::Key::Left)) {
		viewOffset.x -= camSpd * dT;
	}
	else if (sfInput.IsKeyDown(sf::Key::Right)) {
		viewOffset.x += camSpd * dT;
	}
	if (sfInput.IsKeyDown(sf::Key::Up)) {
		viewOffset.y -= camSpd * dT;
	}
	else if (sfInput.IsKeyDown(sf::Key::Down)) {
		viewOffset.y += camSpd * dT;
	}
	Game->GetWorldCamera2D()->Move(viewOffset);
	mScreenString1->Move(viewOffset);



#include <iostream>

/**/
	// keyboard
	int l = -sfInput.IsKeyDown(sf::Key::A);
	int r = sfInput.IsKeyDown(sf::Key::D);
	int x = l + r;
	int u = -sfInput.IsKeyDown(sf::Key::W);
	int d = sfInput.IsKeyDown(sf::Key::S);
	int y = u + d;

	player->WalkInDir(dT, sf::Vector2f(x, y) * 11123.0f); // will be normalized - noprob
/**/

/**/
	// mouse
	if (sfInput.IsMouseButtonDown(sf::Mouse::Left)) {
		sf::Vector2f target =
			Game->GetRenderWindow()->ConvertCoords(
				sfInput.GetMouseX(),
				sfInput.GetMouseY());
		target = sf::Vector2f(
			target.x * (MConst::baseResolution.x / Game->GetRenderWindow()->GetWidth()),
			target.y * (MConst::baseResolution.y / Game->GetRenderWindow()->GetHeight()));

		//Game->GetWorldCamera2D()->SetCenter(target);

		//player->WalkInDir(dT, target - player->GetPosition()); // not good for mouse
		//player->LookAt(target);
		player->WalkTowards(dT, target);
	}
/**/

}

void GameState_Logos::Exit () {
}
