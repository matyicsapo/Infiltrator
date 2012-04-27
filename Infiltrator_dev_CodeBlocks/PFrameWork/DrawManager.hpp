#ifndef DRAWMANAGER_HPP
#define DRAWMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <list>

class DrawableBase;
class WorldDrawable;
class ScreenSpaceDrawable;

class DrawManager {
private:
	std::list<WorldDrawable*> mWorldSpaceDrawables;
	std::list<ScreenSpaceDrawable*> mScreenSpaceDrawables;

	inline bool Comp_layerDepth (DrawableBase* d1, DrawableBase* d2);

public:
	DrawManager () {}

	virtual ~DrawManager ();

	std::list<WorldDrawable*> GetWorldDrawables () { return mWorldSpaceDrawables; }

	virtual void DrawWorldSpace (sf::RenderWindow& rwin);
	virtual void DrawScreenSpace (sf::RenderWindow& rwin);

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

	void SortAscendingWorld ();
	void SortAscendingScreen ();
};

#define Drawables DrawManager::Instance()

#endif // DRAWMANAGER_HPP
