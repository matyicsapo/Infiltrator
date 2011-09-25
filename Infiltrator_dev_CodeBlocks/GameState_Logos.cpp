#include "GameState_Logos.hpp"

#include "CfgParser.hpp"

#include <iostream>

void GameState_Logos::Enter (GameStateMachine* stateMachine) {
	//sfWin = SFMLGameManager::Instance()->GetRenderWindow();

	CfgParser::CfgContents contents;
    CfgParser::Instance()->GetContents("Content/logos.ini", contents);

    delay = Convert::Instance()->ToNum<float>(contents[""]["delay"]);
    fadeDuration = Convert::Instance()->ToNum<float>(contents[""]["fadeDuration"]);
    showDuration = Convert::Instance()->ToNum<float>(contents[""]["showDuration"]);

    // iterating from 1st named section
    for (CfgParser::CfgContents::CfgSectionMap::iterator itSection = ++contents.GetIteratorBegin();
            itSection != contents.GetIteratorEnd(); itSection++) {

		std::string path = contents[(*itSection)->first]["path"];
		unsigned char bgColorR = Convert::Instance()->ToNum<unsigned char>(contents[(*itSection)->first]["bgColorR"]);
        unsigned char bgColorG = Convert::Instance()->ToNum<unsigned char>(contents[(*itSection)->first]["bgColorG"]);
        unsigned char bgColorB = Convert::Instance()->ToNum<unsigned char>(contents[(*itSection)->first]["bgColorB"]);

        logos.push_back(new Logo(path, bgColorR, bgColorG, bgColorB));
    }

    currentLogo = logos.begin();
    SFMLGameManager::Instance()->SetClearColor((*currentLogo)->bgColor);

    std::cout << (int)(*currentLogo)->bgColor.r << " " << (int)(*currentLogo)->bgColor.g << " " <<
		(int)(*currentLogo)->bgColor.b << " " << (int)(*currentLogo)->bgColor.a << std::endl;
}

void GameState_Logos::Execute (GameStateMachine* stateMachine) {
	if (sfInput.IsKeyDown(sf::Key::Space)) {
		if (!keyDown_Space) {
			keyDown_Space = true;

			currentLogo++;
			if (currentLogo == logos.end()) {
				std::cout << "woot" << std::endl;
				currentLogo = logos.begin();
			}

			SFMLGameManager::Instance()->SetClearColor((*currentLogo)->bgColor);

			std::cout << (int)(*currentLogo)->bgColor.r << " " << (int)(*currentLogo)->bgColor.g << " " <<
				(int)(*currentLogo)->bgColor.b << " " << (int)(*currentLogo)->bgColor.a << std::endl;
		}
	}
	else {
		keyDown_Space = false;
	}

	AnimatedSprite* asp = 0;
	asp = dynamic_cast<AnimatedSprite*>( (*currentLogo)->sprite );
	if (asp != 0) {
		asp->Update();
	}

	(*currentLogo)->sprite->Draw();
}

void GameState_Logos::Exit (GameStateMachine* stateMachine) {
}
