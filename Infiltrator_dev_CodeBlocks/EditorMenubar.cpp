#include "EditorMenubar.hpp"

#include "PFrameWork/ConfigParser.hpp"
#include "PFrameWork/SFMLGameManager.hpp"
#include <boost/bind.hpp>
#include "Session.hpp"

EditorMenubar::EditorMenubar (boost::function<void (int)> toolSelected,
							int layerDepth)
		: toolSelected(toolSelected),
		hover(false),
		layerDepth(layerDepth),
		hidden(false) {
	background = new TilingScreenSprite(this, "Content/Textures/GUI/menubarleft.png",
		"Content/Textures/GUI/menubarright.png",
		"Content/Textures/GUI/menubarmiddle.png",
		true,
		ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::TOP, layerDepth);

	SortAscendingScreen();
}

EditorMenubar::EditorMenubar (boost::function<void (int)> toolSelected,
							std::vector<MenuDescription*> toolDescs,
							int selectedID,
							int layerDepth)
		: toolSelected(toolSelected),
		hover(false),
		layerDepth(layerDepth),
		hidden(false) {
	background = new TilingScreenSprite(this, "Content/Textures/GUI/menubarleft.png",
		"Content/Textures/GUI/menubarright.png",
		"Content/Textures/GUI/menubarmiddle.png",
		true,
		ScreenSpaceDrawable::LEFT, ScreenSpaceDrawable::TOP, layerDepth);

	SetTools(toolDescs, selectedID);
}

EditorMenubar::~EditorMenubar () {
	for (std::vector<EditorToolButton*>::iterator itTool = tools.begin();
			itTool != tools.end(); itTool++) {
		delete *itTool;
	}

	for (std::vector<MenuDescription*>::iterator itTool = toolDescs.begin();
			itTool != toolDescs.end(); itTool++) {
		delete *itTool;
	}

	delete background;
}

void EditorMenubar::SetTools (std::vector<MenuDescription*> toolDescs, int selectedID) {
	for (std::vector<EditorToolButton*>::iterator itTool = tools.begin();
			itTool != tools.end(); itTool++) {
		delete *itTool;
	}
	tools.clear();

	for (std::vector<MenuDescription*>::iterator itTool = this->toolDescs.begin();
			itTool != this->toolDescs.end(); itTool++) {
		delete *itTool;
	}
	this->toolDescs.clear();

	this->toolDescs = toolDescs;

	CfgContents contents_lang;
	CfgParser->GetContents(std::string("Content/LanguageFiles/") + CurrentSession->language
		+ std::string(".ini"), contents_lang);

	boost::function<void (int)> event =
		boost::bind(&EditorMenubar::OnToolActivated, this, _1);

	for (std::vector<MenuDescription*>::iterator itToolDesc = this->toolDescs.begin();
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
		tool->SetPosition((*itToolDesc)->offset);

		if ((*itToolDesc)->ID == selectedID) {
			tool->SetActive(true);
			toolSelected(selectedID);
		}

		tools.push_back(tool);
	}

	SortAscendingScreen();
}

void EditorMenubar::HandleSFEvents(std::list<sf::Event>& sfEvents) {
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
			} break;
			case sf::Event::MouseButtonPressed:
				if (hover) {
					mouseOverPressed = true;
				}
			break;
			case sf::Event::MouseWheelMoved:
				if (hover) {
					itSfEvent =	--sfEvents.erase(itSfEvent);
				}
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

void EditorMenubar::OnToolActivated (int ID) {
	if (ID > toolEventID)
		toolEventID = ID;
}

void EditorMenubar::SetActive (int ID) {
	for (std::vector<EditorToolButton*>::iterator itTool = tools.begin();
			itTool != tools.end(); itTool++) {
		(*itTool)->SetActive(ID == (*itTool)->GetID() ? true : false);
	}
}

sf::FloatRect EditorMenubar::GetScreenBounds () {
	return sf::FloatRect(0,
						0,
						Game->GetRenderWindow()->GetWidth(),
						background->GetScreenSize().y);
}

void EditorMenubar::Move (sf::Vector2f offset) {
	background->Move(offset);

	for (std::vector<EditorToolButton*>::iterator itT = tools.begin(); itT != tools.end(); itT++) {
		(*itT)->Move(offset);
	}
}

void EditorMenubar::SetHidden (bool hidden) {
	if (this->hidden == hidden)
		return;

	this->hidden = hidden;
	Move(sf::Vector2f(0, (background->GetScreenSize().y / windowFakeScale) * (hidden ? -1 : 1)));
}

bool EditorMenubar::IsHidden () {
	return hidden;
}

void EditorMenubar::OnLanguageChanged (CfgContents& contents_lang) {
	for (unsigned int i = 0; i != tools.size(); ++i) {
		tools[i]->SetText( contents_lang[toolDescs[i]->langSection][toolDescs[i]->langKey] );
	}
}

