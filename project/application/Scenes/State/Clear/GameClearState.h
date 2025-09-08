#pragma once
//ゲームクリアシーン//
#include"Scenes/State/IgameState.h"
#include "Input/Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"
#include "WorldTransform/WorldTransform.h"
#include "Editer/Editer.h"


class GameClearState :public IGameState
{
public:
	GameClearState() { StateNo = GameStateNo::CLEAR; };
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	TextureManager* textureManager_;
	std::unique_ptr<Sprite> clearSprite;
	WorldTransform clear;
	
};
