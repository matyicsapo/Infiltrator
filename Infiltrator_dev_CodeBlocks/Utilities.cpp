#include "Utilities.hpp"

#include <cmath>

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

float Utilities::Length (sf::Vector2f v) {
	return float(sqrt((v.x * v.x) + (v.y * v.y)));
}
