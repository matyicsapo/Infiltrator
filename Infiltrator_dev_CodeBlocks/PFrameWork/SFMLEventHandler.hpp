#ifndef SFMLEVENTHANDLER_HPP
#define SFMLEVENTHANDLER_HPP

#include <SFML/Window.hpp>
#include <list>

class SFMLEventHandler {
public:
	virtual void HandleSFEvents(std::list<sf::Event>& sfEvents) = 0;
};

#endif // SFMLEVENTHANDLER_HPP
