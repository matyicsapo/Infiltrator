#include "Utilities.hpp"

#include <cmath>
#include "PFWConstants.hpp"
//#include <iostream>

Utilities* Utilities::Instance () {
	static Utilities instance;

	return &instance;
}

void Utilities::Normalize (sf::Vector2f& v) {
	v = Length(v) == 0 ? v : v / Length(v);
}

sf::Vector2f Utilities::Normalized (sf::Vector2f v) {
	return Length(v) == 0 ? v : v / Length(v);
}

float Utilities::Length (const sf::Vector2f& v) {
	return float(sqrt((v.x * v.x) + (v.y * v.y)));
}

float Utilities::Length (const sf::Vector2i& v) {
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

void Utilities::Clamp (float& value, float min, float max) {
	if (value < min)
		value = min;
	else
	if (value > max)
		value = max;
}
float Utilities::Clamped (float value, float min, float max) {
	if (value < min)
		return min;
	else
	if (value > max)
		return max;
	else
		return value;
}

float Utilities::Dst (const sf::Vector2f& p1, const sf::Vector2f& p2) { return sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) ); }
float Utilities::Dst (const sf::Vector2i& p1, const sf::Vector2i& p2) { return sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) ); }
float Utilities::Dst (const sf::Vector2i& p1, const sf::Vector2f& p2) { return sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) ); }
float Utilities::Dst (const sf::Vector2f& p1, const sf::Vector2i& p2) { return sqrt( pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) ); }

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

float Utilities::Max (const float& lhs, const float& rhs) {
	return lhs > rhs ? lhs : rhs;
}

float Utilities::Min (const float& lhs, const float& rhs) {
	return lhs < rhs ? lhs : rhs;
}

sf::Vector2f Utilities::Normal (sf::Vector2f v) {
	return sf::Vector2f(-v.y, v.x);
}

sf::Vector2f Utilities::NormalRightHand (sf::Vector2f v) {
	return Normal(v);
}

sf::Vector2f Utilities::NormalLeftHand (sf::Vector2f v) {
	return sf::Vector2f(v.y, -v.x);
}

sf::Vector2f Utilities::Project (sf::Vector2f toProject, sf::Vector2f projectTo) {
	sf::Vector2f projection(0, 0);

	float projectToSqrLength = projectTo.x * projectTo.x + projectTo.y * projectTo.y;
	if (projectToSqrLength == 0) {
//std::cout << "ERROR: division by zero" << std::endl;
		return projection;
	}

	float dp = toProject.x * projectTo.x + toProject.y * projectTo.y;

	projection.x = (dp / projectToSqrLength) * projectTo.x;
	projection.y = (dp / projectToSqrLength) * projectTo.y;

//std::cout << projection.x << " " << projection.y << std::endl;

	return projection;
}
