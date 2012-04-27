#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <map>
#include <string>

namespace sf {
	class Image;
	class Font;
	class SoundBuffer;
};

class ResourceManager {
private:
    std::map<std::string, sf::Image*> images;
    std::map<std::string, sf::Font*> fonts;
    std::map<std::string, sf::SoundBuffer*> sounds;
//    std::map<std::string, sf::SoundBuffer*> musics;

    ResourceManager () {}

	~ResourceManager ();

	ResourceManager (ResourceManager const&);
	ResourceManager& operator= (ResourceManager const&);

public:
	static ResourceManager* Instance ();

	sf::Image* GetImage (std::string textureFile);
	void ReleaseImage (std::string textureFile);
	void ReleaseImage (const sf::Image* texture);

	sf::Font* GetFont (std::string fontFile);
	void ReleaseFont (std::string fontFile);
	void ReleaseFont (const sf::Font* font);

	sf::SoundBuffer* GetSound (std::string soundFile);
	void ReleaseSound (std::string soundFile);
	void ReleaseSound (const sf::SoundBuffer* sound);

//	sf::SoundBuffer* GetMusic (std::string soundFile);
//	void ReleaseMusic (std::string soundFile);
//	void ReleaseMusic (const sf::SoundBuffer* sound);
};

#define Resources ResourceManager::Instance()

#endif // RESOURCEMANAGER_HPP
