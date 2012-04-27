#include "EditorToolbar.hpp"

#include "PFrameWork/ConfigParser.hpp"
#include "PFrameWork/SFMLGameManager.hpp"
#include <boost/bind.hpp>
#include "Session.hpp"

#define WHEELSCROLLSPD 1500.0f

EditorToolbar::EditorToolbar (boost::function<void (int)> toolSelected,
							int layerDepth)
		: toolSelected(toolSelected),
		hover(false),
		toolViewOffsetY(0),
		dragging(false),
		layerDepth(layerDepth),
		hidden(false) {
	background = new TilingScreenSprite(this, "Content/Textures/GUI/toolbartop.png",
		"Content/Textures/GUI/toolbarbottom.png",
		"Content/Textures/GUI/toolbarmiddle.png",
		false,
		ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::TOP, layerDepth);

	SortAscendingScreen();
}

EditorToolbar::EditorToolbar (boost::function<void (int)> toolSelected,
							std::vector<ToolDescription*> toolDescs,
							int selectedID,
							int layerDepth)
		: toolSelected(toolSelected),
		hover(false),
		toolViewOffsetY(0),
		dragging(false),
		layerDepth(layerDepth),
		hidden(false) {
	background = new TilingScreenSprite(this, "Content/Textures/GUI/toolbartop.png",
		"Content/Textures/GUI/toolbarbottom.png",
		"Content/Textures/GUI/toolbarmiddle.png",
		false,
		ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::TOP, layerDepth);

	SetTools(toolDescs, selectedID);
}

EditorToolbar::~EditorToolbar () {
	for (std::vector<EditorToolButton*>::iterator itTool = tools.begin();
			itTool != tools.end(); itTool++) {
		delete *itTool;
	}

	for (std::vector<ToolDescription*>::iterator itTool = toolDescs.begin();
			itTool != toolDescs.end(); itTool++) {
		delete *itTool;
	}

	delete background;
}

void EditorToolbar::SetTools (std::vector<ToolDescription*> toolDescs, int selectedID) {
	for (std::vector<EditorToolButton*>::iterator itTool = tools.begin();
			itTool != tools.end(); itTool++) {
		delete *itTool;
	}
	tools.clear();

	for (std::vector<ToolDescription*>::iterator itTool = this->toolDescs.begin();
			itTool != this->toolDescs.end(); itTool++) {
		delete *itTool;
	}
	this->toolDescs.clear();

	this->toolDescs = toolDescs;

	CfgContents contents_lang;
	CfgParser->GetContents(std::string("Content/LanguageFiles/") + CurrentSession->language
		+ std::string(".ini"), contents_lang);

	boost::function<void (int)> event =
		boost::bind(&EditorToolbar::OnToolActivated, this, _1);

	sf::Vector2f createToolButtonOffset(0, 150);

	for (std::vector<ToolDescription*>::iterator itToolDesc = this->toolDescs.begin();
			itToolDesc != this->toolDescs.end(); itToolDesc++) {

		EditorToolButton* tool = new EditorToolButton(this, (*itToolDesc)->ID,
										(*itToolDesc)->mainTexture,
										"Content/Textures/glow.png",
										event,
										contents_lang[(*itToolDesc)->langSection][(*itToolDesc)->langKey],
										PFWConstants::defaultFontFile,
										24,
										sf::Vector2f(0, 60),
										ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::TOP,
										layerDepth + 2,
										sf::Vector2i(100, 100));

		tool->SetPosition(createToolButtonOffset);
		createToolButtonOffset.y += 140;

		if ((*itToolDesc)->ID == selectedID) {
			tool->SetActive(true);
			toolSelected(selectedID);
			activeID = toolEventID;
		}

		tools.push_back(tool);
	}

	if (hidden) {
		for (std::vector<EditorToolButton*>::iterator itT = tools.begin(); itT != tools.end(); itT++) {
			(*itT)->Move(sf::Vector2f( -background->GetScreenSize().x, 0 ));
		}
	}

	SortAscendingScreen();
}

void EditorToolbar::HandleSFEvents(std::list<sf::Event>& sfEvents) {
	bool mouseOverPressed = false;

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

				if (dragging) {
					float before = toolViewOffsetY;

					toolViewOffsetY += (itSfEvent->MouseMove.Y - lastMousePressedY);

					CalcViewOffsetY();

					lastMousePressedY += toolViewOffsetY - before;

					/* mouse leaving window messes up values on _WINDOWS_
					 * 	so this still does not solve anything :/
					 * */
//					if (!Game->GetRenderWindow()->GetInput().IsMouseButtonDown(sf::Mouse::Left))
//						dragging = false;
				}
			} break;
			case sf::Event::MouseWheelMoved:
				if (hover && !tools.empty()) {
					toolViewOffsetY += Game->GetDeltaTime() * WHEELSCROLLSPD * itSfEvent->MouseWheel.Delta;

					CalcViewOffsetY();
				}
			break;
			case sf::Event::MouseButtonPressed:
				if (hover) {
					if (!tools.empty()) {
						if (itSfEvent->MouseButton.Button == sf::Mouse::Left) {
							lastMousePressedY = itSfEvent->MouseButton.Y;

							dragging = true;
						}
					}

					mouseOverPressed = true;
				}
			break;
			case sf::Event::MouseButtonReleased:
				dragging = false;
			break;
			case sf::Event::Resized:
				CalcViewOffsetY();
			break;
			default:
			break;
		}

		itSfEvent++;
	}

	toolEventID = -2;

	for (std::vector<EditorToolButton*>::iterator itTool = tools.begin();
			itTool != tools.end(); itTool++) {
		(*itTool)->HandleSFEvents(sfEvents);
	}

	if (toolEventID != -2) {
		for (std::vector<EditorToolButton*>::iterator itTool = tools.begin();
				itTool != tools.end(); itTool++) {
			if ((*itTool)->GetID() != toolEventID) {
				(*itTool)->SetActive(false);
			}
		}

		toolSelected(toolEventID);
		activeID = toolEventID;

		if (toolEventID == -1 && mouseOverPressed) {
			// if no tool was activated - the event wasn't eaten
			// but the mouse pressed over the toolbar
			// then eat it still

			itSfEvent = sfEvents.begin();
			while (itSfEvent != sfEvents.end()) {
				switch (itSfEvent->Type) {
					case sf::Event::MouseButtonPressed:
						itSfEvent =	--sfEvents.erase(itSfEvent);
					break;
					default:
					break;
				}

				itSfEvent++;
			}
		}
	}
}

void EditorToolbar::OnToolActivated (int ID) {
	if (ID > toolEventID)
		toolEventID = ID;
}

void EditorToolbar::CalcViewOffsetY () {
	float ySpaceAvailable = Game->GetRenderWindow()->GetHeight() / Game->GetWindowFakeScale() - 150;
	float ySpaceNeeded = (tools.size() - 1) * 140 + 135;

	float minOffset = ySpaceNeeded - ySpaceAvailable;

	if (minOffset < 0) minOffset = 0;

	if (toolViewOffsetY < -minOffset)
		toolViewOffsetY = -minOffset;
	else if (toolViewOffsetY > 0)
		toolViewOffsetY = 0;

	float createToolButtonOffsetY = 150;
	for (std::vector<EditorToolButton*>::iterator itTool = tools.begin();
			itTool != tools.end(); itTool++) {
		(*itTool)->SetPosition(
			sf::Vector2f(0, createToolButtonOffsetY + toolViewOffsetY));

		createToolButtonOffsetY += 140;
	}
}

void EditorToolbar::SetActive (int ID) {
	for (std::vector<EditorToolButton*>::iterator itTool = tools.begin();
			itTool != tools.end(); itTool++) {
		(*itTool)->SetActive(ID == (*itTool)->GetID() ? true : false);
	}
}

int EditorToolbar::GetActive () {
	return activeID;
}

sf::FloatRect EditorToolbar::GetScreenBounds () {
	return sf::FloatRect(0,
						0,
						background->GetScreenSize().x,
						Game->GetRenderWindow()->GetHeight());
}

void EditorToolbar::Move (sf::Vector2f offset) {
	background->Move(offset);

	for (std::vector<EditorToolButton*>::iterator itT = tools.begin(); itT != tools.end(); itT++) {
		(*itT)->Move(offset);
	}
}

void EditorToolbar::SetHidden (bool hidden) {
	if (this->hidden == hidden)
		return;

	this->hidden = hidden;
	Move(sf::Vector2f((background->GetScreenSize().x / windowFakeScale) * (hidden ? -1 : 1), 0));
}

bool EditorToolbar::IsHidden () {
	return hidden;
}

void EditorToolbar::OnLanguageChanged (CfgContents& contents_lang) {
	for (unsigned int i = 0; i != tools.size(); ++i) {
		tools[i]->SetText( contents_lang[toolDescs[i]->langSection][toolDescs[i]->langKey] );
	}
}

