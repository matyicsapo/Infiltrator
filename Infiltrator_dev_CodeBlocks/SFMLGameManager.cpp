#include "SFMLGameManager.hpp"

#include <iostream>

SFMLGameManager::SFMLGameManager () : pGameStateMachine(0), bClear(true), clearColor(0, 0, 0) {
	Create();
}

// singleton
// ugyan a header guard miatt ha ez ott lenne definiálva akkorsem lenne gond
// de anélkül minden fájlban ahol a header include -olva van létezne egy példány
SFMLGameManager* SFMLGameManager::Instance () {
	static SFMLGameManager instance;

	return &instance;
}

void SFMLGameManager::Create () {
	bool fullScreen = false;

	//------------------------------------
	// getting all videomodes available
	// needed only in fullscreen mode
	//------------------------------------
	//unsigned int VideoModesCount = sf::VideoMode::GetModesCount();
	//for (unsigned int i = 0; i < VideoModesCount; ++i)
	//{
	//sf::VideoMode Mode = sf::VideoMode::GetMode(i);

	// Mode is a valid video mode
	//}
	sf::VideoMode videoMode;
	if (fullScreen) {
		//videoMode = sf::VideoMode::GetMode(0); // 0th, best mode
		videoMode = sf::VideoMode::GetDesktopMode(); // desktop mode
	}
	else {
		unsigned int width = 800;
		unsigned int height = 600;
		unsigned int bitsPerPixel = 32;
		videoMode = sf::VideoMode(width, height, bitsPerPixel);
	}

	std::string windowTitle = "Game";

	unsigned long windowStyle = fullScreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close;

	unsigned int depthBits = 24;
	unsigned int stencilBits = 8;
	unsigned int antialiasingLevel = 0;
	sf::WindowSettings windowSettings(depthBits, stencilBits, antialiasingLevel);

    mainWindow.Create(videoMode, windowTitle, windowStyle, windowSettings);

    // sg. is needed on: for example Ubuntu 11.4
    mainWindow.UseVerticalSync(true);
    //mainWindow.SetFramerateLimit(60);
}

int SFMLGameManager::Run () {
    while (mainWindow.IsOpened()) {
        sf::Event event;
        while (mainWindow.GetEvent(event)) {
            if (event.Type == sf::Event::Closed)
                mainWindow.Close();
        }

        //clearColor = sf::Color(64, 64, 64, 0);

        //std::cout << (int)clearColor.r << " " << (int)clearColor.g << " " << (int)clearColor.b <<
			//" " << (int)clearColor.a << std::endl;

        // ha a kép teljes egésze felülrajzolódik akkor a képernyőtörlés redundáns
        if (bClear) mainWindow.Clear(clearColor);

        if (pGameStateMachine) pGameStateMachine->Update();

        mainWindow.Display();
    }

    return EXIT_SUCCESS;
}
