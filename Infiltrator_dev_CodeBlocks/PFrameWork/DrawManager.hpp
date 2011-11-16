#ifndef DRAWMANAGER_HPP
#define DRAWMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <list>

class DrawableBase;
class WorldDrawable;
class ScreenSpaceDrawable;
//#include "WorldDrawable.hpp"
//#include "ScreenSpaceDrawable.hpp"

class DrawManager {
private:
	std::list<WorldDrawable*> mWorldSpaceDrawables;
	std::list<ScreenSpaceDrawable*> mScreenSpaceDrawables;

	inline bool Comp_layerDepth (DrawableBase* d1, DrawableBase* d2);

    DrawManager () {}

	~DrawManager ();

	DrawManager (DrawManager const&);
	DrawManager& operator= (DrawManager const&);

public:
	static DrawManager* Instance ();

	void DrawWorldSpace (sf::RenderWindow& rwin);
	void DrawScreenSpace (sf::RenderWindow& rwin);

	void AddWorldSpace (WorldDrawable* xWorldDrawable) { mWorldSpaceDrawables.push_back(xWorldDrawable); };
	void PopWorldSpace (WorldDrawable* xWorldDrawable) { mWorldSpaceDrawables.remove(xWorldDrawable); }
	void AddScreenSpace (ScreenSpaceDrawable* xScreenSpaceDrawable) { mScreenSpaceDrawables.push_back(xScreenSpaceDrawable); }
	void PopScreenSpace (ScreenSpaceDrawable* xScreenSpaceDrawable) { mScreenSpaceDrawables.remove(xScreenSpaceDrawable); }
	void ClearAll () {
		ClearWorldSpace();
		ClearScreenSpace();
	}
	void ClearWorldSpace () { mWorldSpaceDrawables.clear(); }
	void ClearScreenSpace () { mScreenSpaceDrawables.clear(); }

	// call manually to avoid multiple, redundant calls when adding more elements one after an other
	void SortByLayerDepthAscendingWorldSpace ();
	void SortByLayerDepthAscendingScreenSpace ();
};

#define Drawables DrawManager::Instance()

#endif // DRAWMANAGER_HPP
