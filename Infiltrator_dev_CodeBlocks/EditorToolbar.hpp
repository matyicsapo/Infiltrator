#ifndef EDITORTOOLBAR_HPP
#define EDITORTOOLBAR_HPP

#include "PFrameWork/SFMLEventHandler.hpp"

#include "EditorToolButton.hpp"
#include "TilingScreenSprite.hpp"
#include <vector>
#include <string>
#include <boost/function.hpp>
#include "PFrameWork/DrawManager.hpp"
#include "PFrameWork/ConfigParser.hpp"

struct ToolDescription {
	ToolDescription (int ID
					, std::string mainTexture
					, std::string langSection
					, std::string langKey
					)
		: ID(ID)
		, mainTexture(mainTexture)
		, langSection(langSection)
		, langKey(langKey)
		{}

	int ID;
	std::string mainTexture;
	std::string langSection;
	std::string langKey;
};

class EditorToolbar : public GameObject, public SFMLEventHandler, public DrawManager {
protected:
	std::vector<ToolDescription*> toolDescs;

	short toolEventID;
	boost::function<void (int)> toolSelected;

	int activeID;

	void CalcViewOffsetY ();

	bool hover;

	float toolViewOffsetY;

	float lastMousePressedY;
	bool dragging;

	int layerDepth;

	TilingScreenSprite* background;
	std::vector<EditorToolButton*> tools;

	bool hidden;

	void OnToolActivated (int ID);

	virtual sf::Vector2f CalcFakePos () { return sf::Vector2f(0, 0); }

	virtual void SetFakeScale (sf::Vector2f fakeScale) {}
	virtual void SetFakePos (sf::Vector2f fakePos) {}

public:
	EditorToolbar (boost::function<void (int)> toolSelected
					, int layerDepth = 0);
	EditorToolbar (boost::function<void (int)> toolSelected
					, std::vector<ToolDescription*> toolDescs
					, int selectedID
					, int layerDepth = 0);

	~EditorToolbar ();

	void SetTools (std::vector<ToolDescription*> toolDescs,	int selectedID);
	void SetActive (int ID);
	int GetActive ();

	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents);

	virtual void OnLanguageChanged (CfgContents& contents_lang);

	sf::FloatRect GetScreenBounds ();

	virtual void Move (sf::Vector2f offset);

	void SetHidden (bool hidden);
	bool IsHidden ();

	virtual void SetRotation (float rotation) {}
	virtual float GetRotation () { return 0; }
	virtual void Rotate (float angle) {}
};

#endif // EDITORTOOLBAR_HPP
