#include "GameObjectManager.hpp"

#include "GO/GameObject.hpp"
//#include <iostream>

GameObjectManager* GameObjectManager::Instance () {
	static GameObjectManager instance;

	return &instance;
}

GameObjectManager::~GameObjectManager () {
	for (TypeMap::iterator itT = allGOs.begin(); itT != allGOs.end(); itT++) {
		for (EntityMap::iterator itE = itT->second.begin(); itE != itT->second.end(); ) {
			EntityMap::iterator itDummy = itE++;

			delete itDummy->second;
		}
	}

	for (std::vector<GameObject*>::iterator itG = blackList.begin(); itG != blackList.end(); ++itG) {
		delete (*itG);
	}
}

void GameObjectManager::Add (GameObject* go) {
	allGOs[go->entityType].insert( std::pair<unsigned int, GameObject*>(go->ID, go) );
	newGOs[go->entityType].insert( std::pair<unsigned int, GameObject*>(go->ID, go) );

	// ez az hogy itt még nem lehet meghívni a SetFakeStuff-ot és ezért kellett egy külön tároló
//	// pure virtual method called
//	// terminate called without an exception
//	// Aborted
//	go->SetFakeStuff(this->windowFakeScale);
}

void GameObjectManager::Pop (GameObject* go) {
	allGOs[go->entityType].erase(go->ID);
	newGOs[go->entityType].erase(go->ID);
}

void GameObjectManager::Delete (GameObject* go) {
	Pop(go);

	blackList.push_back(go);
}

EntityMap& GameObjectManager::operator[] (unsigned int entityType) {
	return allGOs[entityType];
}

EntityMap& GameObjectManager::GetEntityMap (unsigned int entityType) {
	return allGOs[entityType];
}

void GameObjectManager::Update (float windowFakeScale, bool resized) {
	for (std::vector<GameObject*>::iterator itG = blackList.begin(); itG != blackList.end(); ++itG) {
		delete (*itG);
	}
	blackList.clear();

	if (resized) {
		this->windowFakeScale = windowFakeScale;

		for (TypeMap::iterator itT = allGOs.begin(); itT != allGOs.end(); itT++) {
			for (EntityMap::iterator itE = itT->second.begin(); itE != itT->second.end(); itE++) {
				itE->second->SetFakeStuff(windowFakeScale);
			}
		}
	}
	else if (!newGOs.empty()) {
		for (TypeMap::iterator itT = newGOs.begin(); itT != newGOs.end(); itT++) {
			for (EntityMap::iterator itE = itT->second.begin(); itE != itT->second.end(); itE++) {
				itE->second->SetFakeStuff(windowFakeScale);
			}
		}
	}

	newGOs.clear();
}

unsigned int GameObjectManager::GetValidID () {
	return ++lastID;
}
