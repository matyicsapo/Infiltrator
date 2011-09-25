#ifndef IMAGEMANAGER_HPP
#define IMAGEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>

class ImageManager {
    std::map<std::string, sf::Image*> images;

    ImageManager () {}

	~ImageManager () {}

	ImageManager (ImageManager const&);
	ImageManager& operator= (ImageManager const&);

public:
	static ImageManager* Instance ();

	sf::Image* GetImage (std::string imageName);
};

#endif
