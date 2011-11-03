#ifndef DRAWMANAGER_HPP
#define DRAWMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <list>

#include "Drawable.hpp"
#include "ScreenSpaceDrawable.hpp"

class DrawManager {
private:
	std::list<Drawable*> mDrawables;
	std::list<ScreenSpaceDrawable*> mScreenSpaceDrawables;

	inline bool Comp_layerDepth (DrawableBase* d1, DrawableBase* d2);

    DrawManager () {}

	~DrawManager ();

	DrawManager (DrawManager const&);
	DrawManager& operator= (DrawManager const&);

public:
	static DrawManager* Instance ();

	void DrawWorld (sf::RenderWindow& rwin, float windowFakeScale);
	void DrawScreenSpace (sf::RenderWindow& rwin, float windowFakeScale);

	void Add (Drawable* xDrawable) { mDrawables.push_back(xDrawable); };
	void Pop (Drawable* xDrawable) { mDrawables.remove(xDrawable); }
	void AddScreenSpace (ScreenSpaceDrawable* xScreenSpaceDrawable) { mScreenSpaceDrawables.push_back(xScreenSpaceDrawable); }
	void PopScreenSpace (ScreenSpaceDrawable* xScreenSpaceDrawable) { mScreenSpaceDrawables.remove(xScreenSpaceDrawable); }
	void ClearAll () {
		ClearDrawables();
		ClearScreenSpace();
	}
	void ClearDrawables () { mDrawables.clear(); }
	void ClearScreenSpace () { mScreenSpaceDrawables.clear(); }

	// call manually to avoid multiple, redundant calls when adding more elements one after an other
	void SortByLayerDepthAscendingWorld ();
	void SortByLayerDepthAscendingScreenSpace ();
};

#endif // DRAWMANAGER_HPP
