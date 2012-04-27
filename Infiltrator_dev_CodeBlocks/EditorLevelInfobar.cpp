#include "EditorLevelInfobar.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "PFrameWork/GO/GUI/GUITextfield.hpp"
#include "PFrameWork/GO/ScreenString.hpp"
#include "TilingScreenSprite.hpp"
#include "PFrameWork/PFWConstants.hpp"
#include "PFrameWork/SFMLGameManager.hpp"
#include "VisualItemSlot.hpp"
#include "MyMagicNumbers.hpp"
#include "PFrameWork/ConfigParser.hpp"
#include "PFrameWork/GameObjectManager.hpp"
#include <boost/bind.hpp>
#include "Session.hpp"
#include "TilingButton.hpp"
#include "PFrameWork/GO/GUI/GUIToggleButton.hpp"
#include "FullScreenEventCatcherTint.hpp"
//#include <iostream>

EditorLevelInfobar::EditorLevelInfobar (int layerDepth)
		: objectiveDragee(0), objectiveID(0), hover(false), layerDepth(layerDepth), hidden(false),
		navMapState(GameState_Editor::NONE) {
	background = new TilingScreenSprite(this, "Content/Textures/GUI/infobartop.png",
		"Content/Textures/GUI/infobarbottom.png",
		"Content/Textures/GUI/infobarmiddle.png",
		false,
		ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth);

	CfgContents contents_lang;
	CfgParser->GetContents(std::string("Content/LanguageFiles/") + CurrentSession->language
		+ std::string(".ini"), contents_lang);

	levelNameLabel = new ScreenString(contents_lang["editor_levelinfo"]["name"],
									PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(levelNameLabel);
	levelNameLabel->SetPosition(sf::Vector2f(-290 + levelNameLabel->GetScreenSize().x, 155));

	GameState_Editor* editor = dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top());

	boost::function<void (void)> onApplyLevelName = boost::bind(&GameState_Editor::UpdateLevelName, editor);
	std::string regexPatternLevelName = "[0-9a-zA-Z]*";
	levelNameTextField = new GUITextfield(this, "Content/Textures/GUI/levelNameTextField_normal.png",
										"Content/Textures/GUI/levelNameTextField_hover.png",
										sf::Color(128, 128, 128),
										sf::Color::White,
										MyMagicNumbers::untitledLevelName,
										regexPatternLevelName,
										onApplyLevelName,
										PFWConstants::defaultFontFile,
										PFWConstants::defaultFontSize,
										210,
										ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP,
										layerDepth + 10);
	levelNameTextField->SetPosition(sf::Vector2f(100, 195));

	labelLevelSize = new ScreenString(contents_lang["editor_levelinfo"]["size"],
									PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(labelLevelSize);
	labelLevelSize->SetPosition(sf::Vector2f(-290 + labelLevelSize->GetScreenSize().x, 245));
	labelLevelSizeX = new ScreenString("X:", PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(labelLevelSizeX);
	labelLevelSizeX->SetPosition(sf::Vector2f(-275 + labelLevelSizeX->GetScreenSize().x, 290));
	labelLevelSizeY = new ScreenString("Y:", PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(labelLevelSizeY);
	labelLevelSizeY->SetPosition(sf::Vector2f(-275 + labelLevelSizeY->GetScreenSize().x, 340));

	boost::function<void (void)> onApplyLevelSize = boost::bind(&EditorLevelInfobar::ApplyLevelSize, this);
//	std::string regexPatternLevelSize = "(^[1-4][0-9]*$)|(^5[0]*$)";
//	std::string regexPatternLevelSize = "(^[1-5][0-9]*)|(^[5][0][0][0]){3,4}";
	std::string regexPatternLevelSize = "[0-9]*";
	levelSizeXTextField = new GUITextfield(this, "Content/Textures/GUI/levelSizeTextField_normal.png",
										"Content/Textures/GUI/levelSizeTextField_hover.png",
										sf::Color(128, 128, 128),
										sf::Color::White,
										Convert->ToString<float>(editor->GetLevelSize().x),
										regexPatternLevelSize,
										onApplyLevelSize,
										PFWConstants::defaultFontFile,
										PFWConstants::defaultFontSize,
										40,
										ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP,
										layerDepth + 10);
	levelSizeXTextField->SetPosition(sf::Vector2f(-90, 285));

	levelSizeYTextField = new GUITextfield(this, "Content/Textures/GUI/levelSizeTextField_normal.png",
										"Content/Textures/GUI/levelSizeTextField_hover.png",
										sf::Color(128, 128, 128),
										sf::Color::White,
										Convert->ToString<float>(editor->GetLevelSize().y),
										regexPatternLevelSize,
										onApplyLevelSize,
										PFWConstants::defaultFontFile,
										PFWConstants::defaultFontSize,
										40,
										ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP,
										layerDepth + 10);
	levelSizeYTextField->SetPosition(sf::Vector2f(-90, 335));

	labelObjectives = new ScreenString(contents_lang["editor_levelinfo"]["objectives"],
									PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(labelObjectives);
	labelObjectives->SetPosition(sf::Vector2f(-290 + labelObjectives->GetScreenSize().x, 385));

	label1stObjective = new ScreenString("1:",
									PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(label1stObjective);
	label1stObjective->SetPosition(sf::Vector2f(-275 + label1stObjective->GetScreenSize().x, 425));
	slot1stObjective = new VisualItemSlot(this, "Content/Textures/GUI/toggle_inactive_normal.png",
										"Content/Textures/GUI/toggle_inactive_hover.png",
										ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP,
										layerDepth + 10);
	slot1stObjective->SetPosition(sf::Vector2f(-240 + slot1stObjective->GetScreenSize().x, 425));

	label2ndObjective = new ScreenString("2:",
									PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(label2ndObjective);
	label2ndObjective->SetPosition(sf::Vector2f(-130 + label2ndObjective->GetScreenSize().x, 425));
	slot2ndObjective = new VisualItemSlot(this, "Content/Textures/GUI/toggle_inactive_normal.png",
										"Content/Textures/GUI/toggle_inactive_hover.png",
										ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP,
										layerDepth + 10);
	slot2ndObjective->SetPosition(sf::Vector2f(-95 + slot2ndObjective->GetScreenSize().x, 425));

	labelNavMap = new ScreenString(contents_lang["editor_levelinfo"]["navmap"],
									PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(labelNavMap);
	labelNavMap->SetPosition(sf::Vector2f(-290 + labelNavMap->GetScreenSize().x, 530));

	labelNavMapState = new ScreenString(contents_lang["editor_levelinfo"]["navmap_none"],
									PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(labelNavMapState);
	labelNavMapState->SetPosition(sf::Vector2f(-270 + labelNavMap->GetScreenSize().x / windowFakeScale
												+ labelNavMapState->GetScreenSize().x / windowFakeScale, 530));

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

	boost::function<void (void)> event = boost::bind(&GameState_Editor::InvokeGenerateNavMap, editor);
	generateNavMapBtn = new TilingButton(this, txInactiveNormal, txInactiveHover, txActiveHover,
							event,
							contents_lang["editor_levelinfo"]["gennavmap"],
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							-1,
							ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP,
							layerDepth + 10);
	generateNavMapBtn->SetPosition(sf::Vector2f(-275 + generateNavMapBtn->GetScreenSize().x, 605));

	labelToggleShowNavMap = new ScreenString(contents_lang["editor_levelinfo"]["shownavmap"],
									PFWConstants::defaultFontFile, PFWConstants::defaultFontSize,
									ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP, layerDepth + 10);
	AddScreenSpace(labelToggleShowNavMap);
	labelToggleShowNavMap->SetPosition(sf::Vector2f(-275 + labelToggleShowNavMap->GetScreenSize().x, 700));

	boost::function <void (void)> activate = boost::bind(&GameState_Editor::ShowNavMap, editor, true);
	boost::function <void (void)> deactivate = boost::bind(&GameState_Editor::ShowNavMap, editor, false);
	toggleShowNavMap = new GUIToggleButton("Content/Textures/GUI/toggle_inactive_normal.png",
							"Content/Textures/GUI/toggle_inactive_hover.png",
							"Content/Textures/GUI/toggle_active_normal.png",
							"Content/Textures/GUI/toggle_active_hover.png",
							activate,
							deactivate,
							"",
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP,
							layerDepth + 10);
	AddScreenSpace(toggleShowNavMap);
	toggleShowNavMap->SetPosition(sf::Vector2f(-240 + labelToggleShowNavMap->GetScreenSize().x
												+ toggleShowNavMap->GetScreenSize().x, 685));


	event = boost::bind(&EditorLevelInfobar::ToggleHidden, this);
	hideBtn = new GUIButton("Content/Textures/GUI/darkbarhidearrow.png",
							"Content/Textures/GUI/barhidearrow.png",
							"Content/Textures/GUI/barhidearrow.png",
							event,
							"",
							PFWConstants::defaultFontFile,
							PFWConstants::defaultFontSize,
							sf::Vector2f(0, 0),
							ScreenSpaceDrawable::RIGHT, ScreenSpaceDrawable::TOP,
							layerDepth);
	AddScreenSpace(hideBtn);
	hideBtn->SetPosition(sf::Vector2f(-background->GetScreenSize().x, (PFWConstants::baseResolution.y - 150) / 2 ));

	fullScrTint = 0;

	SortAscendingScreen();
}

EditorLevelInfobar::~EditorLevelInfobar () {
	ClearAll();

	delete levelNameLabel;
	delete levelNameTextField;

	delete labelLevelSize;
	delete labelLevelSizeX;
	delete labelLevelSizeY;
	delete levelSizeXTextField;
	delete levelSizeYTextField;

	delete labelObjectives;
	delete label1stObjective;
	delete label2ndObjective;
	delete slot1stObjective;
	delete slot2ndObjective;

	delete labelNavMap;
	delete labelNavMapState;
	delete generateNavMapBtn;
	delete labelToggleShowNavMap;
	delete toggleShowNavMap;

	delete hideBtn;

	delete background;

	delete fullScrTint;

	delete objectiveDragee;
}

void EditorLevelInfobar::Update (float dT) {
	levelNameTextField->Update(dT);
	levelSizeXTextField->Update(dT);
	levelSizeYTextField->Update(dT);

	if (objectiveDragee == 0)
		return;

	const sf::Input& sfInput = Game->GetRenderWindow()->GetInput();

	objectiveDragee->SetPosition(sf::Vector2f(sfInput.GetMouseX(), sfInput.GetMouseY()) / windowFakeScale);
}

void EditorLevelInfobar::HandleSFEvents (std::list<sf::Event>& sfEvents) {
	hideBtn->HandleSFEvents(sfEvents);

	levelNameTextField->HandleSFEvents(sfEvents);

	levelSizeXTextField->HandleSFEvents(sfEvents);
	levelSizeYTextField->HandleSFEvents(sfEvents);

	slot1stObjective->HandleSFEvents(sfEvents);
	slot2ndObjective->HandleSFEvents(sfEvents);

	generateNavMapBtn->HandleSFEvents(sfEvents);
	toggleShowNavMap->HandleSFEvents(sfEvents);

	std::list<sf::Event>::iterator itSfEvent = sfEvents.begin();
	while (itSfEvent != sfEvents.end()) {
		switch (itSfEvent->Type) {
			case sf::Event::MouseMoved: {
				sf::Vector2f screenSize(background->GetScreenSize());
				sf::Vector2f fakePos(background->CalcFakePos());
				sf::Rect<float> bounds(fakePos.x - (screenSize.x / 2),
										fakePos.y - (screenSize.y / 2),
										fakePos.x + (screenSize.x / 2),
										fakePos.y + (screenSize.y / 2 ));

				hover = bounds.Contains(itSfEvent->MouseMove.X, itSfEvent->MouseMove.Y) ? true : false;
			} break;
			case sf::Event::MouseButtonReleased:
				if (!objectiveDragee)
					break;

				if (slot1stObjective->Contains(
						sf::Vector2f(itSfEvent->MouseButton.X, itSfEvent->MouseButton.Y))) {
					ScreenSprite* slot1stContained = new ScreenSprite(*objectiveDragee);
					slot1stObjective->SetContained(slot1stContained,
													objectiveDragee->entityType, objectiveID);

					dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top())->SetObjective(
						0,
						dynamic_cast<BaseObjective*>(
							GameObjects->GetEntityMap(objectiveDragee->entityType)[objectiveID]));
				}

				if (slot2ndObjective->Contains(
						sf::Vector2f(itSfEvent->MouseButton.X, itSfEvent->MouseButton.Y))) {
					ScreenSprite* slot2ndContained = new ScreenSprite(*objectiveDragee);
					slot2ndObjective->SetContained(slot2ndContained,
													objectiveDragee->entityType, objectiveID);

					dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top())->SetObjective(
						1,
						dynamic_cast<BaseObjective*>(
							GameObjects->GetEntityMap(objectiveDragee->entityType)[objectiveID]));
				}
			break;
			case sf::Event::MouseButtonPressed:
				if (slot1stObjective->Contains(sf::Vector2f(itSfEvent->MouseButton.X, itSfEvent->MouseButton.Y))) {
					unsigned int typeID = 0;
					unsigned int ID = 0;
					if (slot1stObjective->GetContained(typeID, ID)) {
						switch (typeID) {
							case ID_CHEST:
								objectiveDragee = new ScreenSprite(TEXTURE_CHEST, layerDepth + 20, ID_CHEST);
								AddScreenSpace(objectiveDragee);
								objectiveDragee->SetScale(.6f);

								SortAscendingScreen();
							break;
							case ID_PRINCESS:
								objectiveDragee = new ScreenSprite(TEXTURE_PRINCESS, layerDepth + 20, ID_PRINCESS);
								AddScreenSpace(objectiveDragee);
								objectiveDragee->SetScale(.5f);

								SortAscendingScreen();
							break;
							case ID_GOAL:
								objectiveDragee = new ScreenSprite(TEXTURE_GOAL, layerDepth + 20, ID_GOAL);
								AddScreenSpace(objectiveDragee);
								objectiveDragee->SetScale(.5f);

								SortAscendingScreen();
							break;
						}

						objectiveID = ID;

						slot1stObjective->SetContained(0, 0, 0);
						dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top())->SetObjective(0, 0);
					}
				}
				else
				if (slot2ndObjective->Contains(sf::Vector2f(itSfEvent->MouseButton.X, itSfEvent->MouseButton.Y))) {
					unsigned int typeID = 0;
					unsigned int ID = 0;
					if (slot2ndObjective->GetContained(typeID, ID)) {
						switch (typeID) {
							case ID_CHEST:
								objectiveDragee = new ScreenSprite(TEXTURE_CHEST, layerDepth + 20, ID_CHEST);
								AddScreenSpace(objectiveDragee);
								objectiveDragee->SetScale(.6f);

								SortAscendingScreen();
							break;
							case ID_PRINCESS:
								objectiveDragee = new ScreenSprite(TEXTURE_PRINCESS, layerDepth + 20, ID_PRINCESS);
								AddScreenSpace(objectiveDragee);
								objectiveDragee->SetScale(.5f);

								SortAscendingScreen();
							break;
							case ID_GOAL:
								objectiveDragee = new ScreenSprite(TEXTURE_GOAL, layerDepth + 20, ID_GOAL);
								AddScreenSpace(objectiveDragee);
								objectiveDragee->SetScale(.5f);

								SortAscendingScreen();
							break;
						}

						objectiveID = ID;

						slot2ndObjective->SetContained(0, 0, 0);
						dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top())->SetObjective(1, 0);
					}
				}

				if (hover) {
					itSfEvent =	--sfEvents.erase(itSfEvent);
				}
			break;
			default:
			break;
		}

		itSfEvent++;
	}
}

std::string EditorLevelInfobar::GetLevelName () {
	return levelNameTextField->GetText();
}

void EditorLevelInfobar::SetLevelName (std::string levelName) {
	levelNameTextField->SetText(levelName);
}

sf::FloatRect EditorLevelInfobar::GetScreenBounds () {
	if (hidden)
		return sf::FloatRect(Game->GetRenderWindow()->GetWidth(),
						0,
						Game->GetRenderWindow()->GetWidth() + background->GetScreenSize().x,
						Game->GetRenderWindow()->GetHeight());

	return sf::FloatRect(Game->GetRenderWindow()->GetWidth() - background->GetScreenSize().x,
						0,
						Game->GetRenderWindow()->GetWidth(),
						Game->GetRenderWindow()->GetHeight());
}

void EditorLevelInfobar::SetObjectiveDragee (unsigned int objectiveType, unsigned int ID, sf::Vector2f pos) {
	switch (objectiveType) {
		case ID_CHEST:
			if (objectiveDragee != 0)
				break;

			objectiveDragee = new ScreenSprite(TEXTURE_CHEST, layerDepth + 20, ID_CHEST);
			AddScreenSpace(objectiveDragee);
			objectiveDragee->SetScale(.6f);

			objectiveID = ID;

			SortAscendingScreen();
		break;
		case ID_PRINCESS:
			if (objectiveDragee != 0)
				break;

			objectiveDragee = new ScreenSprite(TEXTURE_PRINCESS, layerDepth + 20, ID_PRINCESS);
			AddScreenSpace(objectiveDragee);
			objectiveDragee->SetScale(.5f);

			objectiveID = ID;

			SortAscendingScreen();
		break;
		case ID_GOAL:
			if (objectiveDragee != 0)
				break;

			objectiveDragee = new ScreenSprite(TEXTURE_GOAL, layerDepth + 20, ID_GOAL);
			AddScreenSpace(objectiveDragee);
			objectiveDragee->SetScale(.5f);

			objectiveID = ID;

			SortAscendingScreen();
		break;
		default:
			const sf::Input& sfInput = Game->GetRenderWindow()->GetInput();
			if (sfInput.IsMouseButtonDown(sf::Mouse::Left))
				break;

			PopScreenSpace(objectiveDragee);
			delete objectiveDragee;
			objectiveDragee = 0;
		break;
	}
}

void EditorLevelInfobar::Move (sf::Vector2f offset) {
	labelObjectives->Move(offset);
	label1stObjective->Move(offset);
	label2ndObjective->Move(offset);

	slot1stObjective->Move(offset);
	slot2ndObjective->Move(offset);

	background->Move(offset);

	levelNameLabel->Move(offset);
	levelNameTextField->Move(offset);

	labelLevelSize->Move(offset);
	labelLevelSizeX->Move(offset);
	labelLevelSizeY->Move(offset);
	levelSizeXTextField->Move(offset);
	levelSizeYTextField->Move(offset);

	labelNavMap->Move(offset);
	labelNavMapState->Move(offset);
	generateNavMapBtn->Move(offset);
	labelToggleShowNavMap->Move(offset);
	toggleShowNavMap->Move(offset);

	hideBtn->Move(offset);
}

void EditorLevelInfobar::SetHidden (bool hidden) {
	if (this->hidden == hidden)
		return;

	this->hidden = hidden;
	Move(sf::Vector2f((background->GetScreenSize().x / windowFakeScale) * (hidden ? 1 : -1), 0));

	if (hidden) {
		levelNameTextField->SetActive(false);
		levelSizeXTextField->SetActive(false);
		levelSizeYTextField->SetActive(false);
	}
}

bool EditorLevelInfobar::IsHidden () {
	return hidden;
}

void EditorLevelInfobar::ToggleHidden () {
	hidden = !hidden;

	Move(sf::Vector2f((background->GetScreenSize().x / windowFakeScale) * (hidden ? 1 : -1), 0));

	if (hidden) {
		levelNameTextField->SetActive(false);
		levelSizeXTextField->SetActive(false);
		levelSizeYTextField->SetActive(false);
	}
}

void EditorLevelInfobar::ApplyLevelSize () {
	sf::Vector2f lLevelSize;

	std::string
	text = levelSizeXTextField->GetText();
	if (text != "") {
		lLevelSize.x = Convert->ToNum<float>(text);
		if (lLevelSize.x > MyMagicNumbers::maxLevelSize.x) {
			lLevelSize.x = MyMagicNumbers::maxLevelSize.x;
		}
		else
		if (lLevelSize.x < MyMagicNumbers::minLevelSize.x) {
			lLevelSize.x = MyMagicNumbers::minLevelSize.x;
		}
	}
	else lLevelSize.x = MyMagicNumbers::defaultLevelSize.x;

	text = levelSizeYTextField->GetText();
	if (text != "") {
		lLevelSize.y = Convert->ToNum<float>(text);
		if (lLevelSize.y > MyMagicNumbers::maxLevelSize.y) {
			lLevelSize.y = MyMagicNumbers::maxLevelSize.y;
		}
		else
		if (lLevelSize.y < MyMagicNumbers::minLevelSize.y) {
			lLevelSize.y = MyMagicNumbers::minLevelSize.y;
		}
	}
	else lLevelSize.y = MyMagicNumbers::defaultLevelSize.y;

	dynamic_cast<GameState_Editor*>(Game->GetGameStateMachine()->Top())->SetLevelSize(lLevelSize);
}

void EditorLevelInfobar::OnLanguageChanged (CfgContents& contents_lang) {
	levelNameLabel->SetText(contents_lang["editor_levelinfo"]["name"]);
	levelNameLabel->SetPosition(sf::Vector2f(-290 + levelNameLabel->GetScreenSize().x, levelNameLabel->GetPosition().y));

	labelLevelSize->SetText(contents_lang["editor_levelinfo"]["size"]);
	labelLevelSize->SetPosition(sf::Vector2f(-290 + labelLevelSize->GetScreenSize().x, labelLevelSize->GetPosition().y));

	labelObjectives->SetText(contents_lang["editor_levelinfo"]["objectives"]);
	labelObjectives->SetPosition(sf::Vector2f(-290 + labelObjectives->GetScreenSize().x, labelObjectives->GetPosition().y));

	labelNavMap->SetText(contents_lang["editor_levelinfo"]["navmap"]);
	labelNavMap->SetPosition(sf::Vector2f(-290 + labelNavMap->GetScreenSize().x, labelNavMap->GetPosition().y));

	labelNavMapState->SetText( contents_lang["editor_levelinfo"]
		[navMapState == GameState_Editor::NONE ? "navmap_none"
			: (navMapState == GameState_Editor::VALID ? "navmap_got" : "navmap_changed")] );
	labelNavMapState->SetPosition(sf::Vector2f(-290 + labelNavMap->GetScreenSize().x
												+ labelNavMapState->GetScreenSize().x + 20, 530));

	generateNavMapBtn->SetText(contents_lang["editor_levelinfo"]["gennavmap"]);
	generateNavMapBtn->SetPosition(sf::Vector2f(-287 + generateNavMapBtn->GetScreenSize().x, 605));


	labelToggleShowNavMap->SetText(contents_lang["editor_levelinfo"]["shownavmap"]);
	labelToggleShowNavMap->SetPosition(sf::Vector2f(-275 + labelToggleShowNavMap->GetScreenSize().x, 700));

	if (hidden) {
		sf::Vector2f offset(background->GetScreenSize().x, 0);

		levelNameLabel->Move(offset);
		labelLevelSize->Move(offset);
		labelObjectives->Move(offset);
		labelNavMap->Move(offset);
		labelNavMapState->Move(offset);
		generateNavMapBtn->Move(offset);
		labelToggleShowNavMap->Move(offset);
	}
}

void EditorLevelInfobar::SetTextLevelSize (sf::Vector2f newLevelSize) {
	levelSizeXTextField->SetText(Convert->ToString<float>(newLevelSize.x));
	levelSizeYTextField->SetText(Convert->ToString<float>(newLevelSize.y));
}

sf::Vector2f EditorLevelInfobar::GetLevelSize () {
	return sf::Vector2f(Convert->ToNum<float>(levelSizeYTextField->GetText()),
						Convert->ToNum<float>(levelSizeYTextField->GetText()));
}

void EditorLevelInfobar::ResetGUIElements () {
	levelNameTextField->SetActive(false);
	levelSizeXTextField->SetActive(false);
	levelSizeYTextField->SetActive(false);
}

bool EditorLevelInfobar::IsAnyTextFieldActive () {
	return levelNameTextField->GetActive()
		|| levelSizeXTextField->GetActive()
		|| levelSizeYTextField->GetActive();
}

void EditorLevelInfobar::SetNavMapState (GameState_Editor::NavMapState navMapState) {
	this->navMapState = navMapState;

	CfgContents contents_lang;
	CfgParser->GetContents(std::string("Content/LanguageFiles/") + CurrentSession->language
		+ std::string(".ini"), contents_lang);

	labelNavMapState->SetText( contents_lang["editor_levelinfo"]
		[navMapState == GameState_Editor::NONE ? "navmap_none"
			: (navMapState == GameState_Editor::VALID ? "navmap_got" : "navmap_changed")] );
	labelNavMapState->SetPosition(sf::Vector2f(-270 + labelNavMap->GetScreenSize().x / windowFakeScale
												+ labelNavMapState->GetScreenSize().x / windowFakeScale, 530));
	if (hidden) labelNavMapState->Move(sf::Vector2f(background->GetScreenSize().x, 0));

	labelNavMapState->SetColor( navMapState == GameState_Editor::NONE ? sf::Color::White
			: (navMapState == GameState_Editor::VALID ? sf::Color::Green : sf::Color::Red));
}

bool EditorLevelInfobar::IsShowNavMap () {
	return toggleShowNavMap->GetActive();
}
