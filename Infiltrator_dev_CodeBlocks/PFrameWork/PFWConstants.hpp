#ifndef PFWCONSTANTS_HPP
#define PFWCONSTANTS_HPP

#include <SFML/System.hpp>
#include <string>

namespace PFWConstants {
	const sf::Vector2f baseResolution(1024, 768);

	const float defaultFontSize = 30;
	const std::string defaultFontFile = "Content/Fonts/Arial.ttf";
	const std::string stringMaxHeight = "WgS";
	const char charNonHanging = 'a';
	const unsigned int baseFontSize = 30; // used for textcursor, + scaling based on difference to this
	const float fontSizeScaleMultiplier = .4; // used for textcursor, + scaling based on difference to this

	const float PI = 3.14159265;

	const sf::Uint32 charset[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
			'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'v', 'w', 'u', 'x', 'y', 'z',
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
			'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'U', 'X', 'Y', 'Z',
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			' ', '.', ':', ',', ';', '(', '*', '!', '?', '\'', ')',
			'-', '+', '/', '\\', '_', '=', '"', '%', '~', '@',
			0x00E1, // �
			0x00C1, // �
			0x00E9, // �
			0x00C9, // �
			0x00ED, // �
			0x00CD, // �
			0x00F3, // �
			0x00D3, // �
			0x00F6, // �
			0x00D6, // �
			0x0151, // �
			0x0150, // �
			0x00FA, // �
			0x00DA, // �
			0x00FC, // �
			0x00DC, // �
			0x0171, // �
			0x0170, // �
		};
	const unsigned int charset_length = 107;
};

#endif // PFWCONSTANTS_HPP
