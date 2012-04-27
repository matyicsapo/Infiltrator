#ifndef TILINGSCREENSPRITE_HPP
#define TILINGSCREENSPRITE_HPP

#include "PFrameWork/GO/Sprite.hpp"
#include "PFrameWork/GO/ScreenSpaceDrawable.hpp"
#include "PFrameWork/GO/ScreenSprite.hpp"

#include "PFrameWork/SFMLGameManager.hpp"
			#include "PFrameWork/StateMachine.hpp"

class TilingScreenSprite : public Sprite, public ScreenSpaceDrawable {
protected:
	DrawManager* const drawManager;

	bool horizontal;

	ScreenSprite* edge01;
	ScreenSprite* edge02;

public:
	TilingScreenSprite (DrawManager* const drawManager,
						std::string edge01Texture,
						std::string edge02Texture,
						std::string middleTexture,
						bool horizontal = true,
						AlignH alignH = LEFT, AlignV alignV = TOP,
						int layerDepth = 0)
			: DrawableBase(layerDepth), Sprite(middleTexture, layerDepth),
			ScreenSpaceDrawable(horizontal ? CENTER : alignH, horizontal ? alignV : MIDDLE, layerDepth),
			drawManager(drawManager),
			horizontal(horizontal) {

		edge01 = new ScreenSprite(edge01Texture, horizontal ? LEFT : alignH, horizontal ? alignV : TOP, layerDepth + 1);
		edge02 = new ScreenSprite(edge02Texture, horizontal ? RIGHT : alignH, horizontal ? alignV : BOTTOM, layerDepth + 1);

		drawManager->AddScreenSpace(edge01);
		drawManager->AddScreenSpace(edge02);

		drawManager->AddScreenSpace(this);
	}

	TilingScreenSprite (TilingScreenSprite const& xTilingScreenSprite)
			: GameObject(xTilingScreenSprite), DrawableBase(xTilingScreenSprite),
			Sprite(xTilingScreenSprite), ScreenSpaceDrawable(xTilingScreenSprite),
			drawManager(xTilingScreenSprite.drawManager) {
		edge01 = new ScreenSprite( *(xTilingScreenSprite.edge01) );
		edge02 = new ScreenSprite( *(xTilingScreenSprite.edge02) );

		drawManager->AddScreenSpace(edge01);
		drawManager->AddScreenSpace(edge02);

		drawManager->AddScreenSpace(this);

		horizontal = xTilingScreenSprite.horizontal;
	}

	~TilingScreenSprite () {
		drawManager->PopScreenSpace(edge01);
		drawManager->PopScreenSpace(edge02);

		delete edge01;
		delete edge02;

		drawManager->PopScreenSpace(this);
	}

	virtual void Draw (sf::RenderWindow& rwin);

	virtual void SetScale (float scale) {
		Sprite::SetScale(scale);
		edge01->SetScale(scale);
		edge02->SetScale(scale);
	}
	virtual void SetScale (sf::Vector2f scale) {
		Sprite::SetScale(scale);
		edge01->SetScale(scale);
		edge02->SetScale(scale);
	}

	virtual void SetPosition (sf::Vector2f position) {
		Sprite::SetPosition(position);
		edge01->SetPosition(position);
		edge02->SetPosition(position);
	}
	virtual void Move (sf::Vector2f offset) {
		Sprite::Move(offset);
		edge01->Move(offset);
		edge02->Move(offset);
	}

	virtual sf::Vector2f GetScreenSize () {
		if (horizontal) {
			return sf::Vector2f(Game->GetRenderWindow()->GetWidth(),
								sfSprite->GetImage()->GetHeight() * sfSprite->GetScale().y);
		}
		else {
			return sf::Vector2f(sfSprite->GetImage()->GetWidth() * sfSprite->GetScale().x,
								Game->GetRenderWindow()->GetHeight());
		}
	}

	virtual sf::Vector2f CalcFakePos ();
};

#endif // TILINGSCREENSPRITE_HPP
