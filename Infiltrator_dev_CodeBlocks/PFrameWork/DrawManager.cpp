#include "DrawManager.hpp"

#include <boost/bind.hpp>

#include "GO/WorldDrawable.hpp"
#include "GO/ScreenSpaceDrawable.hpp"

DrawManager::~DrawManager () {
	/* THIS SHOULD BE WRITTEN FOR IT IS NOT A SINGLETON ANYMORE, BUT THEN DERIVING SHOULD COMPLY
		IF NOT NULLing SOME DELETED THEN IT MAY BE ATTEMPTED TO DELETE IT TWICE => ERRRRRRRROR
	*/





//	for (std::list<WorldDrawable*>::iterator itDrawable = mWorldSpaceDrawables.begin();
//			itDrawable != mWorldSpaceDrawables.end(); itDrawable++) {
//		// have to subtract 'cause Drawables Pop THEMSELVES from the list
//		// this way the gamestate code only has to call delete on them
//		// but here, on program shutdown it would mess up the iterator references otherwise
//		delete *itDrawable--;
//	}
//
//	for (std::list<ScreenSpaceDrawable*>::iterator itScreenSpaceDrawable = mScreenSpaceDrawables.begin();
//			itScreenSpaceDrawable != mScreenSpaceDrawables.end(); itScreenSpaceDrawable++) {
//		delete *itScreenSpaceDrawable--;
//	}
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

void DrawManager::SortAscendingWorld () {
	mWorldSpaceDrawables.sort( boost::bind(&DrawManager::Comp_layerDepth, this, _1, _2) );
}

void DrawManager::SortAscendingScreen () {
	mScreenSpaceDrawables.sort( boost::bind(&DrawManager::Comp_layerDepth, this, _1, _2) );
}

bool DrawManager::Comp_layerDepth (DrawableBase* d1, DrawableBase* d2) {
	return d1->layerDepth < d2->layerDepth ? true : false;
}
