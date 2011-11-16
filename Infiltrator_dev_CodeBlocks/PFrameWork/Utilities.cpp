#include "Utilities.hpp"

#include <cmath>

#include "PFWConstants.hpp"

Utilities* Utilities::Instance () {
	static Utilities instance;

	return &instance;
}

void Utilities::Normalize (sf::Vector2f& v) {
	v = v / Length(v);
}

sf::Vector2f Utilities::Normalized (sf::Vector2f v) {
	return v / Length(v);
}

float Utilities::Length (const sf::Vector2f& v) {
	return float(sqrt((v.x * v.x) + (v.y * v.y)));
}

void Utilities::SetLength (sf::Vector2f& v, float length) {
	v = Normalized(v) * length;
}

void Utilities::ClampLength (sf::Vector2f& v, float maxLength) {
	if (Length(v) > maxLength) {
		SetLength(v, maxLength);
	}
}

float Utilities::Dst (const sf::Vector2f& p1, const sf::Vector2f& p2) {
	return sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) );
}

float Utilities::Dot (const sf::Vector2f& v1, const sf::Vector2f& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

void Utilities::RotateByDegrees (sf::Vector2f& v, float angle) {
	float angleInRadians = angle * (PFWConstants::PI / 180);
	sf::Vector2f r(	v.x * std::cos(angleInRadians) - v.y * std::sin(angleInRadians),
					v.x * std::sin(angleInRadians) - v.y * std::cos(angleInRadians));
	v = r;
}

float Utilities::AngleBetweenInRadians (const sf::Vector2f& dir1, const sf::Vector2f& dir2) {
	return std::acos(dir1.x * dir2.x + dir1.y * dir2.y);
}

float Utilities::AngleBetweenInDegrees (const sf::Vector2f& dir1, const sf::Vector2f& dir2) {
	return std::acos(dir1.x * dir2.x + dir1.y * dir2.y) * (180.0 / PFWConstants::PI);
}

char Utilities::Sign (float n, bool zeroIsPositive) {
	if (!zeroIsPositive && n == 0)
		return 0;
	else
		return n < 0 ? -1 : 1;
}
