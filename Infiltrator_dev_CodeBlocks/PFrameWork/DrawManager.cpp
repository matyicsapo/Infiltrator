#include "DrawManager.hpp"

#include <boost/bind.hpp>

#include "GO/WorldDrawable.hpp"
#include "GO/ScreenSpaceDrawable.hpp"

DrawManager* DrawManager::Instance () {
	static DrawManager instance;

	return &instance;
}

DrawManager::~DrawManager () {
	for (std::list<WorldDrawable*>::iterator itDrawable = mWorldSpaceDrawables.begin(); itDrawable != mWorldSpaceDrawables.end(); itDrawable++) {
		delete (*itDrawable);
	}

	for (std::list<ScreenSpaceDrawable*>::iterator itScreenSpaceDrawable = mScreenSpaceDrawables.begin();
			itScreenSpaceDrawable != mScreenSpaceDrawables.end(); itScreenSpaceDrawable++) {
		delete (*itScreenSpaceDrawable);
	}
}

void DrawManager::DrawWorldSpace (sf::RenderWindow& rwin) {
	for (std::list<WorldDrawable*>::iterator itDrawable = mWorldSpaceDrawables.begin(); itDrawable != mWorldSpaceDrawables.end();
			itDrawable++) {
		(*itDrawable)->Draw(rwin);
	}
}

void DrawManager::DrawScreenSpace (sf::RenderWindow& rwin) {
	for (std::list<ScreenSpaceDrawable*>::iterator itScreenSpaceDrawable = mScreenSpaceDrawables.begin();
			itScreenSpaceDrawable != mScreenSpaceDrawables.end(); itScreenSpaceDrawable++) {
		(*itScreenSpaceDrawable)->Draw(rwin);
	}
}

void DrawManager::SortByLayerDepthAscendingWorldSpace () {
	mWorldSpaceDrawables.sort( boost::bind(&DrawManager::Comp_layerDepth, this, _1, _2) );
}

void DrawManager::SortByLayerDepthAscendingScreenSpace () {
	mScreenSpaceDrawables.sort( boost::bind(&DrawManager::Comp_layerDepth, this, _1, _2) );
}

bool DrawManager::Comp_layerDepth (DrawableBase* d1, DrawableBase* d2) {
	return d1->layerDepth < d2->layerDepth ? true : false;
}
