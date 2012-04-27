#ifndef VISUALITEMSLOT_HPP
#define VISUALITEMSLOT_HPP

#include "PFrameWork/GO/ScreenSprite.hpp"
#include "PFrameWork/GO/Sprite.hpp"
#include "PFrameWork/SFMLEventHandler.hpp"

class ScreenSprite;

class VisualItemSlot : public ScreenSprite, public SFMLEventHandler {
protected:
	DrawManager* const drawManager;

	bool hover;

	std::string txNormal;
	std::string txHover;

	ScreenSprite* containedSprite;
	unsigned int containedTypeID;
	unsigned int containedID;

public:
	VisualItemSlot (DrawManager* drawManager,
		std::string txNormal,
		std::string txHover,
		AlignH alignH = LEFT,
		AlignV alignV = TOP,
		int layerDepth = 0);

	~VisualItemSlot ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void ReleaseResources ();

	virtual void Move (sf::Vector2f offset) {
		ScreenSprite::Move(offset);
		if (containedSprite)  {
			containedSprite->Move(offset);
		}
	}

	void SetContained(ScreenSprite* sprite, unsigned int typeID, unsigned int ID);
	bool GetContained (unsigned int& typeID, unsigned int& ID) const;
};

#endif // VISUALITEMSLOT_HPP
