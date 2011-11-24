#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include <list>
#include <vector>

class GameObject;

class GameObjectManager {
private:
	std::list<GameObject*> allGameObjects;
	std::vector<GameObject*> newGameObjects;

	float windowFakeScale;

    GameObjectManager () {}

	~GameObjectManager ();

	GameObjectManager (GameObjectManager const&);
	GameObjectManager& operator= (GameObjectManager const&);

public:
	static GameObjectManager* Instance ();

	void Add (GameObject* go);
	void Pop (GameObject* go) { allGameObjects.remove(go); }

	void Update (float windowFakeScale);
};

#define GameObjects GameObjectManager::Instance()

#endif // GAMEOBJECTMANAGER_HPP
