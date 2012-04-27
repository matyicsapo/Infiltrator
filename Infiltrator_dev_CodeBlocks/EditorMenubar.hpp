#ifndef EDITORMENUBAR_HPP
#define EDITORMENUBAR_HPP

#include "PFrameWork/SFMLEventHandler.hpp"

#include "EditorToolButton.hpp"
#include "TilingScreenSprite.hpp"
#include <vector>
#include <string>
#include <boost/function.hpp>
#include "PFrameWork/DrawManager.hpp"
#include "PFrameWork/ConfigParser.hpp"

struct MenuDescription {
	MenuDescription (int ID
					, std::string mainTexture
					, std::string langSection
					, std::string langKey
					, sf::Vector2f offset
					)
		: ID(ID)
		, mainTexture(mainTexture)
		, langSection(langSection)
		, langKey(langKey)
		, offset(offset)
		{}

	int ID;
	std::string mainTexture;
	std::string langSection;
	std::string langKey;
	sf::Vector2f offset;
};

class EditorMenubar : public GameObject, public SFMLEventHandler, public DrawManager {
protected:
	std::vector<MenuDescription*> toolDescs;

	short toolEventID;
	boost::function<void (int)> toolSelected;

	bool hover;

	int layerDepth;

	TilingScreenSprite* background;
	std::vector<EditorToolButton*> tools;

	bool hidden;

	void OnToolActivated (int ID);

	virtual sf::Vector2f CalcFakePos () { return sf::Vector2f(0, 0); }

	virtual void SetFakeScale (sf::Vector2f fakeScale) {}
	virtual void SetFakePos (sf::Vector2f fakePos) {}

public:
	EditorMenubar (boost::function<void (int)> toolSelected
					, int layerDepth = 0);
	EditorMenubar (boost::function<void (int)> toolSelected
					, std::vector<MenuDescription*> toolDescs
					, int selectedID
					, int layerDepth = 0);

	~EditorMenubar ();

	void SetTools (std::vector<MenuDescription*> toolDescs,	int selectedID);
	void SetActive (int ID);

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

#endif // EDITORMENUBAR_HPP
