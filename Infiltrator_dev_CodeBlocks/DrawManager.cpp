#include "DrawManager.hpp"

#include <boost/bind.hpp>

DrawManager* DrawManager::Instance () {
	static DrawManager instance;

	return &instance;
}

DrawManager::~DrawManager () {
	for (std::list<Drawable*>::iterator itDrawable = mDrawables.begin(); itDrawable != mDrawables.end(); itDrawable++) {
		delete (*itDrawable);
	}

	for (std::list<ScreenSpaceDrawable*>::iterator itScreenSpaceDrawable = mScreenSpaceDrawables.begin();
			itScreenSpaceDrawable != mScreenSpaceDrawables.end(); itScreenSpaceDrawable++) {
		delete (*itScreenSpaceDrawable);
	}
}

void DrawManager::DrawWorld (sf::RenderWindow& rwin, float windowFakeScale) {
	if (windowFakeScale == 0) {
		for (std::list<Drawable*>::iterator itDrawable = mDrawables.begin(); itDrawable != mDrawables.end();
				itDrawable++) {
			(*itDrawable)->Draw(rwin);
		}
	}
	else {
		for (std::list<Drawable*>::iterator itDrawable = mDrawables.begin(); itDrawable != mDrawables.end();
				itDrawable++) {
			(*itDrawable)->Reset(windowFakeScale);
			(*itDrawable)->Draw(rwin);
		}
	}
}

void DrawManager::DrawScreenSpace (sf::RenderWindow& rwin, float windowFakeScale) {
	if (windowFakeScale == 0) {
		for (std::list<ScreenSpaceDrawable*>::iterator itScreenSpaceDrawable = mScreenSpaceDrawables.begin();
				itScreenSpaceDrawable != mScreenSpaceDrawables.end(); itScreenSpaceDrawable++) {
			(*itScreenSpaceDrawable)->Draw(rwin);
		}
	}
	else {
		for (std::list<ScreenSpaceDrawable*>::iterator itScreenSpaceDrawable = mScreenSpaceDrawables.begin();
				itScreenSpaceDrawable != mScreenSpaceDrawables.end(); itScreenSpaceDrawable++) {
			(*itScreenSpaceDrawable)->Reset(windowFakeScale);
			(*itScreenSpaceDrawable)->Draw(rwin);
		}
	}
}

void DrawManager::SortByLayerDepthAscendingWorld () {
	mDrawables.sort( boost::bind(&DrawManager::Comp_layerDepth, this, _1, _2) );
}

void DrawManager::SortByLayerDepthAscendingScreenSpace () {
	mScreenSpaceDrawables.sort( boost::bind(&DrawManager::Comp_layerDepth, this, _1, _2) );
}

bool DrawManager::Comp_layerDepth (DrawableBase* d1, DrawableBase* d2) {
	return d1->layerDepth < d2->layerDepth ? true : false;
}
