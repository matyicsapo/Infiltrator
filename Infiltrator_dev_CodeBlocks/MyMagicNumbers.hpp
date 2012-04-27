#ifndef MYMAGICNUMBERS_HPP
#define MYMAGICNUMBERS_HPP

#include <string>
#include <SFML/Graphics/Color.hpp>

#define CAM_MOVE_SPD 350
#define CAM_ZOOM_SPD 1

#define ID_EDMENUBAR_WALLS 1001
#define ID_EDMENUBAR_PAINT 1002
#define ID_EDMENUBAR_CHARACTERS 1003
#define ID_EDMENUBAR_LEVELINFO 1004
#define ID_EDMENUBAR_NEWMAP 1005

#define ID_WORLDOBJECT 1

#define ID_ROCKWALL 10
#define ID_ROCK01 11
#define ID_ROCK02 12
#define ID_BUSH 13
#define ID_PINE 14
#define ID_ROOF 15
#define ID_ROAD 20
#define ID_PLAYER 30
#define ID_GUARD 31
#define ID_PRINCESS 32
#define ID_WAYPOINT 50
#define ID_COIN 70
#define ID_CHEST 71
#define ID_GOAL 72

#define LAYER_ROCKWALL 10
#define LAYER_ROCK01 10
#define LAYER_ROCK02 10
#define LAYER_BUSH 10
#define LAYER_PINE 10
#define LAYER_ROOF 10
#define LAYER_ROAD 1
#define LAYER_PLAYER 20
#define LAYER_GUARD 20
#define LAYER_PRINCESS 20
#define LAYER_WAYPOINT 15
#define LAYER_COIN 15
#define LAYER_CHEST 10
#define LAYER_GOAL 5
#define LAYER_VISIONTRIANGLE 3
#define LAYER_INTERACTEEHIGHLIGHT 2

#define TEXTURE_ROCKWALL "Content/Textures/colliders/rockwall.png"
#define TEXTURE_ROCK01 "Content/Textures/colliders/rock01.png"
#define TEXTURE_ROCK02 "Content/Textures/colliders/rock02.png"
#define TEXTURE_BUSH "Content/Textures/colliders/bush.png"
#define TEXTURE_PINE "Content/Textures/colliders/pine.png"
#define TEXTURE_ROOF "Content/Textures/colliders/roof.png"
#define TEXTURE_ROAD "Content/Textures/road.png"
#define TEXTURE_PLAYER "Content/Textures/player_thumbnail.png"
#define TEXTURE_GUARD "Content/Textures/guard_thumbnail.png"
#define TEXTURE_PRINCESS "Content/Textures/princess_thumbnail.png"
#define TEXTURE_WAYPOINT "Content/Textures/waypoint.png"
#define TEXTURE_COIN "Content/Textures/coin.png"
#define TEXTURE_CHEST "Content/Textures/chest.png"
#define TEXTURE_GOAL "Content/Textures/goal.png"

namespace MyMagicNumbers {
	const bool development_build = true;

	const int characterRadius = 50;
	const sf::Vector2f defaultLevelSize(1000, 1000);
	const sf::Vector2f minLevelSize(500, 500);
	const sf::Vector2f maxLevelSize(5000, 5000);
	const std::string untitledLevelName = "Untitled_";
	const std::string tempLevelPath = "Levels/temp.lev";
	const int goldValue_coin = 1;
	const int goldValue_chest = 5;
	const unsigned int guardNodeFollowDst = 45; // this is as close a guard tries to get to nodes
	const unsigned int princessFollowDst = characterRadius * 2.5f;

	const sf::Color tool_color_normal = sf::Color(255, 255, 255, 255);
	const sf::Color tool_color_allow = sf::Color(0, 255, 0, 128);
	const sf::Color tool_color_disallow = sf::Color(255, 0, 0, 128);
	const sf::Color tool_color_selected = sf::Color(64, 255, 64, 255);
}

#endif // MYMAGICNUMBERS_HPP
