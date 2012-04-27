#include "GameState_Editor.hpp"

#include <boost/bind.hpp>

#include "../PFrameWork/StateMachine.hpp"
#include "../EditorToolbar.hpp"
#include "../EditorMenubar.hpp"
#include "../EditorLevelInfobar.hpp"
#include "../MyMagicNumbers.hpp"
#include "../DarkFader.hpp"
#include "../MenuStates/MenuState_EditorPause.hpp"
#include "../PFrameWork/GO/WorldSprite.hpp"
#include "../PFrameWork/GO/WorldShape.hpp"
#include "../Guard.hpp"
#include "../Princess.hpp"
#include "../PFrameWork/GameObjectManager.hpp"
#include "../PFrameWork/TINI/TINI.hpp"
#include "../WallRectangle.hpp"
#include "../Goal.hpp"
#include "../Coin.hpp"
#include "../Chest.hpp"
#include "../Player.hpp"
#include "../VisualItemSlot.hpp"
#include "../GameplayHUD.hpp"
#include <fstream>

GameState_Editor::GameState_Editor (std::string levelPath)
		: GameState_BasePlayable(levelPath),
		testtempNavMap(0),
		placeElement(0),
		dragee(0),
		screenDragee(0),
		selected(0),
		selectedWayPoint(0),
		activeToolID(-1) {
}

GameState_Editor::~GameState_Editor () {
	ClearAll();

	// if program was terminated during testing then this might have dynamic memory associated with it
	// this deletes the current if there was and sets the above mentioned
	// thus it is managed by the Navigation singleton now and will be cleaned up
	Nav->SetNavMap(testtempNavMap);

	delete menuBar;
	delete toolBar;
	delete levelInfobar;

	for (std::deque<WorldShape*>::iterator itN = navMapDebugNodes.begin(); itN != navMapDebugNodes.end(); itN++) {
		delete *itN;
	}
	navMapDebugNodes.clear();
}

void GameState_Editor::Init () {
	levelInfobar = new EditorLevelInfobar(20);

	GameState_BasePlayable::Init();

	boost::function<void (int)> event = boost::bind(&GameState_Editor::OnToolActivated, this, _1);
	toolBar = new EditorToolbar(event, 20);

	event = boost::bind(&GameState_Editor::OnMenuActivated, this, _1);
	std::vector<MenuDescription*> menus;
	menus.push_back(new MenuDescription(ID_EDMENUBAR_WALLS, TEXTURE_ROCKWALL,
										"editor_menubar", "colliders", sf::Vector2f(0, 10)));
	menus.push_back(new MenuDescription(ID_EDMENUBAR_PAINT, TEXTURE_ROAD,
										"editor_menubar", "ground", sf::Vector2f(140, 10)));
	menus.push_back(new MenuDescription(ID_EDMENUBAR_CHARACTERS, TEXTURE_PLAYER,
										"editor_menubar", "characters", sf::Vector2f(280, 10)));
	menus.push_back(new MenuDescription(ID_EDMENUBAR_LEVELINFO, "Content/Textures/map.png",
										"editor_menubar", "levelinfo", sf::Vector2f(420, 10)));
	menus.push_back(new MenuDescription(ID_EDMENUBAR_NEWMAP, "Content/Textures/newmap.png",
										"editor_menubar", "newmap", sf::Vector2f(600, 10)));
	menuBar = new EditorMenubar(event, menus, 0, 50);

	levelInfobar->SetLevelName(levelName);

	OnPause(true);

	SortAscendingScreen();
}

void GameState_Editor::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	GameState_BasePlayable::HandleSFEvents(sfEvents);

	if (inoutFader || !mMenuStateMachine->IsEmpty())
		return;

	if (!testing) {
		menuBar->HandleSFEvents(sfEvents);
		toolBar->HandleSFEvents(sfEvents);

		levelInfobar->HandleSFEvents(sfEvents);

		if (dragee != 0) {
			const sf::Input& sfInput = Game->GetRenderWindow()->GetInput();

			if (levelInfobar->GetScreenBounds().Contains(sfInput.GetMouseX(), sfInput.GetMouseY())) {
				if ( ! menuBar->GetScreenBounds().Contains(sfInput.GetMouseX(), sfInput.GetMouseY()) ) {
					switch (dragee->entityType) {
						case ID_CHEST:
							levelInfobar->SetObjectiveDragee(ID_CHEST, chest->ID, chest->GetPosition());
						break;
						case ID_PRINCESS:
							levelInfobar->SetObjectiveDragee(ID_PRINCESS, princess->ID, princess->GetPosition());
						break;
						case ID_GOAL:
							levelInfobar->SetObjectiveDragee(ID_GOAL, dragee->ID, dragee->GetPosition());
						break;
					}
				}
				else levelInfobar->SetObjectiveDragee(0, 0, sf::Vector2f(0, 0));
			}
			else levelInfobar->SetObjectiveDragee(0, 0, sf::Vector2f(0, 0));
		}
		else levelInfobar->SetObjectiveDragee(0, 0, sf::Vector2f(0, 0));

		std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
		while (itSfEvent != sfEvents.end()) {
			switch (itSfEvent->Type) {
				case sf::Event::Closed:
					Game->GetRenderWindow()->Close();
				break;
				case sf::Event::KeyPressed:
					switch (itSfEvent->Key.Code) {
						case sf::Key::Escape:
							OnPause(true);
						break;
						case sf::Key::Delete:
							if (selected) {
								if (TryDeleteSelected(ID_PLAYER)) {	player = 0; break; }
								if (TryDeleteSelected(ID_PRINCESS)) { princess = 0; break; }
								if (TryDeleteSelected(ID_GOAL)) { break; }
								if (TryDeleteSelected(ID_CHEST)) { chest = 0; break; }
								if (TryDeleteSelected(ID_COIN)) { break; }
								if (TryDeleteSelected(ID_ROCKWALL)) { break; }
								if (TryDeleteSelected(ID_ROCK01)) { break; }
								if (TryDeleteSelected(ID_ROCK02)) { break; }
								if (TryDeleteSelected(ID_PINE)) { break; }
								if (TryDeleteSelected(ID_BUSH)) { break; }
								if (TryDeleteSelected(ID_ROOF)) { break; }
								if (TryDeleteSelected(ID_ROAD)) { break; }

								if (selected->entityType == ID_GUARD) {
									if (!selectedWayPoint) {
										if (TryDeleteSelected(ID_GUARD)) { OnGuardSelected(0); break; }
									}
									else {
										const EntityMap& waypoints = GameObjects->GetEntityMap(ID_WAYPOINT);
										for (EntityMap::const_iterator itWP = waypoints.begin();
												itWP != waypoints.end(); ++itWP) {

											if (itWP->second == selectedWayPoint) {
												Guard* guard = dynamic_cast<Guard*>(selected);
												sf::Vector2f* searchedPos = new sf::Vector2f(selectedWayPoint->GetPosition());
												if (originalDrageePos != selectedWayPoint->GetPosition()) {
													searchedPos = new sf::Vector2f(originalDrageePos);
												}
												guard->WRemove(sf::Vector2i(searchedPos->x, searchedPos->y));
												dragee = 0;
												PopWorldSpace(dynamic_cast<WorldDrawable*>(itWP->second));
												delete itWP->second;
												selectedWayPoint = 0;

												break;
											}

										}
									}
								}
							}
						default:
						break;
					}
				break;
				case sf::Event::MouseMoved:
					if (testing)
						break;

					if (placeElement != 0) {
						switch (activeToolID) {
							case ID_PLAYER: {
								if (player == 0) {
									player = new Player(this, LAYER_PLAYER);
									player->SetPosition(placeElement->GetPosition());
									if (Colliders->IsColliding(player) || !IsInLevelBounds(placeElement->GetPosition()))
										placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
									else placeElement->SetColor(MyMagicNumbers::tool_color_allow);
									delete player;
									player = 0;
								}
								else placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
							} break;
							case ID_GUARD: {
								Guard* guard = new Guard(this, LAYER_GUARD);
								guard->SetPosition(placeElement->GetPosition());
								if (Colliders->IsColliding(guard) || !IsInLevelBounds(placeElement->GetPosition()))
									placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
								else placeElement->SetColor(MyMagicNumbers::tool_color_allow);
								delete guard;
							} break;
							case ID_PRINCESS: {
								if (princess == 0) {
									princess = new Princess(this, LAYER_PRINCESS);
									princess->SetPosition(placeElement->GetPosition());
									if (Colliders->IsColliding(princess) || !IsInLevelBounds(placeElement->GetPosition()))
										placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
									else placeElement->SetColor(MyMagicNumbers::tool_color_allow);
									delete princess;
									princess = 0;
								}
								else placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
							} break;
							case ID_WAYPOINT: {
	// igazából csak azért Guard mert ekkor még nem volt InvisibleXXXCollider
								Guard* guard = new Guard(this, LAYER_GUARD);
								guard->SetPosition(placeElement->GetPosition());
								if (Colliders->IsColliding(guard) || !IsInLevelBounds(placeElement->GetPosition()))
									placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
								else placeElement->SetColor(MyMagicNumbers::tool_color_allow);
								delete guard;
							} break;
							case ID_COIN: {
								Guard* guard = new Guard(this, LAYER_GUARD);
								guard->SetPosition(placeElement->GetPosition());
								if (Colliders->IsColliding(guard) || !IsInLevelBounds(placeElement->GetPosition()))
									placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
								else placeElement->SetColor(MyMagicNumbers::tool_color_allow);
								delete guard;
							} break;
							case ID_CHEST: {
								if (chest == 0) {
									Guard* guard = new Guard(this, LAYER_GUARD);
									guard->SetPosition(placeElement->GetPosition());
									if (Colliders->IsColliding(guard) || !IsInLevelBounds(placeElement->GetPosition()))
										placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
									else placeElement->SetColor(MyMagicNumbers::tool_color_allow);
									delete guard;
								}
								else placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
							} break;
							case ID_GOAL: {
//								if (goal == 0) {
//									Guard* guard = new Guard(this, LAYER_GUARD);
//									guard->SetPosition(placeElement->GetPosition());
//									if (Colliders->IsColliding(guard) || !IsInLevelBounds(placeElement->GetPosition()))
//										placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
//									else placeElement->SetColor(MyMagicNumbers::tool_color_allow);
//									delete guard;
//								}
//								else placeElement->SetColor(MyMagicNumbers::tool_color_disallow);
							} break;
						}
					}
				break;
				case sf::Event::MouseButtonPressed:
					if (testing)
						break;

					if (itSfEvent->MouseButton.Button == sf::Mouse::Left) {
						if (placeElement != 0) {
							switch (activeToolID) {
								case ID_ROCKWALL: {
									WallRectangle* w =
										new WallRectangle(TEXTURE_ROCKWALL, Collision::STATIC, LAYER_ROCKWALL, ID_ROCKWALL);
									AddWorldSpace(w);
									w->SetPosition(placeElement->GetPosition());
									SortAscendingWorld();
									toolBar->SetActive(ID_ROCKWALL);

									OnNavMapStateChanged(CHANGED);
								} break;
								case ID_ROCK01: {
									WallRectangle* w =
										new WallRectangle(TEXTURE_ROCK01, Collision::STATIC, LAYER_ROCK01, ID_ROCK01);
									AddWorldSpace(w);
									w->SetPosition(placeElement->GetPosition());
									SortAscendingWorld();
									toolBar->SetActive(ID_ROCK01);

									OnNavMapStateChanged(CHANGED);
								} break;
								case ID_ROCK02: {
									WallRectangle* w =
										new WallRectangle(TEXTURE_ROCK02, Collision::STATIC, LAYER_ROCK02, ID_ROCK02);
									AddWorldSpace(w);
									w->SetPosition(placeElement->GetPosition());
									SortAscendingWorld();
									toolBar->SetActive(ID_ROCK02);

									OnNavMapStateChanged(CHANGED);
								} break;
								case ID_BUSH: {
									WallCircle* w =
										new WallCircle(TEXTURE_BUSH, Collision::STATIC, LAYER_BUSH, ID_BUSH);
									AddWorldSpace(w);
									w->SetPosition(placeElement->GetPosition());
									SortAscendingWorld();
									toolBar->SetActive(ID_BUSH);

									OnNavMapStateChanged(CHANGED);
								} break;
								case ID_PINE: {
									WallCircle* w =
										new WallCircle(TEXTURE_PINE, Collision::STATIC, LAYER_PINE, ID_PINE);
									AddWorldSpace(w);
									w->SetPosition(placeElement->GetPosition());
									SortAscendingWorld();
									toolBar->SetActive(ID_PINE);

									OnNavMapStateChanged(CHANGED);
								} break;
								case ID_ROOF: {
									WallRectangle* w =
										new WallRectangle(TEXTURE_ROOF, Collision::STATIC, LAYER_ROOF, ID_ROOF);
									AddWorldSpace(w);
									w->SetPosition(placeElement->GetPosition());
									SortAscendingWorld();
									toolBar->SetActive(ID_ROOF);

									OnNavMapStateChanged(CHANGED);
								} break;
								case ID_ROAD: {
									WorldSprite* w =
										new WorldSprite(TEXTURE_ROAD, LAYER_ROAD, ID_ROAD);
									AddWorldSpace(w);
									w->SetPosition(placeElement->GetPosition());
									SortAscendingWorld();
									toolBar->SetActive(ID_ROAD);
								} break;
								case ID_PLAYER: {
									if (player == 0) {
										player = new Player(this, LAYER_PLAYER);
										player->SetPosition(placeElement->GetPosition());
										if (Colliders->IsColliding(player) || !IsInLevelBounds(placeElement->GetPosition())) {
											delete player;
											player = 0;
										}
										else {
											SortAscendingWorld();
										}
									}
									toolBar->SetActive(ID_PLAYER);
								} break;
								case ID_GUARD: {
									Guard* guard = new Guard(this, LAYER_GUARD);
									guard->SetPosition(placeElement->GetPosition());
									if (Colliders->IsColliding(guard) || !IsInLevelBounds(placeElement->GetPosition()))
										delete guard;
									else {
										SortAscendingWorld();
									}
									toolBar->SetActive(ID_GUARD);
								} break;
								case ID_PRINCESS: {
									if (princess == 0) {
										princess = new Princess(this, LAYER_PRINCESS);
										princess->SetPosition(placeElement->GetPosition());
										if (Colliders->IsColliding(princess) || !IsInLevelBounds(placeElement->GetPosition())) {
											delete princess;
											princess = 0;
										}
										else {
											SortAscendingWorld();
										}
									}
									toolBar->SetActive(ID_PRINCESS);
								} break;
								case ID_WAYPOINT: {
									WorldSprite* w =
										new WorldSprite(TEXTURE_WAYPOINT, LAYER_WAYPOINT, ID_WAYPOINT);
									Guard* guard = dynamic_cast<Guard*>(selected);
									originalDrageePos = guard->GetPosition();
									guard->SetPosition(placeElement->GetPosition());
									if (Colliders->IsColliding(guard) || !IsInLevelBounds(placeElement->GetPosition())) {
										PopWorldSpace(w);
										delete w;
									}
									else {
										w->SetPosition(placeElement->GetPosition());
										guard->WPushBack(new sf::Vector2i(placeElement->GetPosition().x, placeElement->GetPosition().y));
										AddWorldSpace(w);
										SortAscendingWorld();
									}
									guard->SetPosition(originalDrageePos);
									toolBar->SetActive(ID_WAYPOINT);
								} break;
								case ID_COIN: {
									Coin* newCoin = new Coin(LAYER_COIN);
									newCoin->SetPosition(placeElement->GetPosition());
									if (Colliders->IsColliding(newCoin) || !IsInLevelBounds(placeElement->GetPosition())) {
										delete newCoin;
									}
									else {
										AddWorldSpace(newCoin);
										SortAscendingWorld();
									}
									toolBar->SetActive(ID_COIN);
								} break;
								case ID_CHEST: {
									if (chest == 0) {
										Chest* newChest = new Chest(LAYER_CHEST);
										newChest->SetPosition(placeElement->GetPosition());
										if (Colliders->IsColliding(newChest) || !IsInLevelBounds(placeElement->GetPosition())) {
											delete newChest;
										}
										else {
											chest = newChest;
											AddWorldSpace(chest);

											OnNavMapStateChanged(CHANGED);
										}
									}
									toolBar->SetActive(ID_CHEST);
								} break;
								case ID_GOAL: {
//									if (goal == 0) {
										Goal* newGoal = new Goal(LAYER_GOAL);
										newGoal->SetPosition(placeElement->GetPosition());
										if (/*Colliders->IsColliding(newGoal) || */!IsInLevelBounds(placeElement->GetPosition())) {
											delete newGoal;
										}
										else {
//											goal = newGoal;
											AddWorldSpace(newGoal);
										}
//									}
									toolBar->SetActive(ID_GOAL);
								} break;
							}
						}
						else {
							std::list<WorldDrawable*> wds = GetWorldDrawables();

							sf::Vector2f wMouse =
								Game->ConvertCoordsWindowToWorld(itSfEvent->MouseButton.X,
																itSfEvent->MouseButton.Y);

							Guard* originalGuard = dynamic_cast<Guard*>(selected);

							bool wasWaypointSelected = false;
							if (selectedWayPoint != 0) {
								const EntityMap& waypoints = GameObjects->GetEntityMap(ID_WAYPOINT);
								for (EntityMap::const_iterator itWP = waypoints.begin();
										itWP != waypoints.end(); ++itWP) {
									if (itWP->second == selectedWayPoint) {
										wasWaypointSelected = true;
										break;
									}
								}
							}

							selected = 0;
							selectedWayPoint = 0;

							// backwards, from highest layer to lowest, stops at ground if there is
							for (std::list<WorldDrawable*>::reverse_iterator itWD = wds.rbegin();
									itWD != wds.rend();
									itWD++) {
								if ((*itWD)->GetLayerDepth() <= -10)
									break; // not continue but break since it's ordered by layerdepth

								// HACKy skip..unselectable
								if ((*itWD)->GetLayerDepth() == LAYER_VISIONTRIANGLE)
									continue;

								if ((*itWD)->Contains(wMouse) && !selected) {
	//std::cout << "contains" << std::endl;
									if ((*itWD)->IsOpaque(wMouse)) {
	//std::cout << "opaque" << std::endl;
										dragee = *itWD;

										const EntityMap& waypoints = GameObjects->GetEntityMap(ID_WAYPOINT);
										for (EntityMap::const_iterator itWP = waypoints.begin();
												itWP != waypoints.end(); ++itWP) {
											if (itWP->second == *itWD) {
												selectedWayPoint = *itWD;
												break;
											}
										}

										if (! selectedWayPoint ) {
											selected = *itWD;
										}
										else {
											selected = originalGuard;
										}

										originalDrageePos = (*itWD)->GetPosition();

										lastMousePosInWorld = wMouse;
									}
									else {
	//std::cout << "NOT opaque" << std::endl;
										(*itWD)->SetColor(MyMagicNumbers::tool_color_normal);
									}
								}
								else {
	//std::cout << "NOT contains" << std::endl;
									(*itWD)->SetColor(MyMagicNumbers::tool_color_normal);
								}
							}

							if (selected)
								selected->SetColor(sf::Color(0, 0, 255, 255));

							if (selectedWayPoint)
								selectedWayPoint->SetColor(sf::Color(0, 0, 255, 255));

							Guard* guard = dynamic_cast<Guard*>(selected);
							if ((originalGuard != 0 || guard != 0 || wasWaypointSelected) && !selectedWayPoint)
								OnGuardSelected(guard);

						}
					}
					else if (itSfEvent->MouseButton.Button == sf::Mouse::Right) {
						PopWorldSpace(placeElement);
						delete placeElement;
						placeElement = 0;
					}
				break;
				default:
				break;
			}

			itSfEvent++;
		}
	}
}

void GameState_Editor::Update (float dT) {
	GameState_BasePlayable::Update(dT);

	if (inoutFader || !mMenuStateMachine->IsEmpty())
		return;

	if (testing) {
		if (finished) {
			SetTesting(false);
			OnPause(true);
		}
		else UpdateGamePlay(dT);
	}
	else {
		levelInfobar->Update(dT);

		const sf::Input& sfInput = Game->GetRenderWindow()->GetInput();

		// this is better handled with the realtime input system rather than events so the levelinfobar textfields
		// can't just eat the events..
		// i Could overwrite some SFML stuff and reset the values for these keys..naah..they would be reactivated
		// so i'd have to check the inputs in the textfields update too and would be only more convoluted
		if (!levelInfobar->IsAnyTextFieldActive()) {
			if (sfInput.IsKeyDown(sf::Key::Add)) {
				Game->GetWorldCamera2D()->Zoom(1 + CAM_ZOOM_SPD * dT);
			}
			else if (sfInput.IsKeyDown(sf::Key::Subtract)) {
				Game->GetWorldCamera2D()->Zoom(1 - CAM_ZOOM_SPD * dT);
			}

			sf::Vector2f worldOffset(0, 0);
			if (sfInput.IsKeyDown(sf::Key::Left)) {
				worldOffset.x -= CAM_MOVE_SPD * dT;
			}
			else if (sfInput.IsKeyDown(sf::Key::Right)) {
				worldOffset.x += CAM_MOVE_SPD * dT;
			}
			if (sfInput.IsKeyDown(sf::Key::Up)) {
				worldOffset.y -= CAM_MOVE_SPD * dT;
			}
			else if (sfInput.IsKeyDown(sf::Key::Down)) {
				worldOffset.y += CAM_MOVE_SPD * dT;
			}
			if (worldOffset != sf::Vector2f(0, 0))
				Game->GetWorldCamera2D()->Move(worldOffset);
		}

		if (placeElement != 0) {
			placeElement->SetPosition(
				Game->ConvertCoordsWindowToWorld(sfInput.GetMouseX(), sfInput.GetMouseY()));
		}
		else if (dragee != 0) {
			if ( ! sfInput.IsMouseButtonDown(sf::Mouse::Left) ) {
				levelInfobar->SetObjectiveDragee(0, 0, sf::Vector2f(0, 0));

				if ( (levelInfobar->GetScreenBounds().Contains(sfInput.GetMouseX(), sfInput.GetMouseY()))
						|| menuBar->GetScreenBounds().Contains(sfInput.GetMouseX(), sfInput.GetMouseY())
						|| toolBar->GetScreenBounds().Contains(sfInput.GetMouseX(), sfInput.GetMouseY()) ) {
					dragee->SetPosition(originalDrageePos);
				}
				else if (!IsInLevelBounds(dragee->GetPosition())) {
					dragee->SetPosition(originalDrageePos);
				}
				else if (dynamic_cast<Guard*>(dragee) != 0
						|| dynamic_cast<Player*>(dragee) != 0
						|| dynamic_cast<Princess*>(dragee)
						|| dragee->entityType == ID_COIN) {
					if (Colliders->IsColliding(dynamic_cast<ColliderBase*>(dragee))) {
						dragee->SetPosition(originalDrageePos);
					}
				}
				else {
					const EntityMap& waypoints = GameObjects->GetEntityMap(ID_WAYPOINT);
					for (EntityMap::const_iterator itWP = waypoints.begin(); itWP != waypoints.end(); ++itWP) {
						if (itWP->second == dragee) {
							if (selectedWayPoint != 0) {
								Guard* guard = dynamic_cast<Guard*>(selected);
								std::deque<sf::Vector2i*>& guardWPs = guard->GetWayPoints();

								for (std::deque<sf::Vector2i*>::iterator itPos = guardWPs.begin();
										itPos != guardWPs.end(); itPos++) {
									if ((**itPos) == sf::Vector2i(originalDrageePos.x, originalDrageePos.y)) {
										Guard* collTestGuard = new Guard(this, LAYER_GUARD);
										collTestGuard->SetPosition(dragee->GetPosition());
										std::set<ColliderBase*> exceptionColliders;
										exceptionColliders.insert(dynamic_cast<ColliderBase*>(guard));
										if (Colliders->IsColliding( collTestGuard, &exceptionColliders, Collision::STATIC )) {
											dragee->SetPosition(originalDrageePos);
										}
										else {
											*itPos = new sf::Vector2i(dragee->GetPosition().x, dragee->GetPosition().y);
										}
										delete collTestGuard;
										break;
									}
								}
								break;
							}
						}
					}
				}
				if (dragee->GetPosition().x != originalDrageePos.x || dragee->GetPosition().y != originalDrageePos.y) {
					ColliderBase* c = dynamic_cast<ColliderBase*>(dragee);
					if (c) {
						if ((Collision::STATIC & c->collisionType) != 0
								&& (Collision::TRIGGER & c->collisionType) == 0) {
							OnNavMapStateChanged(CHANGED);
						}
					}
				}
				dragee = 0;

			}
			else {
				sf::Vector2f wMouse = Game->ConvertCoordsWindowToWorld(sfInput.GetMouseX(),
																		sfInput.GetMouseY());

				dragee->Move(wMouse - lastMousePosInWorld);

				lastMousePosInWorld = wMouse;
			}
		}
	}
}

void GameState_Editor::DrawScreenSpace (sf::RenderWindow& rwin) {
	if (!testing) {
		toolBar->DrawScreenSpace(rwin);
		levelInfobar->DrawScreenSpace(rwin);
		menuBar->DrawScreenSpace(rwin);
	}

	GameState_BasePlayable::DrawScreenSpace(rwin);
}

void GameState_Editor::SetLevelSize (sf::Vector2f levelSize) {
	if (this->levelSize.x != levelSize.x || this->levelSize.y != levelSize.y) {
		OnNavMapStateChanged(CHANGED);
	}

	GameState_BasePlayable::SetLevelSize(levelSize);

	levelInfobar->SetTextLevelSize(levelSize);
}

sf::Vector2f GameState_Editor::GetLevelSize () {
	return sf::Vector2f(0, 0);
}

void GameState_Editor::ShowNavMap (bool show) {
	if (navMapState != NONE/* && navMapDebugNodes.size() != 0*/) {
		for (std::deque<WorldShape*>::iterator itN = navMapDebugNodes.begin();
				itN != navMapDebugNodes.end(); itN++) {
			PopWorldSpace(*itN);
			delete *itN;
		}
		navMapDebugNodes.clear();


		if (show) {
			NavMap* navMap = Nav->GetNavMap();

			navMapDebugNodes.clear();
			for (NavMap::iterator itN = navMap->begin(); itN != navMap->end(); itN++) {
				WorldShape* ws_node = new WorldShape(
						new sf::Shape(
							sf::Shape::Circle(sf::Vector2f(itN->first.x, itN->first.y), 5, sf::Color::Yellow, 1.5f, sf::Color::Blue)),
						0);
				AddWorldSpace(ws_node);
				navMapDebugNodes.push_back(ws_node);
			}

			SortAscendingWorld();
		}

	}
}

void GameState_Editor::InvokeGenerateNavMap () {
	if (navMapState == VALID)
		return;

	boost::function<void (void)> onFinishFadeDark = boost::bind(&GameState_Editor::DoGenerateNavMap, this);
	inoutFader = new DarkFader(.25f, true, onFinishFadeDark, 5000);
	AddScreenSpace(inoutFader);
}

void GameState_Editor::SetTesting (bool testing) {
	static NavMapState testtempNavMapState = NONE;
	static std::string testtempLevelPath = "";

	if (this->testing && !testing) {
		// just quit testing

		finished = false;

		// must be set before calling LoadLevel()
		this->testing = testing;

		LoadLevel(MyMagicNumbers::tempLevelPath);
		std::remove(MyMagicNumbers::tempLevelPath.c_str());

		navMapState = testtempNavMapState;
		OnNavMapStateChanged(navMapState);

		Nav->SetNavMap(testtempNavMap);
		testtempNavMap = 0;

		levelPath = testtempLevelPath;

		delete HUD;
		HUD = 0;

		PopWorldSpace(interacteeHighlight);
		delete interacteeHighlight;
		interacteeHighlight = 0;

	}
	else if (!this->testing && testing) {
		// just started testing

//		finished = false;

		this->testing = testing;

		testtempNavMapState = navMapState;

		NavMap* navMap = Nav->GetNavMap();
		testtempNavMap = navMap ? new NavMap(*navMap) : 0;

		testtempLevelPath = levelPath;

		SaveLevel(MyMagicNumbers::tempLevelPath, "");

		StartPlaying();
	}
}

void GameState_Editor::OnPause (bool pause) {
	GameState_BasePlayable::OnPause(pause);

	if (pause) {
		mMenuStateMachine->Push(new MenuState_EditorPause());

		if (selected != 0)
			selected->SetColor(sf::Color::White);

		if (dragee != 0) {
			if (!IsInLevelBounds(dragee->GetPosition())) {
				dragee->SetPosition(originalDrageePos);
			}
			else if (dynamic_cast<Guard*>(dragee) != 0 || dynamic_cast<Player*>(dragee) != 0) {
				if (Colliders->IsColliding(dynamic_cast<ColliderBase*>(dragee))) {
					dragee->SetPosition(originalDrageePos);
				}
			}
			if (dragee->GetPosition().x != originalDrageePos.x || dragee->GetPosition().y != originalDrageePos.y) {
				ColliderBase* c = dynamic_cast<ColliderBase*>(dragee);
				if (c) {
					if ((Collision::STATIC & c->collisionType) != 0
							&& (Collision::TRIGGER & c->collisionType) == 0) {
						OnNavMapStateChanged(CHANGED);
					}
				}
			}
			dragee = 0;
		}

		OnGuardSelected(0);

		levelInfobar->ResetGUIElements();
//		levelInfobar->ApplyLevelSize();
		levelInfobar->SetTextLevelSize(levelSize);

		PopWorldSpace(placeElement);
		delete placeElement;
		placeElement = 0;
	}
	else {
		mMenuStateMachine->PopBack();
	}
}

void GameState_Editor::OnFinishedNavMapGen (bool successful) {
	GameState_BasePlayable::OnFinishedNavMapGen(successful);

	PopScreenSpace(inoutFader);
	delete inoutFader;
	boost::function<void (void)> onFinishFadeLight = boost::bind(&GameState_Editor::OnFinishFadeLight, this);
	inoutFader = new DarkFader(.25f, false, onFinishFadeLight, 5000);
	AddScreenSpace(inoutFader);

	if (successful) {
		OnNavMapStateChanged(VALID);
	}
}

void GameState_Editor::OnLevelLoaded (bool successful) {
	if (successful) {
		originalLevelName = levelName;
	}
	else {
		originalLevelName = levelName = MyMagicNumbers::untitledLevelName;;
		levelInfobar->SetLevelName(levelName);

		std::string folders = MyMagicNumbers::development_build ? "Levels/Main/" : "Levels/User/";
		levelPath = folders + levelName + ".lev";

		SetLevelSize(MyMagicNumbers::defaultLevelSize);
	}
}

void GameState_Editor::OnToolActivated (int ID) {
	if (ID != -1) {
		PopWorldSpace(placeElement);
		delete placeElement;
		placeElement = 0;

		switch (ID) {
			case ID_ROCKWALL:
				placeElement = new WorldSprite(TEXTURE_ROCKWALL, LAYER_ROCKWALL);
			break;
			case ID_ROCK01:
				placeElement = new WorldSprite(TEXTURE_ROCK01, LAYER_ROCK01);
			break;
			case ID_ROCK02:
				placeElement = new WorldSprite(TEXTURE_ROCK02, LAYER_ROCK02);
			break;
			case ID_BUSH:
				placeElement = new WorldSprite(TEXTURE_BUSH, LAYER_BUSH);
			break;
			case ID_PINE:
				placeElement = new WorldSprite(TEXTURE_PINE, LAYER_PINE);
			break;
			case ID_ROOF:
				placeElement = new WorldSprite(TEXTURE_ROOF, LAYER_ROOF);
			break;
			case ID_ROAD:
				placeElement = new WorldSprite(TEXTURE_ROAD, LAYER_ROAD);
			break;
			case ID_PLAYER:
				placeElement = new WorldSprite(TEXTURE_PLAYER, LAYER_PLAYER);
			break;
			case ID_GUARD:
				placeElement = new WorldSprite(TEXTURE_GUARD, LAYER_GUARD);
			break;
			case ID_PRINCESS:
				placeElement = new WorldSprite(TEXTURE_PRINCESS, LAYER_PRINCESS);
			break;
			case ID_WAYPOINT:
				placeElement = new WorldSprite(TEXTURE_WAYPOINT, LAYER_WAYPOINT);
			break;
			case ID_COIN:
				placeElement = new WorldSprite(TEXTURE_COIN, LAYER_COIN);
			break;
			case ID_CHEST:
				placeElement = new WorldSprite(TEXTURE_CHEST, LAYER_CHEST);
			break;
			case ID_GOAL:
				placeElement = new WorldSprite(TEXTURE_GOAL, LAYER_GOAL);
			break;
		}

		if (placeElement)
			AddWorldSpace(placeElement);

		SortAscendingWorld();
		activeToolID = ID;

		placeElement->SetColor(MyMagicNumbers::tool_color_allow);
	}
}

void GameState_Editor::OnMenuActivated (int ID) {
	switch (ID) {
		case ID_EDMENUBAR_WALLS:
			OnClick_menuButtonWalls();
		break;
		case ID_EDMENUBAR_PAINT:
			OnClick_menuButtonGround();
		break;
		case ID_EDMENUBAR_CHARACTERS:
			OnClick_menuButtonCharacters();
		break;
		case ID_EDMENUBAR_LEVELINFO:
			OnClick_menuButtonLevelInfo();
		break;
		case ID_EDMENUBAR_NEWMAP:
			//OnClick_menuButtonLevelInfo();
			LoadLevel("");
		break;
	}
}

bool GameState_Editor::IsInLevelBounds (const sf::Vector2f& position) {
	if (position.x >= 0 && position.x <= levelSize.x
				&&
			position.y >= 0 && position.y <= levelSize.y) {
		return true;
	}

	return false;
}

void GameState_Editor::OnNavMapStateChanged (NavMapState navMapState) {
	if (this->navMapState == NONE && navMapState == CHANGED)
		return;

	this->navMapState = navMapState;
	levelInfobar->SetNavMapState(navMapState);

	if (navMapState == VALID) {
		if (levelInfobar->IsShowNavMap()) {
			ShowNavMap(true);
		}
	}
	else if (navMapState == NONE) {
		for (std::deque<WorldShape*>::iterator itN = navMapDebugNodes.begin();
				itN != navMapDebugNodes.end(); itN++) {
			PopWorldSpace(*itN);
			delete *itN;
		}
		navMapDebugNodes.clear();

		Nav->SetNavMap(0);
	}
}

void GameState_Editor::OnGuardSelected (Guard* guard) {
	const EntityMap& waypoints = GameObjects->GetEntityMap(ID_WAYPOINT);
	for (EntityMap::const_iterator itWP = waypoints.begin(); itWP != waypoints.end(); ++itWP) {
		PopWorldSpace(dynamic_cast<WorldDrawable*>(itWP->second));
		delete itWP->second;
	}

	if ( ! guard) {
		OnClick_menuButtonWalls();
		return;
	}

	std::vector<ToolDescription*> tools;

	tools.push_back(new ToolDescription(50, "Content/Textures/waypoint_thumbnail.png",
												"editor_tools", "waypoint"));

	toolBar->SetTools(tools, -1);

	std::deque<sf::Vector2i*> newWayPoints = guard->GetWayPoints();
	for (std::deque<sf::Vector2i*>::iterator itWP = newWayPoints.begin(); itWP != newWayPoints.end(); itWP++) {
		WorldSprite* ws = new WorldSprite(TEXTURE_WAYPOINT, LAYER_WAYPOINT, ID_WAYPOINT);
		AddWorldSpace(ws);
		ws->SetPosition(sf::Vector2f((**itWP).x, (**itWP).y));
	}

	SortAscendingWorld();
}

void GameState_Editor::OnClick_menuButtonWalls () {
	std::vector<ToolDescription*> tools;

	tools.push_back(new ToolDescription(ID_ROCKWALL, TEXTURE_ROCKWALL,
												"editor_tools", "rockwall"));
	tools.push_back(new ToolDescription(ID_ROCK01, TEXTURE_ROCK01,
												"editor_tools", "rock"));
	tools.push_back(new ToolDescription(ID_ROCK02, TEXTURE_ROCK02,
												"editor_tools", "rock"));
	tools.push_back(new ToolDescription(ID_BUSH, TEXTURE_BUSH,
												"editor_tools", "bush"));
	tools.push_back(new ToolDescription(ID_PINE, TEXTURE_PINE,
												"editor_tools", "pine"));
	tools.push_back(new ToolDescription(ID_ROOF, TEXTURE_ROOF,
												"editor_tools", "roof"));

	toolBar->SetTools(tools, -1);

	SortAscendingScreen();
}

void GameState_Editor::OnClick_menuButtonGround () {
	std::vector<ToolDescription*> tools;

	tools.push_back(new ToolDescription(ID_ROAD, TEXTURE_ROAD,
												"editor_tools", "road"));

	toolBar->SetTools(tools, -1);

	SortAscendingScreen();
}

void GameState_Editor::OnClick_menuButtonCharacters () {
	std::vector<ToolDescription*> tools;

	tools.push_back(new ToolDescription(ID_PLAYER, TEXTURE_PLAYER,
												"editor_tools", "player"));

	tools.push_back(new ToolDescription(ID_GUARD, TEXTURE_GUARD,
												"editor_tools", "guard"));

	tools.push_back(new ToolDescription(ID_PRINCESS, TEXTURE_PRINCESS,
												"editor_tools", "princess"));

	toolBar->SetTools(tools, -1);

	SortAscendingScreen();
}

void GameState_Editor::OnClick_menuButtonLevelInfo () {
	std::vector<ToolDescription*> tools;
	tools.push_back(new ToolDescription(ID_COIN, TEXTURE_COIN,
												"editor_tools", "gold"));
	tools.push_back(new ToolDescription(ID_CHEST, TEXTURE_CHEST,
												"editor_tools", "chest"));
	tools.push_back(new ToolDescription(ID_GOAL, TEXTURE_GOAL,
												"editor_tools", "goal"));
	toolBar->SetTools(tools, -1);
}

bool GameState_Editor::TryDeleteSelected (unsigned int entityType) {
	if (selected->entityType != entityType)
		return false;

//	const EntityMap& guards = (*GameObjects)[ID_GUARD];
//	for (EntityMap::const_iterator itG = guards.begin(); itG != guards.end(); itG++) {
//		if (selected == itG->second) {
//			PopWorldSpace(dynamic_cast<Guard*>(itG->second));
//			delete itG->second;
//			selected = 0;
//			dragee = 0;
//			OnGuardSelected(0);
//
//			break;
//		}
//	}

    ColliderBase* collider = dynamic_cast<ColliderBase*>(selected);
	if (collider && (collider->collisionType & Collision::TRIGGER) != 0)
		OnNavMapStateChanged(CHANGED);

	PopWorldSpace(selected);
	delete selected;
	selected = 0;
	dragee = 0;

	if (entityType == ID_CHEST || entityType == ID_GOAL || entityType == ID_PRINCESS) {
		unsigned int typeID = 0;
		unsigned int ID = 0;
		VisualItemSlot* objSlot = levelInfobar->Get1stObjectiveSlot();
		if (objSlot->GetContained(typeID, ID))
			if (typeID == entityType)
				objSlot->SetContained(0, 0, 0);

		objSlot = levelInfobar->Get2ndObjectiveSlot();
		if (objSlot->GetContained(typeID, ID))
			if (typeID == entityType)
				objSlot->SetContained(0, 0, 0);

	}

	return true;
}

void GameState_Editor::LoadLevel (std::string levelPath) {
	if (testing) {
		// this is the simplest and most effective way of solving the problems that
		// would arise when loading a level when testing
		testing = false;

		std::remove(MyMagicNumbers::tempLevelPath.c_str());
	}

	selected = 0;
	selectedWayPoint = 0;
	dragee = 0;
	screenDragee = 0;

	delete HUD;
	HUD = 0;

	// the ScreenSprites are given the proper IDs too so they are deleted along the real world objects
	// when ClearMap() is called in GameState_BasePlayable::LoadLevel(..)
	// so we preemptly clear them beforehand
	levelInfobar->Get1stObjectiveSlot()->SetContained(0, 0, 0);
	levelInfobar->Get2ndObjectiveSlot()->SetContained(0, 0, 0);

	this->levelPath = levelPath;

	GameState_BasePlayable::LoadLevel(levelPath);

	OnNavMapStateChanged(Nav->GetNavMap() ? VALID : NONE);

	levelInfobar->SetLevelName(levelName);
	levelInfobar->SetTextLevelSize(levelSize);

	if (objectives[0] != 0) {
		switch (objectives[0]->entityType) {
			case ID_CHEST: {
				ScreenSprite* ss = new ScreenSprite(TEXTURE_CHEST, 40, ID_CHEST);
				ss->SetScale(.6f);
				ss->SetFakeStuff(Game->GetWindowFakeScale());
				levelInfobar->Get1stObjectiveSlot()->SetContained(ss, ID_CHEST, chest->ID);
			} break;
			case ID_PRINCESS: {
				ScreenSprite* ss = new ScreenSprite(TEXTURE_PRINCESS, 40, ID_PRINCESS);
				ss->SetScale(.5f);
				ss->SetFakeStuff(Game->GetWindowFakeScale());
				levelInfobar->Get1stObjectiveSlot()->SetContained(ss, ID_PRINCESS, princess->ID);
			} break;
			case ID_GOAL: {
				ScreenSprite* ss = new ScreenSprite(TEXTURE_GOAL, 40, ID_GOAL);
				ss->SetScale(.5f);
				ss->SetFakeStuff(Game->GetWindowFakeScale());

				levelInfobar->Get1stObjectiveSlot()->SetContained(ss, ID_GOAL, objectives[0]->ID);
			} break;
			default:
				// just to supress warnings
			break;
		}
	}

	if (objectives[1] != 0) {
		switch (objectives[1]->entityType) {
			case ID_CHEST: {
				ScreenSprite* ss = new ScreenSprite(TEXTURE_CHEST, 40, ID_CHEST);
				ss->SetScale(.6f);
				ss->SetFakeStuff(Game->GetWindowFakeScale());
				levelInfobar->Get2ndObjectiveSlot()->SetContained(ss, ID_CHEST, chest->ID);
			} break;
			case ID_PRINCESS: {
				ScreenSprite* ss = new ScreenSprite(TEXTURE_PRINCESS, 40, ID_PRINCESS);
				ss->SetScale(.5f);
				ss->SetFakeStuff(Game->GetWindowFakeScale());
				levelInfobar->Get2ndObjectiveSlot()->SetContained(ss, ID_PRINCESS, princess->ID);
			} break;
			case ID_GOAL: {
				ScreenSprite* ss = new ScreenSprite(TEXTURE_GOAL, 40, ID_GOAL);
				ss->SetScale(.5f);
				ss->SetFakeStuff(Game->GetWindowFakeScale());

				levelInfobar->Get2ndObjectiveSlot()->SetContained(ss, ID_GOAL, objectives[1]->ID);
			} break;
			default:
				// just to supress warnings
			break;
		}
	}
}

void GameState_Editor::UpdateLevelName () {
	levelName = levelInfobar->GetLevelName();
	if (levelName == "") {
		levelName = MyMagicNumbers::untitledLevelName;;
		levelInfobar->SetLevelName(levelName);
	}
}

void GameState_Editor::SetObjective (unsigned int index, BaseObjective* obj) {
	objectives[index] = obj;
}

void GameState_Editor::SaveLevel (std::string levelPath, std::string levlistFilePath) {
	if (navMapState == CHANGED) {
		OnNavMapStateChanged(NONE);
	}

	GameState_BasePlayable::SaveLevel(levelPath, levlistFilePath);
}

void GameState_Editor::InvokSaveLevel () {
	if (originalLevelName != levelName) {
		std::string folders = MyMagicNumbers::development_build ? "Levels/Main/" : "Levels/User/";

		SaveLevel(folders + levelName + ".lev", folders + "levlist.ini");
	}
	else {
		SaveLevel(levelPath, levelPath.substr(0, levelPath.find_last_of("/\\") + 1) + "levlist.ini");
	}
}

void GameState_Editor::OnLanguageChanged (CfgContents& contents_lang) {
	GameState_BasePlayable::OnLanguageChanged(contents_lang);

	toolBar->OnLanguageChanged(contents_lang);
	levelInfobar->OnLanguageChanged(contents_lang);
	menuBar->OnLanguageChanged(contents_lang);
}

void GameState_Editor::DeleteLevel (std::string levelPath) {
	std::remove(levelPath.c_str());

	unsigned int index = levelPath.find_last_of("/\\");
	std::string lLevelName = levelPath.substr(index + 1, levelPath.length() - index - 5);
	TINI::TINIObject levlist(levelPath.substr(0, index + 1) + "levlist.ini");
	levlist.RemoveEntry("", lLevelName);
//	levlist.SetValue("", lLevelName, "0");

	if (this->levelPath == levelPath) {
		LoadLevel("");
	}
}

void GameState_Editor::OnGameFinished (bool successful) {
	GameState_BasePlayable::OnGameFinished(successful);
}
