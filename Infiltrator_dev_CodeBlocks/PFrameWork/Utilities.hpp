#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <SFML/System/Vector2.hpp>

class Utilities {
private:
	Utilities () {}

	Utilities (Utilities const&);
	Utilities& operator= (Utilities const&);

public:
	static Utilities* Instance ();

	void Normalize (sf::Vector2f& v);
	sf::Vector2f Normalized (sf::Vector2f v);
	float Length (const sf::Vector2f& v);

	void SetLength (sf::Vector2f& v, float length);
	void ClampLength (sf::Vector2f& v, float maxLength);

	float Dst (const sf::Vector2f& p1, const sf::Vector2f& p2);
	float Dot (const sf::Vector2f& v1, const sf::Vector2f& v2);

	void RotateByDegrees (sf::Vector2f& v, float angle);

	float AngleBetweenInRadians (const sf::Vector2f& dir1, const sf::Vector2f& dir2);
	float AngleBetweenInDegrees (const sf::Vector2f& dir1, const sf::Vector2f& dir2);

	char Sign (float n, bool zeroIsPositive = true);
};

#define Utils Utilities::Instance()

#endif // UTILITIES_HPP
