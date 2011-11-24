#ifndef PFWCONSTANTS_HPP
#define PFWCONSTANTS_HPP

#include <SFML/System.hpp>
#include <string>

namespace PFWConstants {
	const sf::Vector2f baseResolution(1024, 768);

	const float fontSize = 11;
	const std::string defaultFontFile = "Content/Fonts/Cardinal/Cardinal.ttf";
	//const std::string defaultFontFile = "Content/Fonts/Apple/AppleGaramond.ttf";
	//const std::string defaultFontFile = "Content/Fonts/Arial.ttf";
	const std::string stringMaxHeight = "WgS";
	const char charNonHanging = 'a';
	const unsigned int baseFontSize = 30; // used for textcursor, + scaling based on difference to this
	const float fontSizeScaleMultiplier = .4; // used for textcursor, + scaling based on difference to this

	const float PI = 3.14159265;
};

#endif // PFWCONSTANTS_HPP
