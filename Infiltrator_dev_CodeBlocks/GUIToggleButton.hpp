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
					std::string fontFile = MConst::defaultFontFile,
					float fontSize = MConst::fontSize,
					AlignH alignH = LEFT,
					AlignV alignV = TOP,
					float layerDepth = 0)
			: DrawableBase(layerDepth),
				GUIButton(txInactiveNormal,
					txInactiveHover,
					txActiveHover,
					onActivate,
					text,
					fontFile,
					fontSize,
					alignH,
					alignV,
					layerDepth),
				txActiveNormal(txActiveNormal),
				onDeactivate(onDeactivate)
			{}

	bool GetActive () { return active; }

	virtual void HandleEvents(std::list<sf::Event>& sfEvents);
};

#endif // GUITOGGLEBUTTON_HPP
