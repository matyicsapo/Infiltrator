#include "GameState_BasePlayable.hpp"

#include <boost/bind.hpp>
#include <fstream>

#include "../PFrameWork/SFMLGameManager.hpp"
#include "../PFrameWork/StateMachine.hpp"
#include "../PFrameWork/GO/Colliders.hpp"
#include "../DarkFader.hpp"
#include "../PFrameWork/GO/ScreenTimeAnimatedSprite.hpp"
#include "../PFrameWork/GO/ScreenString.hpp"
#include "../TilingButton.hpp"
#include "../GroundSprite.hpp"
#include "../PFrameWork/GO/WorldShape.hpp"
#include "../Player.hpp"
#include "../Princess.hpp"
#include "../Goal.hpp"
#include "../WallRectangle.hpp"
#include "../PFrameWork/GameObjectManager.hpp"
#include "../Guard.hpp"
#include "../Coin.hpp"
#include "../Chest.hpp"
#include "../proto/InfiltratorLevel.pb.h"
#include "../PFrameWork/TINI/TINI.hpp"
#include "../Session.hpp"
#include "../GameplayHUD.hpp"
#include "../PFrameWork/GO/SoundEffect.hpp"
#include "../PFrameWork/SoundManager.hpp"
#include "../PFrameWork/Utilities.hpp"

using namespace Infiltrator;

GameState_BasePlayable::GameState_BasePlayable (std::string levelPath)
		: navMapGenThread(0),
		navMapGenThreadState(NONE),
		loadingIcon(0),
		loadingText(0),
		cancelNavMapGenBtn(0),
		groundSprite(0),
		levelBoundsDebug(0),
		levelBoundColliders(0),
		finished(false),
		HUD(0),
		interacteeHighlight(0),
		player(0),
		princess(0),
//		goal(0),
		chest(0),
		levelPath(levelPath),
		inoutFader(0) {
	mMenuStateMachine = new StateMachine<MenuState>();

	objectives[0] = 0;
	objectives[1] = 0;
}

GameState_BasePlayable::~GameState_BasePlayable () {
	ClearAll();

	delete HUD;
	delete interacteeHighlight;

	delete inoutFader;

	delete loadingIcon;
	delete loadingText;
	delete cancelNavMapGenBtn;

	delete groundSprite;

	delete levelBoundsDebug;

	ClearMap();

	delete mMenuStateMachine;

	delete[] levelBoundColliders;

	delete navMapGenThread;

	SpriteAnimationManager::Release();
	ColliderManager::Release();
}

void GameState_BasePlayable::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	mMenuStateMachine->RefreshStack();

	if (inoutFader != 0) {
		if (cancelNavMapGenBtn)
			cancelNavMapGenBtn->HandleSFEvents(sfEvents);

		std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
		while (itSfEvent != sfEvents.end()) {
			switch (itSfEvent->Type) {
				case sf::Event::Closed:
					Game->GetRenderWindow()->Close();
				break;
				default:
				break;
			}

			++itSfEvent;
		}

		return;
	}

	if (!mMenuStateMachine->IsEmpty())
		mMenuStateMachine->HandleSFEvents(sfEvents);

	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::Closed:
				Game->GetRenderWindow()->Close();
			break;
			case sf::Event::KeyPressed:
				switch (itSfEvent->Key.Code) {
					case sf::Key::Escape:
						if (!finished) {
							if (!mMenuStateMachine->IsEmpty())
								OnPause(false);
							else
								OnPause(true);
						}
					break;
					case sf::Key::Space:
						if (HUD) {
							GameObject* interactee = HUD->GetInteractee();
							if (interactee) {
								if (interactee == chest && chest) {
									goldPickedUp += MyMagicNumbers::goldValue_chest;

									for (unsigned int i = 0; i != 2; ++i) {
										if (objectives[i] == chest) {
											OnObjectiveCompleted(i);
											objectives[i] = 0;
										}
									}
									PopWorldSpace(chest);
									delete chest;
									chest = 0;

									SoundEffect* coinJingle = new SoundEffect("Content/Audio/coin.wav");
									coinJingle->oneShot = true;
									Sounds->RefreshVol_Effect_Of(coinJingle);
									coinJingle->Play();

									HUD->SetAmountGold(goldPickedUp);

									HUD->SetInteractContextNote(0);
								}
								else if (/*interactee == princess && */princess) {
									princess->StartFollowing(princess->IsFollowingAnyone() ?
																0 : player);

									HUD->SetInteractContextNote(0);


									// if following than it's considered completed..
									if (princess->IsFollowingAnyone()) {
										for (unsigned int i = 0; i != 2; ++i) {
											// if it was the active objective
											if (objectives[i] == princess
													&& objectives[i]->objectiveState == ACTIVE) {
												OnObjectiveCompleted(i);
											}
										}
									}
									else {
										// However..
										for (unsigned int i = 0; i != 2; ++i) {
											// if the active objective is a goal
											if (objectives[i]
													&& objectives[i]->objectiveState == ACTIVE
													&& objectives[i]->entityType == ID_GOAL) {

												// looking backwards
												for (int j = i - 1; j >= 0; --j) {
													// j represents the last objective

													// if it was this princess
													if (objectives[j] == princess) {
														// then that princess should have been brought here

														// so we set the current goal state to inactive, and hide it
														objectives[i]->objectiveState = INACTIVE;
														PopWorldSpace(dynamic_cast<Goal*>(objectives[i]));

														// and reactivate the former princess objective
														objectives[j]->objectiveState = ACTIVE;
														HUD->SetObjective("Content/Textures/princess_thumbnail.png");
													}
												}

											}
										}
									}

								}
							}
						}
					break;
					default:
					break;
				}
			break;
			default:
			break;
		}

		++itSfEvent;
	}

	if (finished) {
		if (player) player->PauseAudio();
		if (princess) princess->PauseAudio();

		const EntityMap& guards = (*GameObjects)[ID_GUARD];
		for (EntityMap::const_iterator itG = guards.begin(); itG != guards.end(); itG++) {
			dynamic_cast<Guard*>(itG->second)->PauseAudio();
		}
	}
}

void GameState_BasePlayable::Init () {
	GameState::Init();

	groundSprite = new GroundSprite("Content/Textures/grass.png", -10);
	AddWorldSpace(groundSprite);

	LoadLevel(levelPath);

	Game->GetWorldCamera2D()->SetZoom(1);

	boost::function<void (void)> onFinishFadeLight = boost::bind(&GameState_BasePlayable::OnFinishFadeLight, this);
	inoutFader = new DarkFader(.25f, false, onFinishFadeLight, 5000);
	AddScreenSpace(inoutFader);
}

void GameState_BasePlayable::OnPause (bool pause) {
	if (pause) {
		if (player) player->PauseAudio();
		if (princess) princess->PauseAudio();

		const EntityMap& guards = (*GameObjects)[ID_GUARD];
		for (EntityMap::const_iterator itG = guards.begin(); itG != guards.end(); itG++) {
			dynamic_cast<Guard*>(itG->second)->PauseAudio();
		}
	}
}

void GameState_BasePlayable::Update (float dT) {
	if (inoutFader) {
		inoutFader->Update(dT);

		if (loadingIcon)
			loadingIcon->Update(dT);

		if (navMapGenThreadState == SUCCESS) {
			OnFinishedNavMapGen(true);
		}
		else if (navMapGenThreadState == FAILURE) {
			OnFinishedNavMapGen(false);
		}

		return;
	}

	if (!mMenuStateMachine->IsEmpty()) {
		mMenuStateMachine->Update(dT);
	}
}

void GameState_BasePlayable::UpdateGamePlay (float dT) {
	const sf::Input& sfInput = Game->GetRenderWindow()->GetInput();

	if (sfInput.IsKeyDown(sf::Key::Add)) Game->GetWorldCamera2D()->Zoom(1 + CAM_ZOOM_SPD * dT);
	else if (sfInput.IsKeyDown(sf::Key::Subtract)) Game->GetWorldCamera2D()->Zoom(1 - CAM_ZOOM_SPD * dT);

	if (player) {
		player->Update(dT);

		Game->GetWorldCamera2D()->SetCenterOfView(player->GetPosition());

		if (chest && player->CanInteractWith(chest)) {
			HUD->SetInteractContextNote(chest, "chest_collect");
		}
		else if (princess && player->CanInteractWith(princess)) {
			HUD->SetInteractContextNote(princess, princess->IsFollowingAnyone() ?
													"princess_stop" : "princess_follow");
		}
		else {
			HUD->SetInteractContextNote(0);
		}
	}
	else {
		sf::Vector2f worldOffset(0, 0);

		if (sfInput.IsKeyDown(sf::Key::Left)) worldOffset.x -= CAM_MOVE_SPD * dT;
		else if (sfInput.IsKeyDown(sf::Key::Right)) worldOffset.x += CAM_MOVE_SPD * dT;
		if (sfInput.IsKeyDown(sf::Key::Up)) worldOffset.y -= CAM_MOVE_SPD * dT;
		else if (sfInput.IsKeyDown(sf::Key::Down)) worldOffset.y += CAM_MOVE_SPD * dT;

		if (worldOffset != sf::Vector2f(0, 0)) Game->GetWorldCamera2D()->Move(worldOffset);
	}

	if (princess) princess->Update(dT);

	if (HUD->GetInteractee()) {
		if (!interacteeHighlight) {
			interacteeHighlight = new WorldSprite("Content/Textures/glow.png",
													LAYER_INTERACTEEHIGHLIGHT);
			AddWorldSpace(interacteeHighlight);

			SortAscendingWorld();
		}

		interacteeHighlight->SetPosition(HUD->GetInteractee()->GetPosition());
	}
	else {
		PopWorldSpace(interacteeHighlight);
		delete interacteeHighlight;
		interacteeHighlight = 0;
	}

	const EntityMap& guards = (*GameObjects)[ID_GUARD];
	Guard* guard;
	for (EntityMap::const_iterator itG = guards.begin(); itG != guards.end(); itG++) {
		guard = dynamic_cast<Guard*>(itG->second);

		guard->Update(dT);

		if (player) {
			if (guard->CanSeePlayer(player)) {
				OnGameFinished(false);
			}
		}
	}
}

void GameState_BasePlayable::DrawScreenSpace (sf::RenderWindow& rwin) {
	GameState::DrawScreenSpace(rwin);

	if (HUD) HUD->DrawScreenSpace(rwin);

	mMenuStateMachine->DrawScreenSpace(rwin);
}

void GameState_BasePlayable::OnFinishFadeLight () {
	PopScreenSpace(inoutFader);
	delete inoutFader;
	inoutFader = 0;
}

StateMachine<MenuState>* GameState_BasePlayable::GetMenuStateMachine () {
	return mMenuStateMachine;
}

InvisibleRectCollider** GameState_BasePlayable::GetLevelBoundColliders () {
	return levelBoundColliders;
}

void GameState_BasePlayable::LoadLevel (std::string levelPath) {
	Nav->SetNavMap(0);
	ClearMap();
	objectives[0] = 0;
	objectives[1] = 0;

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	std::fstream levelFile(levelPath.c_str(), std::ios::in | std::ios::binary);
	if (!levelFile.is_open()) {
//std::cout << "!levelFile.is_open()" << std::endl;
		OnLevelLoaded(false);
		return;
	}

	Level level;

	if (!level.ParseFromIstream(&levelFile)) {
//std::cout << "!level.ParseFromIstream(&levelFile)" << std::endl;
		google::protobuf::ShutdownProtobufLibrary();
		OnLevelLoaded(false);
		return;
	}

	// LOAD DATA ===
	this->levelName = level.levelname();

	SetLevelSize(sf::Vector2f(level.levelsize().x(), level.levelsize().y()));

	for (int itG = 0; itG != level.guards_size(); itG++) {
		Level_Guard* plGuard =  level.mutable_guards(itG);

		Guard* newGuard = new Guard(this, LAYER_GUARD);
		AddWorldSpace(newGuard);
		newGuard->SetPosition(sf::Vector2f(plGuard->mutable_pos()->x(), plGuard->mutable_pos()->y()));

		for (int itWP = 0; itWP != plGuard->waypoints_size(); itWP++) {
			newGuard->WPushBack(new sf::Vector2i(plGuard->mutable_waypoints(itWP)->x(),
												plGuard->mutable_waypoints(itWP)->y()));
		}
	}

	// all the simple WorldObj stuff
	for (int itS = 0; itS != level.stuff_size(); itS++) {
		Level_WorldObj* plStuff = level.mutable_stuff(itS);
		WorldDrawable* newGO = 0;
		switch (plStuff->type()) {
			case Level_WorldObjType_ROCKWALL:
				newGO = new WallRectangle(TEXTURE_ROCKWALL, Collision::STATIC, LAYER_ROCKWALL, ID_ROCKWALL);
			break;
			case Level_WorldObjType_ROCK01:
				newGO = new WallRectangle(TEXTURE_ROCK01, Collision::STATIC, LAYER_ROCK01, ID_ROCK01);
			break;
			case Level_WorldObjType_ROCK02:
				newGO = new WallRectangle(TEXTURE_ROCK02, Collision::STATIC, LAYER_ROCK02, ID_ROCK02);
			break;
			case Level_WorldObjType_BUSH:
				newGO = new WallCircle(TEXTURE_BUSH, Collision::STATIC, LAYER_BUSH, ID_BUSH);
			break;
			case Level_WorldObjType_PINE:
				newGO = new WallCircle(TEXTURE_PINE, Collision::STATIC, LAYER_PINE, ID_PINE);
			break;
			case Level_WorldObjType_ROOF:
				newGO = new WallRectangle(TEXTURE_ROOF, Collision::STATIC, LAYER_ROOF, ID_ROOF);
			break;
			case Level_WorldObjType_ROAD:
				newGO = new WorldSprite(TEXTURE_ROAD, LAYER_ROAD, ID_ROAD);
			break;
			case Level_WorldObjType_PLAYER:
				newGO = new Player(this, LAYER_PLAYER);
				player = dynamic_cast<Player*>(newGO);
			break;
			case Level_WorldObjType_PRINCESS:
				newGO = new Princess(this, LAYER_PRINCESS);
				princess = dynamic_cast<Princess*>(newGO);
			break;
			case Level_WorldObjType_COIN:
				newGO = new Coin(LAYER_COIN);
			break;
			case Level_WorldObjType_CHEST:
				newGO = new Chest(LAYER_CHEST);
				chest = dynamic_cast<Chest*>(newGO);
			break;
			case Level_WorldObjType_GOAL:
				newGO = new Goal(LAYER_GOAL);
//				goal = dynamic_cast<Goal*>(newGO);
			break;
		}
		AddWorldSpace(newGO);
		newGO->SetPosition(sf::Vector2f(plStuff->mutable_pos()->x(), plStuff->mutable_pos()->y()));
	}

	if (level.has_mission1st()) {
		Level_WorldObj* plStuff = level.mutable_mission1st();
		sf::Vector2f posMission(plStuff->mutable_pos()->x(), plStuff->mutable_pos()->y());

		switch (level.mutable_mission1st()->type()) {
			case Level_WorldObjType_CHEST:
				objectives[0] = chest;
			break;
			case Level_WorldObjType_PRINCESS:
				objectives[0] = princess;
			break;
			case Level_WorldObjType_GOAL: {
				const EntityMap& entities = GameObjects->GetEntityMap(ID_GOAL);
				GameObject* GO = entities.begin()->second;
				for (EntityMap::const_iterator itE = ++entities.begin(); itE != entities.end(); ++itE) {
					if ( Utils->Dst(posMission, itE->second->GetPosition()) < Utils->Dst(GO->GetPosition(), itE->second->GetPosition()) ) {
						GO = itE->second;
					}
				}

				objectives[0] = dynamic_cast<BaseObjective*>(GO);
			} break;
			default: break;
		}
	}
	if (level.has_mission2nd()) {
		Level_WorldObj* plStuff = level.mutable_mission2nd();
		sf::Vector2f posMission(plStuff->mutable_pos()->x(), plStuff->mutable_pos()->y());

		switch (level.mutable_mission2nd()->type()) {
			case Level_WorldObjType_CHEST:
				objectives[1] = chest;
			break;
			case Level_WorldObjType_PRINCESS:
				objectives[1] = princess;
			break;
			case Level_WorldObjType_GOAL: {
				const EntityMap& entities = GameObjects->GetEntityMap(ID_GOAL);
				GameObject* GO = entities.begin()->second;
				for (EntityMap::const_iterator itE = ++entities.begin(); itE != entities.end(); ++itE) {
					if ( Utils->Dst(posMission, itE->second->GetPosition()) < Utils->Dst(GO->GetPosition(), itE->second->GetPosition()) ) {
						GO = itE->second;
					}
				}

				objectives[1] = dynamic_cast<BaseObjective*>(GO);
			} break;
			default: break;
		}
	}

//	development_level = level.usercreated();

	int navmapnodes_size = level.navmapnodes_size();
	if (navmapnodes_size > 0) {
		NavMap* navMap = new NavMap();

		for (int i = 0; i != navmapnodes_size; ++i) {
			const Level_NavMapNode& ilN = level.navmapnodes(i);

			sf::Vector2i iNPos(ilN.position().x(), ilN.position().y());

			std::deque<sf::Vector2i> iNAdjacents;

			int adjacentpositions_size = ilN.adjacentpositions_size();
			if (adjacentpositions_size > 0) {
				for (int j = 0; j != adjacentpositions_size; ++j) {
					iNAdjacents.push_back(
						sf::Vector2i(ilN.adjacentpositions(j).x(), ilN.adjacentpositions(j).y()));
				}
			}

			navMap->insert( std::pair< sf::Vector2i, std::deque<sf::Vector2i> >(iNPos, iNAdjacents) );
		}

		Nav->SetNavMap(navMap);
	}
	// === LOAD DATA

	google::protobuf::ShutdownProtobufLibrary();

	SortAscendingWorld();

	OnLevelLoaded(true);
}

void GameState_BasePlayable::SetLevelSize (sf::Vector2f levelSize) {
	this->levelSize = levelSize;

	PopWorldSpace(levelBoundsDebug);
	delete levelBoundsDebug;
	levelBoundsDebug = new WorldShape(new sf::Shape(
		sf::Shape::Rectangle(0, 0, levelSize.x, levelSize.y,
			sf::Color::Yellow, 5, sf::Color(96, 96, 96))),
		0);
	AddWorldSpace(levelBoundsDebug);
	levelBoundsDebug->EnableFill(false);

	if (levelBoundColliders != 0) {
		for (int i = 0; i != 4; ++i) {
			delete levelBoundColliders[i];
		}
	}
	delete[] levelBoundColliders;
	levelBoundColliders = new InvisibleRectCollider *[4];
	// left
	levelBoundColliders[0] = new InvisibleRectCollider(
		sf::Vector2f(MyMagicNumbers::characterRadius * 4,
					levelSize.y + MyMagicNumbers::characterRadius * 2 * 4));
	levelBoundColliders[0]->SetPosition(sf::Vector2f(0 - MyMagicNumbers::characterRadius * 2,
													levelSize.y / 2.0f));
	// right
	levelBoundColliders[1] = new InvisibleRectCollider(
		sf::Vector2f(MyMagicNumbers::characterRadius * 4,
					levelSize.y + MyMagicNumbers::characterRadius * 2 * 4));
	levelBoundColliders[1]->SetPosition(sf::Vector2f(levelSize.x + MyMagicNumbers::characterRadius * 2,
													levelSize.y / 2.0f));
	// top
	levelBoundColliders[2] = new InvisibleRectCollider(
		sf::Vector2f(levelSize.x + MyMagicNumbers::characterRadius * 2 * 4,
					MyMagicNumbers::characterRadius * 4));
	levelBoundColliders[2]->SetPosition(sf::Vector2f(levelSize.x / 2.0f,
													0 - MyMagicNumbers::characterRadius * 2));
	// bottom
	levelBoundColliders[3] = new InvisibleRectCollider(
		sf::Vector2f(levelSize.x + MyMagicNumbers::characterRadius * 2 * 4,
					MyMagicNumbers::characterRadius * 4));
	levelBoundColliders[3]->SetPosition(sf::Vector2f(levelSize.x / 2.0f,
													levelSize.y + MyMagicNumbers::characterRadius * 2));

	Game->GetWorldCamera2D()->SetWorldBounds(levelSize * Game->GetWindowFakeScale() / 2.0f);
	Game->GetWorldCamera2D()->SetWorldCenter(levelSize * Game->GetWindowFakeScale() / 2.0f);
	Game->GetWorldCamera2D()->SetCenterOfView(levelSize * Game->GetWindowFakeScale() / 2.0f);
}

void GameState_BasePlayable::DeleteAll (unsigned int entityType) {
	const EntityMap& entities = GameObjects->GetEntityMap(entityType);
	for (EntityMap::const_iterator itE = entities.begin(); itE != entities.end(); ++itE) {
//std::cout << "  deleting a: " << entityType << std::endl;
		PopWorldSpace(dynamic_cast<WorldDrawable*>(itE->second));
		delete itE->second;
	}
}

void GameState_BasePlayable::ClearMap () {
	DeleteAll(ID_WAYPOINT);
	DeleteAll(ID_ROCKWALL);
	DeleteAll(ID_ROCK01);
	DeleteAll(ID_ROCK02);
	DeleteAll(ID_PINE);
	DeleteAll(ID_BUSH);
	DeleteAll(ID_ROOF);
	DeleteAll(ID_ROAD);
	DeleteAll(ID_PLAYER); player = 0;
	DeleteAll(ID_PRINCESS); princess = 0;
	DeleteAll(ID_GUARD);
	DeleteAll(ID_CHEST); chest = 0;
	DeleteAll(ID_COIN);
	DeleteAll(ID_GOAL);
}

void GameState_BasePlayable::PickedUpGold (int gold, WorldDrawable* thing) {
	goldPickedUp += gold;

	HUD->SetAmountGold(goldPickedUp);

	PopWorldSpace(thing);
}

void GameState_BasePlayable::OnFinishedNavMapGen (bool successful) {
	delete navMapGenThread;
	navMapGenThread = 0;

	navMapGenThreadState = NONE;

	PopScreenSpace(loadingIcon);
	delete loadingIcon;
	loadingIcon = 0;

	PopScreenSpace(loadingText);
	delete loadingText;
	loadingText = 0;

	PopScreenSpace(cancelNavMapGenBtn);
	delete cancelNavMapGenBtn;
	cancelNavMapGenBtn = 0;
}

void GameState_BasePlayable::DoGenerateNavMap () {
	SpriteAnimationManager::Instance()->LoadAnimations("Content/Textures/Animated/player.anim");
	loadingIcon = new ScreenTimeAnimatedSprite((*SpriteAnimationManager::Instance())["player_walk"],
		ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::BOTTOM, 6000);
	loadingIcon->SetPosition(sf::Vector2f(-100, -100));
	AddScreenSpace(loadingIcon);

	TINI::TINIObject langFile("Content/LanguageFiles/" + CurrentSession->language + ".ini");
	loadingText = new ScreenString(langFile.GetValue("misc", "loading"),
		PFWConstants::defaultFontSize * 1.2, ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::BOTTOM, 6000);
	loadingText->SetPosition(sf::Vector2f(-225, -125));
	AddScreenSpace(loadingText);

	std::string txInactiveNormal[3] {
		"Content/Textures/GUI/darkbuttonleft.png",
		"Content/Textures/GUI/darkbuttonmiddle.png",
		"Content/Textures/GUI/darkbuttonright.png" };
	std::string txInactiveHover[3] {
		"Content/Textures/GUI/buttonleft.png",
		"Content/Textures/GUI/buttonmiddle.png",
		"Content/Textures/GUI/buttonright.png" };
	std::string txActiveHover[3] {
		"Content/Textures/GUI/pressedbuttonleft.png",
		"Content/Textures/GUI/pressedbuttonmiddle.png",
		"Content/Textures/GUI/pressedbuttonright.png" };

	boost::function<void (void)> event = boost::bind(&Navigation::CancelGenerateNavMap, Nav);
	cancelNavMapGenBtn = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							langFile.GetValue("menu", "cancel"),
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::BOTTOM,
							6000);
	cancelNavMapGenBtn->SetPosition(sf::Vector2f(100, -100));

	ThreadData_GenerateNavMap* d = new ThreadData_GenerateNavMap(navMapGenThreadState, sf::Vector2i(levelSize.x, levelSize.y));
	navMapGenThread = new sf::Thread(&Async_GenerateNavMap, d);
	navMapGenThread->Launch();
}

void GameState_BasePlayable::SaveLevel (std::string levelPath, std::string levlistFilePath) {
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	Level level;

	// SAVE DATA ===
	level.set_levelname(levelName);

	Level_Vector2D* plLevelSize = level.mutable_levelsize();
	plLevelSize->set_x(levelSize.x);
	plLevelSize->set_y(levelSize.y);

	if (objectives[0] != 0) {
		Level_WorldObj* plMission1st = level.mutable_mission1st();

		plMission1st->set_type(ConvertType(objectives[0]->entityType));

		// redundant save - not using it..this is just a reference
		Level_Vector2D* plMission1stPos = plMission1st->mutable_pos();
		plMission1stPos->set_x(objectives[0]->GetPosition().x);
		plMission1stPos->set_y(objectives[0]->GetPosition().y);
	}
	if (objectives[1] != 0) {
		Level_WorldObj* plMission2nd = level.mutable_mission2nd();

		plMission2nd->set_type(ConvertType(objectives[1]->entityType));

		Level_Vector2D* plMission2ndPos = plMission2nd->mutable_pos();
		plMission2ndPos->set_x(objectives[1]->GetPosition().x);
		plMission2ndPos->set_y(objectives[1]->GetPosition().y);
	}

	const EntityMap& guards = (*GameObjects)[ID_GUARD];
	for (EntityMap::const_iterator itG = guards.begin(); itG != guards.end(); itG++) {
		Level_Guard* plGuard = level.add_guards();

		Level_Vector2D* plGuardPos = plGuard->mutable_pos();
		plGuardPos->set_x(itG->second->GetPosition().x);
		plGuardPos->set_y(itG->second->GetPosition().y);

		std::deque<sf::Vector2i*>& wayPoints = dynamic_cast<Guard*>(itG->second)->GetWayPoints();
		for (std::deque<sf::Vector2i*>::iterator itWP = wayPoints.begin(); itWP != wayPoints.end(); itWP++) {
			Level_Vector2D* plWayPoint = plGuard->add_waypoints();
			plWayPoint->set_x((*itWP)->x);
			plWayPoint->set_y((*itWP)->y);
		}
	}

	SaveStuff(ID_ROCKWALL, level);
	SaveStuff(ID_ROCK01, level);
	SaveStuff(ID_ROCK02, level);
	SaveStuff(ID_BUSH, level);
	SaveStuff(ID_PINE, level);
	SaveStuff(ID_ROOF, level);
	SaveStuff(ID_ROAD, level);
	SaveStuff(ID_COIN, level);

	if (player) SaveStuff(player, level);

	// there are might be more "illegaly" as the screensprites have the same 'entityType'
	if (princess) SaveStuff(princess, level);
	if (chest) SaveStuff(chest, level);

//	if (goal) SaveStuff(goal, level);
	const EntityMap& entities = GameObjects->GetEntityMap(ID_GOAL);
	for (EntityMap::const_iterator itE = entities.begin(); itE != entities.end(); ++itE) {
		if (dynamic_cast<Goal*>(itE->second)) SaveStuff(itE->second, level);
	}

	NavMap* navMap = Nav->GetNavMap();
	if (navMap) {
		for (NavMap::iterator itN = navMap->begin(); itN != navMap->end(); itN++) {
			Level_NavMapNode* plNavMapNode = level.add_navmapnodes();

			Level_Vector2D* plNavMapNodePosition = plNavMapNode->mutable_position();
			plNavMapNodePosition->set_x(itN->first.x);
			plNavMapNodePosition->set_y(itN->first.y);

			for (std::deque<sf::Vector2i>::iterator itA = itN->second.begin();
					itA != itN->second.end(); ++itA) {
				Level_Vector2D* plAdjPos = plNavMapNode->add_adjacentpositions();

				plAdjPos->set_x((*itA).x);
				plAdjPos->set_y((*itA).y);
			}
		}
	}
	else level.clear_navmapnodes();
	// === SAVE DATA

//	if (!level.IsInitialized()) {
////std::cout << "not all required fields were set" << std::endl;
//		google::protobuf::ShutdownProtobufLibrary();
//		return;
//	}

	bool failed = false;
	std::fstream levelFile(levelPath.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	if (levelFile.is_open()) {
		if (!level.SerializeToOstream(&levelFile)) {
//std::cout << "Level::SerializeToOStream() FAILED" << std::endl;
			//google::protobuf::ShutdownProtobufLibrary();
			failed = true;
		}
		levelFile.close();
	}

	if (!failed && levlistFilePath != "") {
//std::cout << levelName << std::endl;
		TINI::TINIObject* levList = new TINI::TINIObject(levlistFilePath);
		levList->SetValue("", levelName, "1");
		delete levList;
	}

	google::protobuf::ShutdownProtobufLibrary();
}

Level_WorldObjType GameState_BasePlayable::ConvertType (unsigned int typeID) {
	switch (typeID) {
		case ID_ROCKWALL: return Level_WorldObjType_ROCKWALL;
		case ID_ROCK01: return Level_WorldObjType_ROCK01;
		case ID_ROCK02: return Level_WorldObjType_ROCK02;
		case ID_BUSH: return Level_WorldObjType_BUSH;
		case ID_PINE: return Level_WorldObjType_PINE;
		case ID_ROOF: return Level_WorldObjType_ROOF;
		case ID_ROAD: return Level_WorldObjType_ROAD;
		case ID_COIN: return Level_WorldObjType_COIN;
		case ID_PLAYER: return Level_WorldObjType_PLAYER;
		case ID_PRINCESS: return Level_WorldObjType_PRINCESS;
		case ID_CHEST: return Level_WorldObjType_CHEST;
		case ID_GOAL: return Level_WorldObjType_GOAL;
		default: assert(0); break;
	}
}

void GameState_BasePlayable::SaveStuff (unsigned int entityType, Level& level) {
	const EntityMap& entities = GameObjects->GetEntityMap(entityType);
	for (EntityMap::const_iterator itE = entities.begin(); itE != entities.end(); itE++) {
		SaveStuff(itE->second, level);
	}
}

void GameState_BasePlayable::SaveStuff (GameObject* entity, Level& level) {
	Level_WorldObj* plStuff = level.add_stuff();


	Level_Vector2D* plStuffPos = plStuff->mutable_pos();
	plStuffPos->set_x(entity->GetPosition().x);
	plStuffPos->set_y(entity->GetPosition().y);

	plStuff->set_type(ConvertType(entity->entityType));
}

void GameState_BasePlayable::OnLanguageChanged (CfgContents& contents_lang) {
	mMenuStateMachine->OnLanguageChanged(contents_lang);

	if (HUD) HUD->OnLanguageChanged(contents_lang);
}

void GameState_BasePlayable::EnteredGoal (Goal* goal) {
	for (unsigned int i = 0; i != 2; ++i) {
		if (objectives[i] == goal) {
			if (goal->objectiveState == ACTIVE) {
				OnObjectiveCompleted(i);

				PopWorldSpace(goal);
				GameObjects->Delete(goal);
				objectives[i] = 0;

				return;
			}
		}
	}
}

void GameState_BasePlayable::StartPlaying () {
	goldPickedUp = 0;

	if (player) {
		Game->GetWorldCamera2D()->SetCenterOfView(player->GetPosition());
	}

	if (Nav->GetNavMap()) {
		const EntityMap& guards = (*GameObjects)[ID_GUARD];
		for (EntityMap::const_iterator itG = guards.begin(); itG != guards.end(); itG++) {
			dynamic_cast<Guard*>(itG->second)->StartFollowingWayPoints();
		}
	}

	Game->GetWorldCamera2D()->SetZoom(1);

	HUD = new GameplayHUD();

	bool foundFirst = false;
	for (unsigned int i = 0; i != 2; ++i) {
		if (objectives[i]) {
			if (foundFirst) {
				objectives[i]->objectiveState = INACTIVE;

				if (objectives[i]->entityType == ID_GOAL)
					PopWorldSpace(dynamic_cast<Goal*>(objectives[i]));
			}
			else {
				if (objectives[i]->objectiveState != ACTIVE
						&& objectives[i]->entityType == ID_GOAL) {
					AddWorldSpace(dynamic_cast<Goal*>(objectives[i]));
				}

				objectives[i]->objectiveState = ACTIVE;

				switch (objectives[i]->entityType) {
					case ID_GOAL:
						HUD->SetObjective("Content/Textures/goal.png");
					break;
					case ID_CHEST:
						HUD->SetObjective("Content/Textures/chest.png");
					break;
					case ID_PRINCESS:
						HUD->SetObjective("Content/Textures/princess_thumbnail.png");
					break;
				}

				foundFirst = true;
			}
		}
	}
}

void GameState_BasePlayable::OnObjectiveCompleted (unsigned int objectiveIndex) {
	objectives[objectiveIndex]->objectiveState = COMPLETED;

	for (unsigned int i = 0; i != 2; ++i) {
		if (objectives[i] != 0 && objectives[i]->objectiveState != COMPLETED) {

			if (objectives[i]->objectiveState != ACTIVE
					&& objectives[i]->entityType == ID_GOAL) {
				AddWorldSpace(dynamic_cast<Goal*>(objectives[i]));
			}

			objectives[i]->objectiveState = ACTIVE;

			switch (objectives[i]->entityType) {
				case ID_GOAL:
					HUD->SetObjective("Content/Textures/goal.png");
				break;
				case ID_CHEST:
					HUD->SetObjective("Content/Textures/chest.png");
				break;
				case ID_PRINCESS:
					HUD->SetObjective("Content/Textures/princess_thumbnail.png");
				break;
			}

			if (objectives[i]->entityType == ID_PRINCESS && princess->IsFollowingAnyone()) {
				OnObjectiveCompleted(i);
			}

			return;
		}
	}

	HUD->SetObjective("");

	OnGameFinished(true);
}

void GameState_BasePlayable::OnGameFinished (bool successful) {
	finished = true;

	SoundEffect* finishSFX;
	finishSFX = successful ? new SoundEffect("Content/Audio/congrats.wav")
							: new SoundEffect("Content/Audio/caught.wav");
	finishSFX->oneShot = true;
	Sounds->RefreshVol_Effect_Of(finishSFX);
	finishSFX->Play();
}
