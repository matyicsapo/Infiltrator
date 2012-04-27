#include "GameplayHUD.hpp"

#include "PFrameWork/GO/ScreenString.hpp"
#include "PFrameWork/GO/ScreenSprite.hpp"
#include "PFrameWork/TINI/TINI.hpp"
#include "PFrameWork/PFWConstants.hpp"
#include "PFrameWork/Conversion.hpp"
#include "Session.hpp"

GameplayHUD::GameplayHUD () : interactee(0), interactScreenString(0), interactScreenSprite(0),
								caption_currObj(0), sprite_currObj(0) {
	sprite_gold = new ScreenSprite("Content/Textures/coin.png", ScreenSpaceDrawable::LEFT);
	AddScreenSpace(sprite_gold);
	sprite_gold->SetPosition(sf::Vector2f(10, 5));
	amount_gold = new ScreenString("0");
	AddScreenSpace(amount_gold);
	amount_gold->SetPosition(sf::Vector2f(70, 11));
}

GameplayHUD::~GameplayHUD () {
	ClearAll();

	delete interactScreenString;
	delete interactScreenSprite;

	delete caption_currObj;
	delete sprite_currObj;

	delete amount_gold;
	delete sprite_gold;
}

void GameplayHUD::OnLanguageChanged(CfgContents& contents_lang) {
	if (interactScreenString) {
		interactScreenString->SetText(contents_lang["interact"][interactEntry]);
	}

	if (caption_currObj) {
		CfgContents contents_lang;
		CfgParser->GetContents(std::string("Content/LanguageFiles/") + CurrentSession->language
			+ std::string(".ini"), contents_lang);

		caption_currObj->SetText(contents_lang["HUD"]["currobj"]);
	}
}

void GameplayHUD::SetInteractContextNote(GameObject* interactee, std::string interactEntry) {
	this->interactee = interactee;
	this->interactEntry = interactEntry;

	PopScreenSpace(interactScreenString);
	delete interactScreenString;
	PopScreenSpace(interactScreenSprite);
	delete interactScreenSprite;

	if (interactee) {
		if (interactEntry != "") {
			TINI::TINIObject langFile("Content/LanguageFiles/"
				+ CurrentSession->language + ".ini");
			interactScreenString =
				new ScreenString(langFile.GetValue("interact", interactEntry),
								PFWConstants::defaultFontSize,
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::BOTTOM,
								0);
			AddScreenSpace(interactScreenString);
			interactScreenString->SetPosition(sf::Vector2f(0, -20));

			interactScreenSprite =
				new ScreenSprite("Content/Textures/kb_spacebar.png",
								ScreenSpaceDrawable::CENTER, ScreenSpaceDrawable::BOTTOM,
								0);
			AddScreenSpace(interactScreenSprite);
			interactScreenSprite->SetPosition(sf::Vector2f(0, -60));
		}
	}
	else {
		interactScreenString = 0;
		interactScreenSprite = 0;
	}
}

GameObject* GameplayHUD::GetInteractee () {
	return interactee;
}

void GameplayHUD::SetAmountGold (int gold) {
	amount_gold->SetText(Convert->ToString(gold));
}

void GameplayHUD::SetObjective (std::string imageFile) {
	PopScreenSpace(sprite_currObj);
	delete sprite_currObj;

	if (imageFile != "") {
		if (!caption_currObj) {
			CfgContents contents_lang;
			CfgParser->GetContents(std::string("Content/LanguageFiles/") + CurrentSession->language
				+ std::string(".ini"), contents_lang);

			caption_currObj = new ScreenString(contents_lang["HUD"]["currobj"],
												PFWConstants::defaultFontSize,
												ScreenSpaceDrawable::CENTER);
			AddScreenSpace(caption_currObj);
			caption_currObj->SetPosition(sf::Vector2f(0, 10));
		}

		sprite_currObj = new ScreenSprite(imageFile, ScreenSpaceDrawable::CENTER);
		AddScreenSpace(sprite_currObj);
		sprite_currObj->SetPosition(sf::Vector2f(0, 50));
		sprite_currObj->SetScale(.5f);
	}
	else {
		sprite_currObj = 0;

		PopScreenSpace(caption_currObj);
		delete caption_currObj;
		caption_currObj = 0;
	}
}
