#include "SFMLGameManager.hpp"

#include "DrawManager.hpp"
#include "MyConstants.hpp"

SFMLGameManager::SFMLGameManager () : mGameStateMachine(0),
										worldCamera2D(sf::FloatRect(0, 0, 800, 600), .5, 2),
										bClear(true),
										clearColor(0, 128, 0) {
	Create(800, 600, false);
}

SFMLGameManager* SFMLGameManager::Instance () {
	static SFMLGameManager instance;

	return &instance;
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

	std::string windowTitle = "Game";

	unsigned long windowStyle = fullScreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;

	unsigned int depthBits = 24;
	unsigned int stencilBits = 8;
	unsigned int antialiasingLevel = 0;
	sf::WindowSettings windowSettings(depthBits, stencilBits, antialiasingLevel);

	win.Create(videoMode, windowTitle, windowStyle, windowSettings);

	// ********************************************************************************************************
	// sg. is needed on: for example (MY) Ubuntu 11.4
	// not on MY Ubuntu 11.10 ?(not sure if helps)
	// less problem in fullScreen ?(not sure if always)
    win.UseVerticalSync(true);
    //win.SetFramerateLimit(60);

    reset = true;
}

int SFMLGameManager::Run () {
    while (win.IsOpened()) {
    	sfEvents.clear();
    	sf::Event event;
    	while (win.GetEvent(event)) {
    		if (event.Type == sf::Event::Resized) {
				reset = true;
    		}

			// minden event összegyûjtése továbbadásra
			sfEvents.push_front(event);
    	}

		if (mGameStateMachine) mGameStateMachine->HandleEvents(sfEvents);

		float windowFakeScale = 0;
		if (reset) {
			sf::Vector2f scaleV( (win.GetWidth()) / MyConstants::baseResolution.x,
								(win.GetHeight()) / MyConstants::baseResolution.y);

			windowFakeScale = scaleV.x < scaleV.y ? scaleV.x : scaleV.y;

			worldCamera2D.Reset(win);
		}

		win.SetView(worldCamera2D.GetSfView());

        // ha a kép teljes egésze felülrajzolódik akkor a képernyõtörlés redundáns
        if (bClear) win.Clear(clearColor);

		// simple non-processed(non-fixed & no-nothing-ed) deltaTime
        if (mGameStateMachine) mGameStateMachine->Update(win.GetFrameTime());

        DrawManager::Instance()->DrawWorld(win, windowFakeScale);

		win.SetView(sf::View(sf::FloatRect(0, 0, win.GetWidth(), win.GetHeight())));
		DrawManager::Instance()->DrawScreenSpace(win, windowFakeScale);

        win.Display();

        reset = false;
    }

	// deleting game, should delete everything which is pretty much fine since the program is about to close
	// also and more important actually, any dumbfuck mistakes leaving some drawable's still alive can now be
	// safely deleted by the DrawManager
    delete mGameStateMachine; mGameStateMachine = 0;

    return EXIT_SUCCESS;
}
