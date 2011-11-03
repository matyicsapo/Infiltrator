#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <SFML/System.hpp>

class Utilities {
private:
	Utilities () {}

	Utilities (Utilities const&);
	Utilities& operator= (Utilities const&);

public:
	static Utilities* Instance ();

	void Normalize (sf::Vector2f& v);
	sf::Vector2f Normalized (sf::Vector2f v);
	float Length (sf::Vector2f v);
};

#endif // UTILITIES_HPP
