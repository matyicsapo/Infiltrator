#include "Player.hpp"

#include <cmath>

#include "PFrameWork/SFMLGameManager.hpp"
#include "PFrameWork/SpriteAnimationManager.hpp"
#include "PFrameWork/GO/WorldShape.hpp"
#include "MyMagicNumbers.hpp"
#include "PFrameWork/GO/SoundEffect.hpp"
#include "PFrameWork/SoundManager.hpp"

Player::Player (DrawManager* const drawManager, int layerDepth)
		: GameObject(ID_PLAYER),
		DrawableBase(layerDepth, ID_PLAYER),
		Character(drawManager, 350, 720,
					"Content/Textures/Animated/player.anim", "player_walk", layerDepth, ID_PLAYER) {
	//baseLook(0, 1), // looking down since that's how the animation is at 0 rotation

	// load additional animation files here

	visionTriangle = new WorldShape(new sf::Shape());
	sf::Vector2f A(0, 0);
	sf::Vector2f B(100, 0);
	sf::Vector2f C(50, 65);
	visionTriangle->AddPoint(A);
	visionTriangle->AddPoint(B);
	visionTriangle->AddPoint(C);
	visionTriangle->SetCenter(C);
	visionTriangle->SetColor(sf::Color(128, 128, 128, 255));
//	drawManager->AddWorldSpace(visionTriangle);
	visionTriangle->SetLayerDepth(LAYER_VISIONTRIANGLE);

	SetPosition(GetPosition());

	drawManager->SortAscendingWorld();

	footsteps = new SoundEffect("Content/Audio/foot.wav");
	footsteps->SetLoop(true);
	footsteps->SetBaseVolume(10);
	Sounds->RefreshVol_Effect_Of(footsteps);
}

Player::~Player () {
//	SpriteAnimationManager::Instance()->UnloadAnimation("player_walk");

	drawManager->PopWorldSpace(visionTriangle);
	delete visionTriangle;

	drawManager->PopWorldSpace(this);
}

void Player::SetPosition (sf::Vector2f pos) {
	Character::SetPosition(pos);

	if (visionTriangle)
		visionTriangle->SetPosition(basePos);
}

void Player::Move (sf::Vector2f offset) {
	Character::Move(offset);

	if (visionTriangle)
		visionTriangle->Move(offset);
}

void Player::Rotate (float angle) {
	Character::Rotate(angle);

	if (visionTriangle)
		visionTriangle->Rotate(angle);
}

bool Player::CanInteractWith (ColliderBase* object) {
	sf::Vector2f T[visionTriangle->GetNbPoints()];
	for (unsigned int i = 0; i != visionTriangle->GetNbPoints(); ++i) {
		T[i] = visionTriangle->GetPointPositionTransformed(i);
	}

	return Colliders->IsColliding(T, visionTriangle->GetNbPoints(), object);
}

void Player::Update (float dT) {
	const sf::Input& sfInput = Game->GetRenderWindow()->GetInput();

	//std::cout << sf::Key::Left << std::endl;

	// keyboard
//	int l = 0;
//	if (sfInput.IsKeyDown(sf::Key::A) || sfInput.IsKeyDown(sf::Key::Left) {
//		l = -1;
//	}
	int l = -(sfInput.IsKeyDown(sf::Key::A)/* || sfInput.IsKeyDown(sf::Key::Left)*/);//sf::Key::Code(291));
	int r = (sfInput.IsKeyDown(sf::Key::D)/* || sfInput.IsKeyDown(sf::Key::Right)*/);
	int x = l + r;
	int u = -(sfInput.IsKeyDown(sf::Key::W)/* || sfInput.IsKeyDown(sf::Key::Up)*/);
	int d = (sfInput.IsKeyDown(sf::Key::S)/* || sfInput.IsKeyDown(sf::Key::Down)*/);
	int y = u + d;

	WalkInDir(dT, sf::Vector2f(x, y));

//	// mouse
//	if (sfInput.IsMouseButtonDown(sf::Mouse::Left)) {
//		sf::Vector2f target = Game->ConvertCoordsWindowToWorld(sf::Vector2f(sfInput.GetMouseX(),
//																			sfInput.GetMouseY()));
//
////		std::cout << sfInput.GetMouseX() << " " << target.x << std::endl;
//
//		//was->SetPosition(target);
//		LookAt(target);
//		WalkTowards(dT, target);
//	}

	LateUpdate(dT);
}

void Player::HandleSFEvents (std::list<sf::Event>& sfEvents) {
}
