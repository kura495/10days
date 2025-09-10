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
	Model* skydome_ = nullptr;
	std::unique_ptr<Sprite>texture;


	std::unique_ptr<Sprite> stressBar;
	float nowStress = 0;
	std::unique_ptr<Sprite> maxStressBar;
	std::unique_ptr<Sprite> stressMeter;
	float MaxStress = 500;
};
