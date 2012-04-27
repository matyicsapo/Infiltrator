#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include "SFMLEventHandler.hpp"

#include <list>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ConfigParser.hpp"

template<class T>
class StateMachine : public SFMLEventHandler {
private:
	std::list<T*> stateList;

public:
	StateMachine () {}
	StateMachine (T* firstState) {
		stateList.push_back(firstState);
	}

	~StateMachine () {
		for (typename std::list<T*>::iterator itState = stateList.begin();
				itState != stateList.end(); itState++) {
			delete (*itState);
		}
	}

	void Push (T* state) {
		stateList.push_back(state);
	}
	void PopBack () {
		stateList.back()->pop = true;
	}
	void Pop (int cntToPop) {
		int i = 0;
		for (typename std::list<T*>::reverse_iterator itState = stateList.rbegin();
				itState != stateList.rend(); itState++) {
			(*itState)->pop = true;

			if (++i == cntToPop)
				break;
		}
	}
	bool IsEmpty () {
		return stateList.empty();
	}
	void Clear () {
		return stateList.clear();
	}
	T* Top () {
		return stateList.back();
	}

	void RefreshStack () {
		for (typename std::list<T*>::iterator itState = stateList.begin();
				itState != stateList.end(); itState++) {
			if ((*itState)->pop) {
				delete *itState;
				itState = --stateList.erase(itState);
			}
			else
			if ((*itState)->off) {
				(*itState)->Init();
			}
		}
	}

	virtual void HandleSFEvents (std::list<sf::Event>& sfEvents) {
		for (typename std::list<T*>::reverse_iterator itState = stateList.rbegin();
				itState != stateList.rend(); ++itState) {
			if (!(*itState)->off) {
				(*itState)->HandleSFEvents(sfEvents);
				break;
			}
		}
	}

	void Update (float dT) {
	// there was some bug with this somewhere..lol.. => safe and ugly solution
	//		stateList.back()->Update(dT);

		for (typename std::list<T*>::reverse_iterator itState = stateList.rbegin();
				itState != stateList.rend(); ++itState) {
			if (!(*itState)->off) {
				(*itState)->Update(dT);
				break;
			}
		}
	}

	void DrawWorldSpace (sf::RenderWindow& rwin) {
		for (typename std::list<T*>::reverse_iterator itState = stateList.rbegin();
				itState != stateList.rend(); ++itState) {
			if (!(*itState)->off) {
				(*itState)->DrawWorldSpace(rwin);
				break;
			}
		}
	}
	void DrawScreenSpace (sf::RenderWindow& rwin) {
		for (typename std::list<T*>::reverse_iterator itState = stateList.rbegin();
				itState != stateList.rend(); ++itState) {
			if (!(*itState)->off) {
				(*itState)->DrawScreenSpace(rwin);
				break;
			}
		}
	}

	void OnLanguageChanged (CfgContents& contents_lang) {
		for (typename std::list<T*>::iterator itState = stateList.begin();
				itState != stateList.end(); itState++) {
			if (!(*itState)->off) {
				(*itState)->OnLanguageChanged(contents_lang);
			}
		}
	}
};

#endif // STATEMACHINE_HPP
