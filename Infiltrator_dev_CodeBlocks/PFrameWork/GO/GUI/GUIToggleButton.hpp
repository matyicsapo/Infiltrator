#ifndef GUITOGGLEBUTTON_HPP
#define GUITOGGLEBUTTON_HPP

#include "GUIButton.hpp"

class GUIToggleButton : public GUIButton {
protected:
	std::string txActiveNormal;

	boost::function<void (void)> onDeactivate;

public:
	GUIToggleButton (std::string txInactiveNormal,
					std::string txInactiveHover,
					std::string txActiveNormal,
					std::string txActiveHover,
					boost::function<void (void)> onActivate,
					boost::function<void (void)> onDeactivate,
					std::string text = "",
					std::string fontFile = PFWConstants::defaultFontFile,
					float fontSize = PFWConstants::defaultFontSize,
					sf::Vector2f textOffset = sf::Vector2f(0, 0),
					AlignH alignH = LEFT,
					AlignV alignV = TOP,
					int layerDepth = 0)
			: DrawableBase(layerDepth),
				GUIButton(txInactiveNormal,
					txInactiveHover,
					txActiveHover,
					onActivate,
					text,
					fontFile,
					fontSize,
					textOffset,
					alignH,
					alignV,
					layerDepth),
				txActiveNormal(txActiveNormal),
				onDeactivate(onDeactivate)
			{}

	void SetActive (bool activated, bool callevent = false, bool hovertx = false);
	bool GetActive () { return active; }

	virtual sf::Color GetColor () { return sfSprite->GetColor(); }

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual bool Contains (sf::Vector2f globalPosition) {
		return false;
	}
	virtual bool IsOpaque (sf::Vector2f globalPosition) {
		return false;
	}
};

#endif // GUITOGGLEBUTTON_HPP
