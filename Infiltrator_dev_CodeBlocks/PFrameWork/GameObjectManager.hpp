#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include <list>

class GameObject;

class GameObjectManager {
private:
	std::list<GameObject*> allGameObjects;

    GameObjectManager () {}

	~GameObjectManager ();

	GameObjectManager (GameObjectManager const&);
	GameObjectManager& operator= (GameObjectManager const&);

public:
	static GameObjectManager* Instance ();

	void Add (GameObject* go) { allGameObjects.push_back(go); };
	void Pop (GameObject* go) { allGameObjects.remove(go); }

	void ResetAll (float windowFakeScale);
};

#define GameObjects GameObjectManager::Instance()

#endif // GAMEOBJECTMANAGER_HPP
