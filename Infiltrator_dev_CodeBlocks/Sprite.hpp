#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Drawable.hpp"

#include "SFML/Graphics.hpp"

#include <string>

#include "ImageManager.hpp"

#include "SFMLGameManager.hpp"

class Sprite : Drawable {
protected:
	sf::RenderWindow* sfWin;
	sf::Sprite* sfSprite;

public:
	Sprite () : sfSprite(0) {
		sfWin = SFMLGameManager::Instance()->GetRenderWindow();
    }

	Sprite (std::string fileName) {
		sfWin = SFMLGameManager::Instance()->GetRenderWindow();

        sfSprite = 0;

    	sf::Image* imgPointer = ImageManager::Instance()->GetImage(fileName);
        if (imgPointer != 0) {
            sfSprite = new sf::Sprite();
            sfSprite->SetImage(*imgPointer);

            sfSprite->SetCenter(imgPointer->GetWidth() / 2, imgPointer->GetHeight() / 2);

            sfSprite->SetPosition(sfWin->GetWidth() / 2, sfWin->GetHeight() / 2);
        }
    }

    virtual ~Sprite () {
        delete sfSprite; sfSprite = 0;
    }

	virtual void Draw () {
		sfWin->Draw(*sfSprite);
	}

	void SetPosition (sf::Vector2f position) {
		sfSprite->SetPosition(position);
	}
};

#endif
