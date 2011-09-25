#ifndef GAMESTATE_LOGOS_HPP
#define GAMESTATE_LOGOS_HPP

#include "GameState.hpp"

#include "TimeAnimatedSprite.hpp"

#include <vector>

struct Logo {
	Sprite* sprite;

	sf::Color bgColor;

	~Logo () {
		delete sprite; sprite = 0;
	}

	Logo (std::string fileName, unsigned char bgColorR = 0, unsigned char bgColorG = 0, unsigned char bgColorB = 0) {
		this->bgColor = sf::Color(bgColorR, bgColorG, bgColorB); // just clear color - don't care for alpha

		if (fileName.rfind("/Animated/") != std::string::npos) {
			sprite = new TimeAnimatedSprite(fileName);
		}
		else {
			sprite = new Sprite(fileName);
		}
	}
};

class GameState_Logos : public GameState {
	//sf::RenderWindow* sfWin;
	const sf::Input& sfInput;

	bool keyDown_Space;

	float delay, fadeDuration, showDuration;

	std::vector<Logo*> logos;

	std::vector<Logo*>::iterator currentLogo;

public:
	GameState_Logos () : sfInput(SFMLGameManager::Instance()->GetRenderWindow()->GetInput()), keyDown_Space(false) {}

	~GameState_Logos () {
		for (std::vector<Logo*>::iterator itLogo = logos.begin(); itLogo != logos.end(); itLogo++) {
			delete *itLogo;
		}
	}

	void Enter (GameStateMachine* stateMachine);

	void Execute (GameStateMachine* stateMachine);

	void Exit (GameStateMachine* stateMachine);
};

#endif
