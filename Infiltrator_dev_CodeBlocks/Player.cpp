#include "Player.hpp"

#include <cmath>

#include "PFrameWork/SFMLGameManager.hpp"
#include "PFrameWork/SpriteAnimationManager.hpp"

Player::Player (int layerDepth)
		: DrawableBase(layerDepth),
		Character(350, 720, "Content/Textures/Animated/player.anim", "player_walk", layerDepth) {
	//baseLook(0, 1), // looking down since that's how the animation is at 0 rotation

	// load additional animation files here
}

Player::~Player () {
	SpriteAnimationManager::Instance()->UnloadAnimation("player_walk");
}
#include <iostream>
void Player::Update (float dT) {
	const sf::Input& sfInput = Game->GetRenderWindow()->GetInput();

	//std::cout << sf::Key::Left << std::endl;

/**/
	// keyboard
	int l = -sfInput.IsKeyDown(sf::Key::A);//sf::Key::Code(291));
	int r = sfInput.IsKeyDown(sf::Key::D);
	int x = l + r;
	int u = -sfInput.IsKeyDown(sf::Key::W);
	int d = sfInput.IsKeyDown(sf::Key::S);
	int y = u + d;

	WalkInDir(dT, sf::Vector2f(x, y));
/**/

/**/
	// mouse
	if (sfInput.IsMouseButtonDown(sf::Mouse::Left)) {
		sf::Vector2f target = Game->ConvertCoordsWindowToWorld(sf::Vector2f(sfInput.GetMouseX(),
																			sfInput.GetMouseY()));

		//was->SetPosition(target);
		LookAt(target);
		//WalkTowards(dT, target);
	}
/**/
}

void Player::HandleSFEvents (std::list<sf::Event>& sfEvents) {
}
