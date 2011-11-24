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

void GameObjectManager::Add (GameObject* go) {
	allGameObjects.push_back(go);
	newGameObjects.push_back(go);

//	// pure virtual method called
//	// terminate called without an exception
//	// Aborted
//	go->Reset(this->windowFakeScale);
}

void GameObjectManager::Update (float windowFakeScale) {
	if (windowFakeScale != 0) {
		this->windowFakeScale = windowFakeScale;

		for (std::list<GameObject*>::iterator itGO = allGameObjects.begin(); itGO != allGameObjects.end(); itGO++) {
			(*itGO)->Reset(windowFakeScale);
		}

		newGameObjects.clear();
	}
	else if (!newGameObjects.empty()) {
		for (std::vector<GameObject*>::iterator itGO = newGameObjects.begin(); itGO != newGameObjects.end(); itGO++) {
			(*itGO)->Reset(this->windowFakeScale);
		}

		newGameObjects.clear();
	}
}
