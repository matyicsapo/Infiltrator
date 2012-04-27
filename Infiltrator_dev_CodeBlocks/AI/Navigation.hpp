#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <SFML/System/Vector2.hpp>
#include <deque>
#include <map>
#include <set>

struct CompareSFVector2i {
	inline bool operator() (const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
		return lhs.x == rhs.x ? lhs.y < rhs.y : lhs.x < rhs.x;
	}
};

typedef std::map< sf::Vector2i, std::deque<sf::Vector2i>, CompareSFVector2i> NavMap;

class ColliderBase;

class Navigation {
private:
	std::set<sf::Vector2i, CompareSFVector2i> openNodes;
	std::deque<sf::Vector2i> adjacentNodes;
	std::deque<sf::Vector2i> newNodes;
	std::set<sf::Vector2i, CompareSFVector2i> extraNodes;
	sf::Vector2i currBasePos;
	ColliderBase* currDummyCollider;
	float currMinStep;

	bool cancelledGenerateNavMap;

	NavMap* navMap;

    Navigation () : cancelledGenerateNavMap(false), navMap(0) {}

	~Navigation ();

	Navigation (Navigation const&);
	Navigation& operator= (Navigation const&);

	void NavGraphGen_ProcessAdjacent (sf::Vector2i* offset);

public:
	static Navigation* Instance ();

	NavMap* GenerateNavMap (sf::Vector2i levelSize, float minStep, float step);
	void CancelGenerateNavMap () { cancelledGenerateNavMap = true; }

	NavMap* GetNavMap ();
	void SetNavMap (NavMap* navMap);

	std::deque<sf::Vector2i>* Path (sf::Vector2i from, sf::Vector2i to);
};

#define Nav Navigation::Instance()

enum ThreadState_GenerateNavMap { NONE = 0, SUCCESS = 1, FAILURE = 2 };

struct ThreadData_GenerateNavMap {
public:
	ThreadState_GenerateNavMap& navMapGenThreadState;
	sf::Vector2i levelSize;

	ThreadData_GenerateNavMap (ThreadState_GenerateNavMap& navMapGenThreadState, sf::Vector2i levelSize)
		: navMapGenThreadState(navMapGenThreadState),
		levelSize(levelSize) {}
};

void Async_GenerateNavMap (void* UserData);

#endif // NAVIGATION_HPP
