#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <map>
#include <string>

namespace sf {
	class Image;
	class Font;
};

class ResourceManager {
private:
    std::map<std::string, sf::Image*> images;
    std::map<std::string, sf::Font*> fonts;

    ResourceManager () {}

	~ResourceManager ();

	ResourceManager (ResourceManager const&);
	ResourceManager& operator= (ResourceManager const&);

public:
	static ResourceManager* Instance ();

	sf::Image* GetImage (std::string textureFile);

	sf::Font* GetFont (std::string fontFile);
};

#endif // RESOURCEMANAGER_HPP
