#include "Coin.hpp"

#include "MyMagicNumbers.hpp"
//#include <iostream>
#include "GameStates/GameState_BasePlayable.hpp"
#include "PFrameWork/SFMLGameManager.hpp"
#include "PFrameWork/StateMachine.hpp"
#include "PFrameWork/GameObjectManager.hpp"
#include "PFrameWork/GO/SoundEffect.hpp"
#include "PFrameWork/SoundManager.hpp"

Coin::Coin (int layerDepth)
		: GameObject(ID_COIN),
		DrawableBase(layerDepth),
		WallCircle(TEXTURE_COIN,
			Collision::TRIGGER,
			layerDepth,
			ID_COIN) {
}

void Coin::OnIntersect (ColliderBase* other) {
	GameState_BasePlayable* playableState = dynamic_cast<GameState_BasePlayable*>(Game->GetGameStateMachine()->Top());

	if (playableState && other->entityType == ID_PLAYER) {
		playableState->PickedUpGold(MyMagicNumbers::goldValue_coin, this);

		GameObjects->Delete(this);

		SoundEffect* coinJingle = new SoundEffect("Content/Audio/coin.wav");
		coinJingle->oneShot = true;
		Sounds->RefreshVol_Effect_Of(coinJingle);
		coinJingle->Play();
	}
}
