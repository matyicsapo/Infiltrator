#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Window.hpp>
#include <string>

class WorldAnimatedSprite;

class Player {
private:
	WorldAnimatedSprite* was;

	const sf::Vector2f baseLook; // unused variable
	sf::Vector2f look; // unused variable *******************************************************

	float walkSpd;
	float turnSpd;

	float TargetAngle (sf::Vector2f normalizedVToTarget);
	float AngleBetween (sf::Vector2f nV1, sf::Vector2f nV2);

public:
	Player ();
	Player (Player const& xPlayer);

	~Player ();

	void LookAt (sf::Vector2f target);

	void SetPosition (sf::Vector2f position);
	sf::Vector2f GetPosition ();
	void WalkInDir (float dT, sf::Vector2f direction, bool rotate = true);
	void WalkTowards (float dT, sf::Vector2f targetWorldPos, bool rotate = true);
};

#endif // PLAYER_HPP_INCLUDED
