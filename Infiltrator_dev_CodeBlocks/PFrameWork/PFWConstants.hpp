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
			0x00E1, // á
			0x00C1, // Á
			0x00E9, // é
			0x00C9, // É
			0x00ED, // í
			0x00CD, // Í
			0x00F3, // ó
			0x00D3, // Ó
			0x00F6, // ö
			0x00D6, // Ö
			0x0151, // õ
			0x0150, // Õ
			0x00FA, // ú
			0x00DA, // Ú
			0x00FC, // ü
			0x00DC, // Ü
			0x0171, // û
			0x0170, // Û
		};
	const unsigned int charset_length = 107;
};

#endif // PFWCONSTANTS_HPP
