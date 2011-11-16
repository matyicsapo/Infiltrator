#include "GameObjectManager.hpp"

#include "GO/GameObject.hpp"

GameObjectManager* GameObjectManager::Instance () {
	static GameObjectManager instance;

	return &instance;
}

GameObjectManager::~GameObjectManager () {
	for (std::list<GameObject*>::iterator itGO = allGameObjects.begin(); itGO != allGameObjects.end(); itGO++) {
		delete *itGO;
	}
}

void GameObjectManager::ResetAll (float windowFakeScale) {
	for (std::list<GameObject*>::iterator itGO = allGameObjects.begin(); itGO != allGameObjects.end(); itGO++) {
		(*itGO)->Reset(windowFakeScale);
	}
}
