#include "ImageManager.hpp"

ImageManager* ImageManager::Instance () {
	static ImageManager instance;

	return &instance;
}

sf::Image* ImageManager::GetImage (std::string imageName) {
    if (images.count(imageName) == 1) {
        return images.find(imageName)->second;
    }
    else {
        sf::Image* imgNew = new sf::Image();
        if (imgNew->LoadFromFile(imageName)) {
            return images.insert(std::pair<std::string, sf::Image*>(imageName, imgNew)).first->second;
        }

        return 0;
    }
}
