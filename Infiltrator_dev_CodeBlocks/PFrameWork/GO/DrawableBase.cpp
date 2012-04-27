#include "DrawableBase.hpp"

#include "../SFMLGameManager.hpp"
#include "../State.hpp"
#include "../StateMachine.hpp"

void DrawableBase::SetLayerDepth (int layerDepth) {
	this->layerDepth = layerDepth;
}
