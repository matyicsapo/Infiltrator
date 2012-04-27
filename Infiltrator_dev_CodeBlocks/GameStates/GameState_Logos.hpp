#ifndef GAMESTATE_LOGOS_HPP
#define GAMESTATE_LOGOS_HPP

#include "../PFrameWork/State.hpp"

#include <vector>
#include <SFML/Graphics/Color.hpp>

class ScreenSprite;

class DarkFader;

class SoundEffect;
class Music;

class MenuStateMachine;
template<class T> class StateMachine;

class GameState_Logos : public GameState {
private:
	bool finished;

	StateMachine<MenuState>* mMenuStateMachine;

	DarkFader* inoutFader;

	ScreenSprite* currentLogoSprite;

	float showTimeLeft;

	std::vector<std::pair<std::string, sf::Color> > logos;
	std::vector<std::pair<std::string, sf::Color> >::iterator currentLogo;

	void StartMainMenu ();
	void StartLanguageSelection ();
	void FadeToNextLogo ();

	void Finish ();

public:
	GameState_Logos ();

	~GameState_Logos ();

	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents);

	virtual void Init ();

	virtual void Update (float dT);

	virtual void DrawScreenSpace (sf::RenderWindow& rwin);

	void OnFinishFadeLight ();

//	void ContinueWithLanguage (std::string lang);
};

#endif // GAMESTATE_LOGOS_HPP
