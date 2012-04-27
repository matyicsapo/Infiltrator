#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include <map>
#include <vector>

class GameObject;

typedef std::map<unsigned int, GameObject*> EntityMap;

typedef std::map<unsigned int, EntityMap> TypeMap;

class GameObjectManager {
private:
	int lastID;

	TypeMap allGOs;
	TypeMap newGOs;
	std::vector<GameObject*> blackList;

	float windowFakeScale;

    GameObjectManager () {}

	~GameObjectManager ();

	GameObjectManager (GameObjectManager const&);
	GameObjectManager& operator= (GameObjectManager const&);

public:
	static GameObjectManager* Instance ();

	void Add (GameObject* go);
	void Pop (GameObject* go);

	void Delete (GameObject* go);

	EntityMap& operator[] (unsigned int entityType);
	EntityMap& GetEntityMap (unsigned int entityType);

	void Update (float windowFakeScale, bool resized);

	unsigned int GetValidID ();
};

#define GameObjects GameObjectManager::Instance()

#endif // GAMEOBJECTMANAGER_HPP
