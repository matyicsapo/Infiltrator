#ifndef GAMEPLAYHUD_HPP
#define GAMEPLAYHUD_HPP

#include "PFrameWork/DrawManager.hpp"
#include "PFrameWork/ConfigParser.hpp"

class ScreenString;
class ScreenSprite;
class GameObject;

class GameplayHUD : public DrawManager {
private:
	GameObject* interactee;

	ScreenString* interactScreenString;
	ScreenSprite* interactScreenSprite;
	std::string interactEntry;

	ScreenString* caption_currObj;
	ScreenSprite* sprite_currObj;

	ScreenString* amount_gold;
	ScreenSprite* sprite_gold;

public:
	GameplayHUD ();
	~GameplayHUD ();

	void OnLanguageChanged(CfgContents& contents_lang);

	void SetInteractContextNote(GameObject* interactee, std::string interactEntry = "");
	GameObject* GetInteractee ();

	void SetAmountGold (int gold);

	void SetObjective (std::string imageFile);
};

#endif // GAMEPLAYHUD_HPP
