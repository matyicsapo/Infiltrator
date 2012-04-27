#include "SFMLGameManager.hpp"

#include "StateMachine.hpp"
#include "State.hpp"
#include "DrawManager.hpp"
#include "PFWConstants.hpp"
#include "GameObjectManager.hpp"
#include "ResourceManager.hpp"
#include "SpriteAnimationManager.hpp"
#include "ColliderManager.hpp"
#include "SoundManager.hpp"

SFMLGameManager::SFMLGameManager () : deltaTime(0),
										windowFakeScale(1),
										mGameStateMachine(0),
										mWorldCamera2D(sf::FloatRect(0, 0,
															PFWConstants::baseResolution.x,
															PFWConstants::baseResolution.y),
														.5, 2),
										bClear(true),
										clearColor(83, 131, 212) {

	unsigned int firstW = sf::VideoMode::GetMode(0).Width;
	unsigned int firstH = sf::VideoMode::GetMode(0).Height;

	for (unsigned int i = 0; i < sf::VideoMode::GetModesCount(); i++) {
		if (firstW == sf::VideoMode::GetMode(i + 1).Width
					&&
				firstH == sf::VideoMode::GetMode(i + 1).Height) {
			sfVideoModesCount = i + 1;
			break;
		}
	}

	Create(PFWConstants::baseResolution.x, PFWConstants::baseResolution.y, false);
}

SFMLGameManager* SFMLGameManager::Instance () {
	static SFMLGameManager instance;

	return &instance;
}

SFMLGameManager::~SFMLGameManager () {
	delete mGameStateMachine;
}

void SFMLGameManager::SetGameStateMachine (StateMachine<GameState>* newGameStateMachine) {
	mGameStateMachine = newGameStateMachine;
}

StateMachine<GameState>* SFMLGameManager::GetGameStateMachine () {
	return mGameStateMachine;
}

void SFMLGameManager::Create (unsigned int width, unsigned int height, bool enable) {
	fullScreen = enable;

	Create(width, height);
}

void SFMLGameManager::Create (unsigned int width, unsigned int height) {
	sf::VideoMode videoMode;
	unsigned int w = width;
	unsigned int h = height;
	unsigned int bitsPerPixel = 32;
	videoMode = sf::VideoMode(w, h, bitsPerPixel);

	std::string windowTitle = "Infiltrator";

	unsigned long windowStyle = fullScreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;

	unsigned int depthBits = 24;
	unsigned int stencilBits = 8;
	unsigned int antialiasingLevel = 0;
	sf::WindowSettings windowSettings(depthBits, stencilBits, antialiasingLevel);

	win.Create(videoMode, windowTitle, windowStyle, windowSettings);

	// ******************************************************************************************
	// sg. is needed on: for example (MY) Ubuntu 11.4
	// not on MY Ubuntu 11.10 ?(not sure if helps)
	// less problem in fullScreen ?(not sure if always)
    win.UseVerticalSync(true);
    win.SetFramerateLimit(60);

    OnResize();
    reset = true;

    win.EnableKeyRepeat(false);
}

int SFMLGameManager::Run () {
    while (win.IsOpened()) {
    	deltaTime = win.GetFrameTime();

    	sfEvents.clear();
    	sf::Event event;
    	while (win.GetEvent(event)) {
    		if (event.Type == sf::Event::Resized) {
				reset = true;
    		}
    		//else
				// "minden" event összegyûjtése továbbadásra
				sfEvents.push_front(event);
    	}

		if (reset) {
			OnResize();
		}

		Sounds->Update();

		mGameStateMachine->RefreshStack();

		mGameStateMachine->HandleSFEvents(sfEvents);

		win.SetView(mWorldCamera2D.GetSFView());

        // ha a kép teljes egésze felülrajzolódik akkor a képernyõtörlés redundáns
        if (bClear) win.Clear(clearColor);

		// simple non-processed(non-fixed & no-nothing-ed) deltaTime
        mGameStateMachine->Update(deltaTime);

        GameObjects->Update(windowFakeScale, reset);

		mGameStateMachine->DrawWorldSpace(win);

		win.SetView(sf::View(sf::FloatRect(0, 0, win.GetWidth(), win.GetHeight())));
		mGameStateMachine->DrawScreenSpace(win);

        win.Display();

        reset = false;
    }

	// deleting game, should delete everything which is pretty much fine since the program is
	// about to close
    delete mGameStateMachine; mGameStateMachine = 0;

    SpriteAnimationManager::Release();
    ColliderManager::Release();

    return EXIT_SUCCESS;
}

sf::Vector2f SFMLGameManager::ConvertCoordsWindowToWorld (sf::Vector2f windowSpaceCoords) {
	sf::Vector2f worldSpaceCoords = win.ConvertCoords(windowSpaceCoords.x, windowSpaceCoords.y,
													&mWorldCamera2D.GetSFView());

	return worldSpaceCoords / windowFakeScale;
}
sf::Vector2f SFMLGameManager::ConvertCoordsWindowToWorld (float windowSpaceX, float windowSpaceY) {
	return ConvertCoordsWindowToWorld(sf::Vector2f(windowSpaceX, windowSpaceY));
}

void SFMLGameManager::OnResize () {
	sf::Vector2f scaleV( (win.GetWidth()) / PFWConstants::baseResolution.x,
						(win.GetHeight()) / PFWConstants::baseResolution.y);

	windowFakeScale = scaleV.x < scaleV.y ? scaleV.x : scaleV.y;

	mWorldCamera2D.SetFakeScale(windowFakeScale);

	mWorldCamera2D.Reset(win);
}
